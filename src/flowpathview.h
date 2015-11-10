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
    FlowPathProps m_properties;
    QColor m_chosen;

public:
    explicit FlowPathView(QWidget *parent = 0);
    ~FlowPathView();

    const FlowPathProps& getProperties() const;

public slots:
    void changeColor();
    int exec() override;

private:
    Ui::FlowPathView *ui;
};

#endif // FLOWPATHVIEW_H
