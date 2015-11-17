#ifndef Q_DEBUGSTREAM_H
#define Q_DEBUGSTREAM_H

#include <streambuf>
#include <string>

#include <QtGlobal>
#include <QString>

namespace megafi
{

class DebugStream : public std::basic_streambuf<wchar_t>
{
public:
    static bool verbose;

#if QT_VERSION >= 0x050000
    static void handle(QtMsgType type, const QMessageLogContext&, const QString& msg);
#else
    static void handle(QtMsgType type, const QString& msg);
#endif

public:
    //constructor
    DebugStream();

    //destructor
    ~DebugStream();

protected:
    //This is called when a std::endl has been inserted into the stream
    virtual int_type overflow(int_type v);

    virtual std::streamsize xsputn(const char *p, std::streamsize n);
};

}
#endif // Q_DEBUGSTREAM_H


