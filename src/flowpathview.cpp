#include "flowpathview.h"
#include "ui_flowpathview.h"

#include <QPalette>
#include <QColorDialog>

FlowPathView::FlowPathView(QWidget *parent) :
    QDialog(parent),
    m_chosen(),
    ui(new Ui::FlowPathView)
{
    // Fill the default line width and colors
    m_properties.lineWidth = 5;
    m_properties.color.setRgb(0, 255, 0);
    m_chosen = m_properties.color;

    // UI setup :
    ui->setupUi(this);
    // - default line width
    ui->lineWidthSB->setValue(m_properties.lineWidth);

    // - default color
    ui->colorButton->setStyleSheet(QString("background-color: %1;").arg(m_properties.color.name()));

    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(changeColor()));

    // Prevents interaction with main window while this one is opened
    setWindowModality(Qt::WindowModal);
}

FlowPathView::~FlowPathView()
{
    delete ui;
}

const FlowPathProps& FlowPathView::getProperties() const
{
    return m_properties;
}

void FlowPathView::changeColor()
{
    m_chosen = QColorDialog::getColor(m_properties.color, this);
    if(m_chosen.isValid())
    {
        ui->colorButton->setStyleSheet(QString("background-color: %1;").arg(m_chosen.name()));
    }
}

int FlowPathView::exec()
{
    DialogCode ret = static_cast<DialogCode>(QDialog::exec());
    switch(ret)
    {
    case QDialog::Accepted:
        m_properties.color     = m_chosen;
        m_properties.lineWidth = ui->lineWidthSB->value();
        break;
    default: break;
    }

    return ret;
}
