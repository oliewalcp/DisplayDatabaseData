#include "vicefunction.h"
#include "ui_vicefunction.h"

ViceFunction::ViceFunction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViceFunction)
{
    ui->setupUi(this);
}
void ViceFunction::InitComponent()
{
    int length = ApplicationData::ViceFunc.size() + 1;
    int contentLength = this->content.size() + 1;
    int i;
    ViceFunctionItem *vfi;
    for(i = 1; i < length; i++)
    {
        if(content.find(i) == content.end())
        {
            vfi = new ViceFunctionItem(ui->widget);
            content.insert(std::make_pair(i, vfi));
            ui->parent->addWidget(vfi);
        }
        else vfi = content[i];
        VICEFUNC *vf = ApplicationData::ViceFunc[i];
        vfi->setTitle(vf->title);
        vfi->setTime(QString::number(vf->refreshTime / 1000));
        vfi->setSQLCode(vf->sql);
    }
    for(; i < contentLength; i++)
        content.erase(content.find(i));
    this->show();
}
ViceFunction::~ViceFunction()
{
    delete ui;
}
//取消按钮
void ViceFunction::on_pushButton_2_clicked()
{
    this->hide();
}
//确定按钮
void ViceFunction::on_pushButton_clicked()
{
    ApplicationData::ViceFunc.clear();
    int length2 = content.size() + 1;
    int i;
    for(i = 1; i < length2; i++)
    {
        ViceFunctionItem *item = content[i];
        VICEFUNC *temp = new VICEFUNC;
        temp->setValue(item->getTime() * 1000, item->getSQLCode(), item->getTitle());
        ApplicationData::ViceFunc[i] = temp;
    }
    ApplicationData::WriteSetSQLFile();
    this->hide();
}
//增加按钮
void ViceFunction::on_pushButton_3_clicked()
{
    ViceFunctionItem *temp = new ViceFunctionItem(ui->widget);
    content.insert(std::make_pair(content.size() + 1, temp));
    ui->parent->addWidget(temp);
}
//减少按钮
void ViceFunction::on_pushButton_4_clicked()
{
    if(content.empty()) return;
    int index = content.size();
    ViceFunctionItem *temp = content[index];
    content.erase(content.find(index));
    temp->hide();
    ui->parent->removeWidget(temp);
}
