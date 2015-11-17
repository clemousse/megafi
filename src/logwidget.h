#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QPlainTextEdit>

namespace megafi
{

class LogWidget : public QPlainTextEdit
{
public:
    LogWidget(QWidget* parent = NULL);
    LogWidget& operator<<(const QString& txt);
};

}
#endif // LOGWIDGET_H
