#include "logwidget.h"

using namespace megafi;

LogWidget* LogWidget::m_log = NULL;

bool LogWidget::isSet() { return m_log != NULL; }
LogWidget& LogWidget::getLog() { return *m_log; }

LogWidget::LogWidget(QWidget *parent) : QPlainTextEdit(parent)
{
    Q_ASSERT(m_log == NULL);
    m_log = this;
}

LogWidget& LogWidget::operator<<(const QString& txt)
{
    appendPlainText(txt);
    return *this;
}

LogWidget& LogWidget::operator<<(const char* txt)
{
    appendPlainText(txt);
    return *this;
}
