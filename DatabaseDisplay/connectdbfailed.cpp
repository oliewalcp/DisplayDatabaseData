#include "connectdbfailed.h"
#include "ui_connectdbfailed.h"

ConnectDBFailed::ConnectDBFailed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDBFailed)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
}

ConnectDBFailed::~ConnectDBFailed()
{
    delete ui;
}

void ConnectDBFailed::on_pushButton_clicked()
{
    this->hide();
}
