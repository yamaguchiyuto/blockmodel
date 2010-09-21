/* vim:set ts=4 sw=4 sts=4 et: */

#ifndef BLOCKMODEL_IO_HPP
#define BLOCKMODEL_IO_HPP

#include <iostream>
#include <block/blockmodel.h>

/// Supported formats in the IO subsystem
typedef enum {
    FORMAT_NULL, FORMAT_PLAIN, FORMAT_JSON
} Format;

/***************************************************************************/

/// Abstract templatized reader class
/**
 * Specializations of this class must implement reading (deserializing)
 * some kind of an object in some format from an input stream.
 */
template <typename T>
class Reader {
public:
    /// Reads into the given object from the given stream
    virtual void read(T& model, std::istream& is) = 0;
};

/// Reader for block models in plain text format
template <typename T>
class PlainTextReader : public Reader<T> {
    /// Reads into the given object from the given stream
    virtual void read(T& model, std::istream& is);
};

/***************************************************************************/

/// Abstract templatized writer class
/**
 * Specializations of this class must implement writing (serializing)
 * some kind of an object in some format to an output stream.
 */
template <typename T>
class Writer {
public:
    /// Writes the given object to the given stream
    virtual void write(const T& model, std::ostream& os) = 0;
};

/// Null writer that does nothing
template <typename T>
class NullWriter : public Writer<T> {
    /// Does nothing.
    virtual void write(const T&, std::ostream&) {}
};

/// Writer for block models in plain text format
template <typename T>
class PlainTextWriter : public Writer<T> {
    /// Writes the given object to the given stream
    virtual void write(const T& model, std::ostream& os);
};

/// Writer for block models in JSON format
template <typename T>
class JSONWriter : public Writer<T> {
public:
    /// Writes the given object to the given stream
    virtual void write(const T& model, std::ostream& os);
};

#endif

