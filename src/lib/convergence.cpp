/* vim:set ts=4 sw=4 sts=4 et: */

#include <cmath>
#include <limits>
#include <sstream>
#include <block/convergence.h>
#include <block/math.hpp>

using igraph::Vector;

bool EntropyConvergenceCriterion::check(const Vector& samples) {
    double entropy = samples.sum() / samples.size();
    bool result = (!isnan(m_lastEntropy) && 
                   std::fabs(entropy - m_lastEntropy) < m_threshold);
    m_lastEntropy = entropy;
    return result;
}

std::string EntropyConvergenceCriterion::report() const {
    if (isnan(m_lastEntropy))
        return "no estimated entropy so far";

    std::ostringstream oss;
    oss << "estimated entropy of distribution: ";
    oss << m_lastEntropy;
    return oss.str();
}

void EntropyConvergenceCriterion::reset() {
    m_lastEntropy = std::numeric_limits<float>::quiet_NaN();
}

