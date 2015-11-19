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

LogWidget& LogWidget::operator<<(QString txt)
{
    txt.chop(1);
    appendPlainText(txt);
    return *this;
}

LogWidget& LogWidget::operator<<(char* txt)
{
    if(txt)
    {
        // Get last character
        unsigned long i = 0;
        for(i=0; txt[i] != '\0'; ++i);

        // Save trailing newline in 'last'
        char last = 0;
        if(i)
        {
            last = txt[i-1];
            // Delete trailing newline
            txt[i-1] = '\0';
        }

        appendPlainText(txt);

        // Revert
        if(i)
            txt[i-1] = last;
    }

    return *this;
}
