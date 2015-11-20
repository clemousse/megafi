#ifndef LOGWIDGET_H
#define LOGWIDGET_H


#include <QPlainTextEdit>

namespace megafi
{

class LogWidget : public QPlainTextEdit
{
    static LogWidget* m_log;

public:
    static bool isSet();
    static LogWidget &getLog();

public:
    LogWidget(QWidget* parent = NULL);
    LogWidget& operator<<(QString txt);
    LogWidget& operator<<(char* txt);
};

}
#endif // LOGWIDGET_H
