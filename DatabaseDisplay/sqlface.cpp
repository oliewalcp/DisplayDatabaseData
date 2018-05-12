#include "sqlface.h"
#include "ui_sqlface.h"

SQLFace::SQLFace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLFace)
{
    ui->setupUi(this);
}

SQLFace::~SQLFace()
{
    delete ui;
}
void SQLFace::EnterSQLSettingFace()
{
    ui->QuerySQL->setText(ApplicationData::SQLCode);
    ui->USEDatabaseString->setText(ApplicationData::UseDB);
    this->show();
}
//确定按钮事件
void SQLFace::on_pushButton_2_clicked()
{
    ApplicationData::SQLCode = ui->QuerySQL->toPlainText();
    ApplicationData::UseDB = ui->USEDatabaseString->text();
    ApplicationData::WriteSQLScriptFile();
    this->hide();
}
//取消按钮事件
void SQLFace::on_pushButton_clicked()
{
    this->hide();
}
