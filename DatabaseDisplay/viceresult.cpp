#include "viceresult.h"
#include "ui_viceresult.h"

ViceResult::ViceResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViceResult)
{
    ui->setupUi(this);
}
void ViceResult::SetLabel(QString str)
{
    ui->label->setText(str);
}
void ViceResult::SetResult(QString str)
{
    ui->label_2->setText(str);
}
ViceResult::~ViceResult()
{
    delete ui;
}
