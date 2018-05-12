#include "sqleditframe.h"
#include "ui_sqleditframe.h"

SQLEditFrame::SQLEditFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLEditFrame)
{
    ui->setupUi(this);
}
void SQLEditFrame::EnterEdit(QLabel *label)
{
    this->label = label;
    ui->textEdit->setText(label->text());
    this->show();
}
SQLEditFrame::~SQLEditFrame()
{
    delete ui;
}
//取消按钮
void SQLEditFrame::on_pushButton_2_clicked()
{
    this->hide();
}
//确定按钮
void SQLEditFrame::on_pushButton_clicked()
{
    this->label->setText(ui->textEdit->toPlainText());
    this->hide();
}
