#include "lineinputatsetting.h"
#include "ui_lineinputatsetting.h"

LineInputAtSetting::LineInputAtSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineInputAtSetting)
{
    ui->setupUi(this);
    Panel = new QWidget();
    Panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *layout = new QHBoxLayout(Panel);//水平布局管理器
    ColumnNumber = new QLabel();//新建标签
    ColumnNumber->setText(QMainWindow::tr("第0列"));//设置文本
    ColumnNumber->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ColumnNameInput = new QLineEdit();//设置单行输入框
    ColumnNameInput->setText(QMainWindow::tr(""));
    ColumnNameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    NeedChange = new QPushButton();
    NeedChange->setText(QMainWindow::tr("不需要转换代码"));
    NeedChange->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ConnectWeb = new QPushButton();//是否需要链接
    ConnectWeb->setText(QMainWindow::tr("不链接"));
    ConnectWeb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    WebSite = new QLineEdit();//网址
    WebSite->setText(QMainWindow::tr(""));
    WebSite->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    WebSite->setEnabled(false);
    QObject::connect(NeedChange, SIGNAL(clicked()), this, SLOT(MyClicked()));
    QObject::connect(ConnectWeb, SIGNAL(clicked()), this, SLOT(ConnectWebClicked()));
    layout->addWidget(ColumnNumber);
    layout->addWidget(ColumnNameInput);
    layout->addWidget(NeedChange);
    layout->addWidget(ConnectWeb);
    layout->addWidget(WebSite);
}
void LineInputAtSetting::SetCanConnect(bool conn)
{
    if(conn)
    {
        ConnectWeb->setText(QMainWindow::tr("链接到"));
        WebSite->setEnabled(true);
    }
    else
    {
        ConnectWeb->setText(QMainWindow::tr("不链接"));
        WebSite->setEnabled(false);
    }
}
void LineInputAtSetting::SetWebSite(QString webUrl)
{
    WebSite->setText(webUrl);
}
void LineInputAtSetting::SetNumber(int Number)
{
    ColumnNumber->setText(QMainWindow::tr("第") + QString::number(Number) + QMainWindow::tr("列"));
}
void LineInputAtSetting::SetColumnName(QString Column_Name)
{
    ColumnNameInput->setText(Column_Name);
}
void LineInputAtSetting::SetLinePropoties(int Number, QString Column_Name)
{
    ColumnNumber->setText(QMainWindow::tr("第") + QString::number(Number) + QMainWindow::tr("列"));
    ColumnNameInput->setText(Column_Name);
}
QString LineInputAtSetting::GetInputString()
{
    return ColumnNameInput->text();
}
void LineInputAtSetting::AddTo(QVBoxLayout *parent)
{
    parent->addWidget(Panel);
}
void LineInputAtSetting::RemoveFrom(QVBoxLayout *parent)
{
    parent->removeWidget(Panel);
}
LineInputAtSetting::~LineInputAtSetting()
{
    delete ConnectWeb;
    delete WebSite;
    delete NeedChange;
    delete ColumnNameInput;
    delete ColumnNumber;
    delete Panel;
    delete ui;
}
QPushButton* LineInputAtSetting::getButtonObject()
{
    return NeedChange;
}
int LineInputAtSetting::getNumber()
{
    return LineNumber;
}
QString LineInputAtSetting::GetButtonText()
{
    return NeedChange->text();
}
void LineInputAtSetting::SetButtonText(QString text)
{
    NeedChange->setText(text);
}
void LineInputAtSetting::ConnectWebClicked()
{
    if(ConnectWeb->text() == QMainWindow::tr("不链接"))
    {
        ConnectWeb->setText(QMainWindow::tr("链接到"));
        WebSite->setEnabled(true);
    }
    else
    {
        ConnectWeb->setText(QMainWindow::tr("不链接"));
        WebSite->setEnabled(false);
    }
}
void LineInputAtSetting::MyClicked()
{
    QString BText = NeedChange->text();
    if(BText == QMainWindow::tr("不需要转换代码"))
        NeedChange->setText(QMainWindow::tr("布控要求代码"));
    else if(BText == QMainWindow::tr("布控要求代码"))
        NeedChange->setText(QMainWindow::tr("细化代码"));
    else if(BText == QMainWindow::tr("细化代码"))
        NeedChange->setText(QMainWindow::tr("风险布控代码"));
    else NeedChange->setText(QMainWindow::tr("不需要转换代码"));
}
QString LineInputAtSetting::GetWebSite()
{
    if(ConnectWeb->text() == QMainWindow::tr("不链接"))
        return "";
    return WebSite->text();
}
QString LineInputAtSetting::GetButtonSignal()
{
    return this->ConnectWeb->text();
}
