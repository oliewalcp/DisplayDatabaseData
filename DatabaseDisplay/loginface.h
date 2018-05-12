#ifndef LOGINFACE_H
#define LOGINFACE_H

#include <QSqlDatabase>
#include <QList>
#include <QTimer>
#include <QSqlQuery>
#include "applicationdata.h"
#include "connectdbfailed.h"

namespace Ui {
class LoginFace;
}

class LoginFace : public QDialog
{
    Q_OBJECT

public:
    explicit LoginFace(QTimer *time, QTimer *viceTimer, QWidget *parent = 0);
    ~LoginFace();

    bool getStatus();//获取登录状态
    QSqlQuery* getQueryObject(bool &success);
    QSqlQuery* getQueryObject(QString sql, bool &success);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    QWidget *Application;
    Ui::LoginFace *ui;
    ConnectDBFailed *cdbf;
    QTimer *timer;
    QTimer *vicetimer;
    bool AlreadyLogin;//标记是否已登录
    QSqlDatabase *conn;

    bool LoginEnterDatabase();//连接数据库
    void closeEvent(QCloseEvent *event);
};

#endif // LOGINFACE_H
