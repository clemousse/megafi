#include "flowpathview.h"
#include "ui_flowpathview.h"

#include <QPalette>
#include <QColorDialog>

FlowPathView::FlowPathView(QWidget *parent) :
    QDialog(parent),
    m_chosen(),
    ui(new Ui::FlowPathView)
{
    // UI setup :
    ui->setupUi(this);

    // Prevents interaction with main window while this one is opened
    setWindowModality(Qt::WindowModal);
}

FlowPathView::~FlowPathView()
{
    delete ui;
}


void FlowPathView::changeColor()
{
    m_chosen = QColorDialog::getColor(m_chosen, this);
    if(m_chosen.isValid())
    {
        ui->colorButton->setStyleSheet(QString("background-color: %1;").arg(m_chosen.name()));
    }
}

int FlowPathView::exec()
{
    throw "Exec should not be called";
}

QDialog::DialogCode FlowPathView::changeProps(megafi::FlowPathProps &props)
{
    // Initialization
    // - default line width
    ui->lineWidthSB->setValue(props.lineWidth);
    // - default color
    m_chosen = QColor(props.color.r, props.color.g, props.color.b);
    ui->colorButton->setStyleSheet(QString("background-color: %1;").arg(m_chosen.name()));

    // Open dialog
    DialogCode ret = static_cast<DialogCode>(QDialog::exec());
    switch(ret)
    {
    case QDialog::Accepted:
        props.color.r   = m_chosen.red();
        props.color.g   = m_chosen.green();
        props.color.b   = m_chosen.blue();
        props.lineWidth = ui->lineWidthSB->value();
        break;
    default: break;
    }

    return ret;
}
