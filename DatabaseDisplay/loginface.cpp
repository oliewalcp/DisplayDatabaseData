#include "loginface.h"
#include "ui_loginface.h"

LoginFace::LoginFace(QTimer *time, QTimer *viceTimer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginFace)
{
    ui->setupUi(this);
    this->timer = time;
    this->vicetimer = viceTimer;
    Application = parent;
    AlreadyLogin = false;
    ApplicationData::ReadLoginDataFile();//获取用户帐号数据
    ui->IPPortNumber->setText(ApplicationData::IPPortNumber);
    ui->UserID->setText(ApplicationData::UserID);
    ui->Password->setText(ApplicationData::Password);
    ui->Password->setEchoMode(QLineEdit::Password);
    this->setWindowModality(Qt::ApplicationModal);//阻塞非当前窗口
    cdbf = new ConnectDBFailed(parent);
}

LoginFace::~LoginFace()
{
    delete conn;
    delete cdbf;
    delete ui;
}
//退出按钮按下事件
void LoginFace::on_pushButton_2_clicked()
{
    exit(0);
}
//窗口关闭事件（重写）
void LoginFace::closeEvent(QCloseEvent *event)
{
    exit(0);
}
//登录按钮按下事件
void LoginFace::on_pushButton_clicked()
{
    ApplicationData::UserID = ui->UserID->text();
    ApplicationData::Password = ui->Password->text();
    ApplicationData::IPPortNumber = ui->IPPortNumber->text();
    if(LoginEnterDatabase())
    {
        AlreadyLogin = true;
        ApplicationData::WriteLoginDataFile();
        timer->setInterval(1000);
        vicetimer->setInterval(1000);
        vicetimer->start();
        timer->start();
        this->hide();
    }
    else cdbf->show();//连接失败，弹出提示框
}
//判断是否成功连接数据库
bool LoginFace::LoginEnterDatabase()
{
    QList<QString> host = ApplicationData::IPPortNumber.split(":");
    QSqlDatabase con = QSqlDatabase::addDatabase("QODBC");
    conn = &con;
    QString dsn = QString("Driver={sql server};SERVER=%1;PORT=%2;UID=%3;PWD=%4;")
        .arg(host[0])//主机IP
        .arg(host[1].toInt())//端口号
        .arg(ApplicationData::UserID)//用户名
        .arg(ApplicationData::Password);//密码
    conn->setDatabaseName(dsn);//设置驱动
    return conn->open();
}
//获取登录状态
bool LoginFace::getStatus()
{
    return AlreadyLogin;
}
QSqlQuery* LoginFace::getQueryObject(bool &success)
{
    QSqlQuery* query = new QSqlQuery();
    query->exec(ApplicationData::UseDB);
    query->prepare(ApplicationData::SQLCode);
    success = query->exec();
    return query;
}
QSqlQuery* LoginFace::getQueryObject(QString sql, bool &success)
{
    if(sql == "")
    {
        success = false;
        return nullptr;
    }
    QSqlQuery* query = new QSqlQuery();
    query->exec(ApplicationData::UseDB);
    query->prepare(sql);
    success = query->exec();
    return query;
}
