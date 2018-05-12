#include "versioninstructions.h"
#include "ui_versioninstructions.h"

VersionInstructions::VersionInstructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionInstructions)
{
    ui->setupUi(this);
}

VersionInstructions::~VersionInstructions()
{
    delete ui;
}

void VersionInstructions::on_pushButton_clicked()
{
    this->hide();
}
