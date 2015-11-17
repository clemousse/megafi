#ifndef Q_DEBUGSTREAM_H
#define Q_DEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <QMessageBox>
#include <QTextEdit>

class Q_DebugStream : public std::basic_streambuf<char>
{

public:

    //constructor
    Q_DebugStream(std::ostream &stream, QTextEdit* text_edit);

    //destructor
    ~Q_DebugStream();

    static void registerQDebugMessageHandler();


private:
    std::ostream &m_stream;
    std::streambuf* m_old_buf;
    QTextEdit* log_window;

    static void myQDebugMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg);

protected:
    //This is called when a std::endl has been inserted into the stream
    virtual int_type overflow(int_type v);

    virtual std::streamsize xsputn(const char *p, std::streamsize n);

};

#endif // Q_DEBUGSTREAM_H


