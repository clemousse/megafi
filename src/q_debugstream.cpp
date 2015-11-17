#include "q_debugstream.h"
#include "logwidget.h"

#include <iostream>

using namespace megafi;

#ifdef QT_NO_DEBUG_OUTPUT
bool DebugStream::verbose = false;
#else
bool DebugStream::verbose = true;
#endif

#if QT_VERSION >= 0x050000
void DebugStream::handle(QtMsgType type, const QMessageLogContext&, const QString& msg)
#else
void DebugStream::handle(QtMsgType type, const QString& msg)
#endif
{
    //"std::cout <<" append the text
    //"toStdString()" allows to return a std::string object with the data contained in this QString.
            //The Unicode data is converted into 8-bit characters using the toAscii() function.
    //"c_str()" return a pointer toward the array of char which contains the object of string type.

    switch (type)
    {
       case QtDebugMsg:
            if(verbose)
                std::wcout << msg.toStdWString();
            break;
       case QtWarningMsg:
            std::wcerr << msg.toStdWString();
            break;
       case QtCriticalMsg:
            break;
       case QtFatalMsg:
            //abort();
            break;
//for critical messages : QMessageBox::critical(this,"Critical Message","Test");
    }
}


//constructor
DebugStream::DebugStream() : basic_streambuf()
{
}


//destructor
DebugStream::~DebugStream()
{
}



DebugStream::int_type DebugStream::overflow(DebugStream::int_type v)
{
    if (v == '\n')
    {
        //log_window->append("");
    }
    return v;
}


std::streamsize DebugStream::xsputn(const char *p, std::streamsize n)
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
