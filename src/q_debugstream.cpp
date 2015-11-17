#include "q_debugstream.h"


//constructor
Q_DebugStream::Q_DebugStream(std::ostream &stream, QTextEdit* text_edit)
    :m_stream(stream),
      m_oldBuf(stream.rdbuf()),
      m_logWindow(text_edit)

{
    stream.rdbuf(this);
}


//destructor
Q_DebugStream::~Q_DebugStream()
{
    m_stream.rdbuf(m_oldBuf);
    qInstallMessageHandler(0);
}


void Q_DebugStream::registerQDebugMessageHandler()
{
    qInstallMessageHandler(myQDebugMessageHandler);
}


void Q_DebugStream::myQDebugMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    //"std::cout <<" append the text
    //"toStdString()" allows to return a std::string object with the data contained in this QString.
            //The Unicode data is converted into 8-bit characters using the toAscii() function.
    //"c_str()" return a pointer toward the array of char which contains the object of string type.

    switch (type)
    {
       case QtDebugMsg:
            std::cout << msg.toStdString();
            break;
       case QtWarningMsg:
            std::cerr << msg.toStdString();
            //m_logWindow << msg;
            break;
       case QtCriticalMsg:
            qInstallMessageHandler(0);
       case QtFatalMsg:
            abort();
            break;
//for critical messages : QMessageBox::critical(this,"Critical Message","Test");
    }
}



Q_DebugStream::int_type Q_DebugStream::overflow(Q_DebugStream::int_type v)
{
    if (v == '\n')
    {
        //log_window->append("");
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
        //log_window->moveCursor (QTextCursor::End);
        //log_window->insertPlainText (strSplitted.at(0)); //Index 0 is still on the same old line

        for(int i = 1; i < strSplitted.size(); i++)
        {
            //log_window->append(strSplitted.at(i));
        }
    }
    else
    {
        //Move to the end of the document.
        //log_window->moveCursor (QTextCursor::End);
        //convenience slot that inserts text at the current cursor position. Here, at the end.
        //log_window->insertPlainText (str);
    }
    return n;
}
