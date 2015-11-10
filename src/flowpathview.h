#ifndef FLOWPATHVIEW_H
#define FLOWPATHVIEW_H

#include <QDialog>
#include <QColor>

namespace Ui {
class FlowPathView;
}

struct FlowPathProps
{
    float lineWidth;
    QColor color;
};

class FlowPathView : public QDialog
{
    Q_OBJECT

private:
    QColor m_chosen;

public:
    explicit FlowPathView(QWidget *parent = 0);
    ~FlowPathView();

public slots:
    int exec() override;
    DialogCode changeProps(FlowPathProps& props);

protected slots:
    void changeColor();

private:
    Ui::FlowPathView *ui;
};

#endif // FLOWPATHVIEW_H
