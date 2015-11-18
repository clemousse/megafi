#include "logwidget.h"

using namespace megafi;

LogWidget* LogWidget::m_log = NULL;

bool LogWidget::isSet() { return m_log != NULL; }
LogWidget& LogWidget::getLog() { return *m_log; }

LogWidget::LogWidget(QWidget *parent) : QPlainTextEdit(parent), m_lock()
{
    Q_ASSERT(m_log == NULL);
    m_log = this;
}

LogWidget& LogWidget::operator<<(const QString& txt)
{
    m_lock.lock();
    appendPlainText(txt);
    m_lock.unlock();
    return *this;
}

LogWidget& LogWidget::operator<<(const char* txt)
{
    m_lock.lock();
    appendPlainText(txt);
    m_lock.unlock();
    return *this;
}
