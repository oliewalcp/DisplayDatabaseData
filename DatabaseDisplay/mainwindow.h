#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSound>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRect>
#include <QRegExp>
#include <QTableWidget>
#include <QSqlResult>
#include <QVariant>
#include <QAxObject>
#include <QFont>
#include <QMessageBox>
#include <set>
#include <list>
#include <QPointer>
#include <QDesktopServices>
#include <QStandardPaths>
#include "settingface.h"
#include "loginface.h"
#include "multi_key_map.h"
#include "sqlface.h"
#include "vicefunction.h"
#include "viceresult.h"
#include "versioninstructions.h"

struct TIMEQUEUE
{
    int id;
    unsigned int time;//刷新时间
    QString sql;//SQL代码
    ViceResult *vr;
    bool equals(const TIMEQUEUE *t)
    {
        return this->time == t->time;
    }
    void setValue(const int ID, unsigned int Time, QString SQL, ViceResult *VR)
    {
        id = ID;
        time = Time;
        sql = SQL;
        vr = VR;
    }
};
//用于排序
struct compare
{
    bool operator()(const TIMEQUEUE *t1, const TIMEQUEUE *t2)const  //重载（）运算符
    {
        return t1->time <= t2->time && t1->id != t2->id;
    }
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //用户排序的比较函数

private slots:
    void Item_clicked(int row, int column);

private:
    Ui::MainWindow *ui;
    SettingFace *settingface;//设置界面
    ViceFunction *viceFunc;//副功能设置界面
    LoginFace *loginface;//登录界面
    SQLFace *sqlface;//SQL界面
    VersionInstructions *versionInstruc;//版本说明界面
    QSound *sound;//声音对象
    QTableWidget *resultpanel;//结果面板
    bool AlreadyPlaySound;//标记此轮刷新是否已播放音效
    std::set<TIMEQUEUE*, compare> *TimeQueue;//副功能计时消息队列
    std::unordered_map<int, ViceResult*> leftResult;//ID——组件
    std::unordered_map<int, ViceResult*> rightResult;//ID——组件

    Multi_key_map ResultTableValue;
    bool Visible;//标记主窗口是否显示
    QTimer *timer;//表格数据的计时器
    QTimer *ViceTimer;//副功能的计时器
    void SetMenuBar();//设置菜单栏
    void DisplayResult();//显示数据
    void SetTray();//设置系统托盘显示
    void ExitApp();//退出程序
    void SetApplicationVisible();//设置程序窗口可见性
    void SetShow();//设置窗口显示
    void SetHide();//设置窗口隐藏
    void StopSound();//停止音效
    void EnterSettingFace();//进入设置界面
    void EnterSQLFace();//进入SQL代码设计界面
    void EnterViceFuncFace();//进入副功能设置界面
    void EnterVersion();//进入版本说明界面
    void PlaySound();//播放音效
    void DisplayColumnName();//显示列名
    void ExportExcel();//导出excel文件
    QString GetCorrespondingString(QString (*func)(int), QString str);//获取代码对应的字符串
    QTableWidgetItem* CreateItem(QString Text);//创建单元格
    void SetLineColor(int lineNumber, QColor *Color);//设置一行的文本颜色
    void ApplicationResponse(QSystemTrayIcon::ActivationReason reason);
    void timerEvent();//基本计时器
    void ViceTimerEvent();//副功能计时器
    void closeEvent(QCloseEvent *event);
    QList<int> *getCode(QString codestring);
};

#endif // MAINWINDOW_H
