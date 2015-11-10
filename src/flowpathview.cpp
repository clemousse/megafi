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

    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(changeColor()));

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
    m_chosen = QColor(props.color.r *255, props.color.g *255, props.color.b *255);
    ui->colorButton->setStyleSheet(QString("background-color: %1;").arg(m_chosen.name()));

    // Open dialog
    DialogCode ret = static_cast<DialogCode>(QDialog::exec());
    switch(ret)
    {
    case QDialog::Accepted:
        props.color.r   = m_chosen.redF();
        props.color.g   = m_chosen.greenF();
        props.color.b   = m_chosen.blueF();
        props.lineWidth = ui->lineWidthSB->value();
        break;
    default: break;
    }

    return ret;
}
