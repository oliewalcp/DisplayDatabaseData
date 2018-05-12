#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <unordered_map>
#include <QString>
#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QSettings>
#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <thread>
#include <condition_variable>
#include <QList>
#include <unordered_set>

struct VICEFUNC
{
    unsigned int refreshTime;
    QString sql;
    QString title;
    void setValue(int T, QString S, QString t)
    {
        refreshTime = T;
        sql = S;
        title = t;
    }
};

class ApplicationData
{
private:
    static QString LoginDataFileName;
    static QString SettingFileName;
    static QString SQLScriptFileName;
    static QString SQLSetFileName;
    static std::unordered_map<int, QString> codeString;//要求代码——中文串
    static std::unordered_map<int, QString> smallString;//细化代码——中文串
    static std::unordered_map<int, QString> riskString;//风险代码——中文串
    static std::unordered_map<int, QString> connectWeb;//列号——基地址
public:
    static std::unordered_map<int, QString> ColumnName;//列名
    static std::unordered_map<int, VICEFUNC*> ViceFunc;
    static unsigned int SoundLongTime;//音效播放最长时长
    static unsigned int RefreshLongTime;//刷新时间
    static QString SoundPath;//音效文件位置
    static QString SQLCode;//SQL查询代码
    static QString UseDB;//使用数据库的SQL代码
    static QString UserID;//登录帐号
    static QString Password;//登录密码
    static QString IPPortNumber;//IP地址和端口

    static std::unordered_set<int> ChangeColumn;//需要转换要求代码的列
    static std::unordered_set<int> SmallColumn;//需要转换细化代码的列
    static std::unordered_set<int> RiskColumn;//需要转换风险代码的列
    static QString smallChineseString;//细化代码的中文串组

    static void ReadLoginDataFile();//读取登录信息文件
    static void SetConnectWeb(int linenumber, QString web);//设置超链接
    static QString GetConnectWeb(int linenumber);//获取超链接地址
    static void ReadSettingFile();//读取用户配置文件
    static void ReadSQLScriptFile();//读取SQL文件
    static void ReadSetSQLFile();//读取SQL代码配置文件
    static void ReadCodeFile();//读取各种代码文件
    static void WriteSetSQLFile();//写入SQL代码配置文件
    static void WriteLoginDataFile();//写入登录信息文件
    static void WriteSettingFile();//写入用户配置文件
    static void WriteSQLScriptFile();//写入SQL文件
    static QString GetRequestCodeString(int code);//获取要求代码对应的中文串
    static QString GetSmallCodeString(int code);//获取细化代码对应的中文串
    static QString GetRiskCodeString(int code);//获取风险布控号对应的中文串
    static void ClearWebSite();//清空网址

    static void SetLocalToCenter(QWidget *qwid);//设置窗口到屏幕中央
};
#endif // APPLICATIONDATA_H
