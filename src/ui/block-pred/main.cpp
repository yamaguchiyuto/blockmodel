/* vim:set ts=4 sw=4 sts=4 et: */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <block/blockmodel.h>
#include <block/io.hpp>
#include <block/optimization.h>
#include <block/util.hpp>
#include <igraph/cpp/graph.h>

#include "../common/logging.h"
#include "../common/string_util.h"
#include "cmd_arguments.h"

using namespace igraph;
using namespace std;

class BlockmodelPredictionApp {
private:
    /// Parsed command line arguments
    CommandLineArguments m_args;

    /// Markov chain Monte Carlo strategy to optimize the model
    MetropolisHastingsStrategy m_mcmc;

public:
    LOGGING_FUNCTION(debug, 2);
    LOGGING_FUNCTION(info, 1);
    LOGGING_FUNCTION(error, 0);

    /// Constructor
    BlockmodelPredictionApp() {}

    /// Returns whether we are running in quiet mode
    bool isQuiet() {
        return m_args.verbosity < 1;
    }

    /// Returns whether we are running in verbose mode
    bool isVerbose() {
        return m_args.verbosity > 1;
    }

    /// Reads the model from the disk
    int readModel(UndirectedBlockmodel& model) {
        auto_ptr<Reader<UndirectedBlockmodel> > pModelReader;

        info(">> loading model: %s", m_args.inputFile.c_str());

        switch (m_args.inputFormat) {
            default:
                pModelReader.reset(new PlainTextReader<UndirectedBlockmodel>);
                break;
        };

        try {
            if (m_args.inputFile == "-")
                pModelReader->read(model, cin);
            else {
                ifstream is(m_args.inputFile.c_str());
                pModelReader->read(model, is);
            }
        } catch (const runtime_error& ex) {
            error("Cannot read input file: %s", m_args.inputFile.c_str());
            error(ex.what());
            return 1;
        }

        return 0;
    }

    /// Runs the user interface
    int run(int argc, char** argv) {
        UndirectedBlockmodel model;
        unsigned int samplesTaken = 0;
        double logL, bestLogL;
        MersenneTwister* rng = m_mcmc.getRNG();

        m_args.parse(argc, argv);

        if (m_args.sampleCount <= 0)
            return 0;

        if (readModel(model))
            return 1;

        debug(">> using random seed: %lu", m_args.randomSeed);
        rng->init_genrand(m_args.randomSeed);

        m_mcmc.setModel(&model);
        
        // If we have requested only one sample, then always take the one
        // we have in the beginning
        if (m_args.sampleCount == 1)
            m_args.samplingFreq = 1.0;

        if (model.getGraph() == NULL && m_args.sampleCount > 1) {
            error("Loaded model has no associated graph; cannot start sampling.");
            return 2;
        }

        info(">> starting Markov chain");
        bestLogL = model.getLogLikelihood();

        while (1) {
            if (rng->random() < m_args.samplingFreq) {
                samplesTaken++;
                // TODO
            }

            if (samplesTaken >= m_args.sampleCount)
                break;

            m_mcmc.step();

            logL = model.getLogLikelihood();
            if (bestLogL < logL)
                bestLogL = logL;

            if (m_mcmc.getStepCount() % m_args.logPeriod == 0 && !isQuiet()) {
                clog << '[' << setw(6) << m_mcmc.getStepCount() << "] "
                     << '(' << setw(6) << samplesTaken << ") "
                     << setw(12) << logL << "\t(" << bestLogL << ")\t"
                     << (m_mcmc.wasLastProposalAccepted() ? '*' : ' ')
                     << setw(8) << m_mcmc.getAcceptanceRatio()
                     << '\n';
            }
        }

        return 0;
    }
};

int main(int argc, char** argv) {
    BlockmodelPredictionApp app;

    igraph::AttributeHandler::attach();
    return app.run(argc, argv);
}