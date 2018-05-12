#include "vicefunctionitem.h"
#include "ui_vicefunctionitem.h"
#include <QDebug>

ViceFunctionItem::ViceFunctionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViceFunctionItem)
{
    ui->setupUi(this);
    editFrame = new SQLEditFrame(this);
    QValidator *val = new QIntValidator(0, 2000000000, this);//设定输入框的输入范围
    ui->lineEdit_2->setValidator(val);
    ui->label_3->setMaximumHeight(ui->label_3->size().height());
    ui->label_3->setMaximumWidth(250);
}
QString ViceFunctionItem::getSQLCode()
{
    return ui->label_3->text();
}
int ViceFunctionItem::getTime()
{
    return ui->lineEdit_2->text().toInt();
}
QString ViceFunctionItem::getTitle()
{
    return ui->lineEdit->text();
}
void ViceFunctionItem::setSQLCode(QString sql)
{
    ui->label_3->setText(sql);
}
void ViceFunctionItem::setTitle(QString title)
{
    ui->lineEdit->setText(title);
}
void ViceFunctionItem::setTime(QString time)
{
    ui->lineEdit_2->setText(time);
}
ViceFunctionItem::~ViceFunctionItem()
{
    delete ui;
}
void ViceFunctionItem::on_pushButton_clicked()
{
    editFrame->hide();
    editFrame->EnterEdit(ui->label_3);
}
