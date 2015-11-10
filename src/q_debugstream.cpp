#include "q_debugstream.h"

//constructor
Q_DebugStream::Q_DebugStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
{
    log_window = text_edit;
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
}


//destructor
Q_DebugStream::~Q_DebugStream()
{
    m_stream.rdbuf(m_old_buf);
}


void Q_DebugStream::registerQDebugMessageHandler()
{
    qInstallMessageHandler(myQDebugMessageHandler);
}


void Q_DebugStream::myQDebugMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    std::cout << msg.toStdString().c_str();
}


Q_DebugStream::int_type Q_DebugStream::overflow(Q_DebugStream::int_type v)
{
    if (v == '\n')
    {
        log_window->append("");
    }
    return v;
}


std::streamsize Q_DebugStream::xsputn(const char *p, std::streamsize n)
{
    QString str(p);
    if(str.contains("\n"))
    {
        QStringList strSplitted = str.split("\n");
        //Move to the end of the document.
        log_window->moveCursor (QTextCursor::End);
        log_window->insertPlainText (strSplitted.at(0)); //Index 0 is still on the same old line

        for(int i = 1; i < strSplitted.size(); i++)
        {
            log_window->append(strSplitted.at(i));
        }
    }
    else
    {
        //Move to the end of the document.
        log_window->moveCursor (QTextCursor::End);
        //convenience slot that inserts text at the current cursor position. Here, at the end.
        log_window->insertPlainText (str);
    }
    return n;
}