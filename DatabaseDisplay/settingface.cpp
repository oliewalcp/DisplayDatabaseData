#include "settingface.h"
#include "ui_settingface.h"

SettingFace::SettingFace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingFace)
{
    ui->setupUi(this);
    Application = parent;
    QValidator *val = new QIntValidator(0, 200000000, this);//设定输入框的输入范围
    ui->FlushLongTime->setValidator(val);
    ui->SoundLong->setValidator(val);
    ColumnAreaParent = new QVBoxLayout(ui->DisplayArea);
}

SettingFace::~SettingFace()
{
    delete ui;
}

void SettingFace::SetInitData()
{
    ui->FlushLongTime->setText(QString::number(ApplicationData::RefreshLongTime / 1000));//设置刷新间隔时间
    ui->MusicPath->setText(ApplicationData::SoundPath == ":/resource/tip.wav" ? QMainWindow::tr("默认音效") : ApplicationData::SoundPath);//设置音频文件路径
    ui->SoundLong->setText(QString::number(ApplicationData::SoundLongTime / 1000));//设置音效时长
    int lengthList = LineList.size(), i;
    int lengthName = ApplicationData::ColumnName.size();
    if(lengthName <= lengthList)
    {
        for(i = 1; i < lengthName + 1; i++)
            LineList[i]->SetColumnName(ApplicationData::ColumnName[i]);
        for(; i < lengthList; i++)
        {
            auto it = LineList.find(i);
            LineInputAtSetting *Line = it->second;
            Line->RemoveFrom(ColumnAreaParent);
            LineList.erase(it);
        }
    }
    else
    {
        for(i = 1; i < lengthList + 1; i++)
            LineList[i]->SetColumnName(ApplicationData::ColumnName[i]);
        for(; i < lengthName + 1; i++)
        {
            QString cn = ApplicationData::ColumnName[i];
            LineInputAtSetting *Line = new LineInputAtSetting(this);
            Line->SetLinePropoties(i, cn);
            if(ApplicationData::ChangeColumn.find(i) != ApplicationData::ChangeColumn.end())
                Line->SetButtonText(QMainWindow::tr("布控要求代码"));
            else if(ApplicationData::SmallColumn.find(i) != ApplicationData::SmallColumn.end())
                Line->SetButtonText(QMainWindow::tr("细化代码"));
            else if(ApplicationData::RiskColumn.find(i) != ApplicationData::RiskColumn.end())
                Line->SetButtonText(QMainWindow::tr("风险布控代码"));
            QString web = ApplicationData::GetConnectWeb(i);
            if(web != "")
            {
                Line->SetCanConnect(true);
                Line->SetWebSite(web);
            }
            Line->AddTo(ColumnAreaParent);
            LineList[i] = Line;
        }
    }
}
//取消按钮事件
void SettingFace::on_pushButton_4_clicked()
{
    this->hide();
}
//确定按钮事件
void SettingFace::on_pushButton_3_clicked()
{
    int lengthList = LineList.size() + 1, i;
    int lengthName = ApplicationData::ColumnName.size() + 1;
    ApplicationData::ChangeColumn.clear();
    ApplicationData::SmallColumn.clear();
    ApplicationData::ClearWebSite();
    //修改显示的列值
    for(i = 1; i < lengthList; i++)
    {
        LineInputAtSetting *line = LineList[i];
        ApplicationData::ColumnName[i] = line->GetInputString();
        if(line->GetButtonText() == "布控要求代码")
            ApplicationData::ChangeColumn.insert(i);
        else if(line->GetButtonText() == "细化代码")
            ApplicationData::SmallColumn.insert(i);
        else if(line->GetButtonText() == "风险布控代码")
            ApplicationData::RiskColumn.insert(i);
        if(line->GetButtonSignal() == "链接到")
            ApplicationData::SetConnectWeb(i, line->GetWebSite());
    }
    //删除多余的列
    for(; i < lengthName; i++)
        ApplicationData::ColumnName.erase(ApplicationData::ColumnName.find(i));
    QString music = ui->MusicPath->text();
    ApplicationData::SoundPath = music == "默认音效" ? QMainWindow::tr(":/resource/tip.wav") : music;//保存音频文件路径
    ApplicationData::RefreshLongTime = ui->FlushLongTime->text().toInt() * 1000;
    ApplicationData::SoundLongTime = ui->SoundLong->text().toInt() * 1000;
    ApplicationData::WriteSettingFile();
    this->hide();
}
//增加一列按钮事件
void SettingFace::on_pushButton_5_clicked()
{
    int nowNumber = LineList.size() + 1;
    LineInputAtSetting *Line = new LineInputAtSetting(this);
    Line->SetNumber(nowNumber);
    if(ApplicationData::ChangeColumn.find(nowNumber) != ApplicationData::ChangeColumn.end())
        Line->SetButtonText(QMainWindow::tr("布控要求代码"));
    else if(ApplicationData::SmallColumn.find(nowNumber) != ApplicationData::SmallColumn.end())
        Line->SetButtonText(QMainWindow::tr("细化代码"));
    else if(ApplicationData::RiskColumn.find(nowNumber) != ApplicationData::RiskColumn.end())
        Line->SetButtonText(QMainWindow::tr("风险布控代码"));
    Line->AddTo(ColumnAreaParent);
    LineList[nowNumber] = Line;
}
//减少一列按钮事件
void SettingFace::on_pushButton_6_clicked()
{
    auto it = LineList.find(LineList.size());
    LineInputAtSetting *Line = it->second;
    Line->RemoveFrom(ColumnAreaParent);
    LineList.erase(it);
}
//还原按钮事件
void SettingFace::on_pushButton_2_clicked()
{
    ui->MusicPath->setText(QMainWindow::tr("默认音效"));
}
//浏览按钮事件
void SettingFace::on_pushButton_clicked()
{
    //选择一个文件
    QString filename = QFileDialog::getOpenFileName(
        this,
        QMainWindow::tr("选择一个音频文件"),
        QMainWindow::tr("/"),
        QMainWindow::tr("音频 (*.wav)"));
    if(filename.isEmpty()) return;
    ui->MusicPath->setText(filename);
}
