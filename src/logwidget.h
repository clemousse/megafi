#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QPlainTextEdit>
#include <QMutex>

namespace megafi
{

class LogWidget : public QPlainTextEdit
{
    QMutex m_lock;
    static LogWidget* m_log;

public:
    static bool isSet();
    static LogWidget &getLog();

public:
    LogWidget(QWidget* parent = NULL);
    LogWidget& operator<<(const QString& txt);
    LogWidget& operator<<(const char* txt);
};

}
#endif // LOGWIDGET_H
