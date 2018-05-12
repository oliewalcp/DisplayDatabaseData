#include "applicationdata.h"

std::unordered_map<int, QString> ApplicationData::ColumnName;
QString ApplicationData::IPPortNumber = "127.0.0.1:1433";
QString ApplicationData::SoundPath = ":/resource/tip.wav";
unsigned int ApplicationData::SoundLongTime = 5000;
QString ApplicationData::SQLCode = QMainWindow::tr("select * into #tempMENG FROM (") +
"(SELECT W.ENTRY_ID,W.CREATE_DATE AS D_DATE,H.TRADE_NAME,L.G_NAME,I.RSK_NO,P.RSK_REQUEST_STR ,E.EXAM_MODE_CODE" +
"FROM dbo.ENTRY_WORKFLOW W" +
"JOIN dbo.ENTRY_HEAD H ON W.ENTRY_ID=H.ENTRY_ID" +
"JOIN dbo.ENTRY_LIST L ON W.ENTRY_ID=L.ENTRY_ID" +
"LEFT JOIN [H2000_IMS_RECV].[dbo].[RSK_EXAM_HEAD_REL] E ON E.FORM_ID =H.ENTRY_ID" +
"LEFT JOIN [H2000_IMS_RECV].[dbo].[RSK_FIT_LOG_REL] I ON E.FORM_ID=I.FORM_ID" +
"LEFT JOIN [H2000_IMS_RECV].[dbo].[RSK_INSTR_PROC_REL] P ON E.RSK_NO=P.RSK_NO" +
"WHERE CREATE_DATE >(convert(varchar(10),getdate(),120) + ' 00:00:00')" +
"AND STEP_ID LIKE '51%' AND L.G_NO='1'" +
"OR (CREATE_DATE >(convert(varchar(10),getdate(),120) + ' 00:00:00') AND STEP_ID LIKE '53%' AND PROC_RESULT='1' AND L.G_NO='1' AND I.FIT_FLAG='1' AND PROC_ER LIKE '67%'))" +
"UNION" +
"(SELECT H.ENTRY_ID,R.D_DATE,H.TRADE_NAME,L.G_NAME,R.RSK_NO,I.RSK_REQUEST_STR ,E.EXAM_MODE_CODE" +
"FROM [H2000_IMS_RECV].[dbo].[RSK_FIT_LOG_REL] R" +
"INNER JOIN [H2000_IMS_RECV].[dbo].[ENTRY_HEAD] H ON R.FORM_ID=H.ENTRY_ID" +
"INNER JOIN  [H2000_IMS_RECV].[dbo].[ENTRY_LIST] L ON H.ENTRY_ID=L.ENTRY_ID" +
"LEFT JOIN [H2000_IMS_RECV].[dbo].[RSK_INSTR_PROC_REL] I ON R.RSK_NO=I.RSK_NO" +
"LEFT JOIN [H2000_IMS_RECV].[dbo].[RSK_EXAM_HEAD_REL] E ON E.FORM_ID =R.FORM_ID" +
"WHERE R.D_DATE >  (convert(varchar(10),getdate(),120) + ' 00:00:00')" +
"AND FIT_FLAG='1'" +
"AND H.I_E_PORT LIKE '67%'" +
"AND L.G_NO='1'))T" +
"SELECT ENTRY_ID,D_DATE,TRADE_NAME,G_NAME,RSK_NO,RSK_REQUEST_STR,EXAM_MODE_CODE FROM #tempMENG T" +
"--ORDER BY ENTRY_ID" +
"WHERE NOT EXISTS (SELECT 1 FROM #tempMENG WHERE T.ENTRY_ID = ENTRY_ID AND T.D_DATE > D_DATE)" +
"--GROUP BY ENTRY_ID,TRADE_NAME,G_NAME,RSK_NO,RSK_REQUEST_STR,EXAM_MODE_CODE" +
"ORDER BY D_DATE DESC" +
"DROP TABLE #tempMENG";

QString ApplicationData::UseDB = "use H2000_IMS_RECV";
unsigned int ApplicationData::RefreshLongTime = 100000;
QString ApplicationData::LoginDataFileName = "ldf.ini";//用户名、密码、IP端口地址
QString ApplicationData::SettingFileName = "stf.ini";//列名、音效文件、音效时长、刷新间隔时间
QString ApplicationData::SQLScriptFileName = "ssf.ini";//SQL代码
QString ApplicationData::SQLSetFileName = "sql.ini";
QString ApplicationData::UserID = "";
QString ApplicationData::Password = "";
std::unordered_map<int, QString> ApplicationData::codeString;//要求代码——中文串
std::unordered_map<int, QString> ApplicationData::smallString;//细化代码——中文串
std::unordered_map<int, QString> ApplicationData::riskString;//风险代码——中文串
std::unordered_map<int, QString> ApplicationData::connectWeb;//列号——网址基址
std::unordered_set<int> ApplicationData::ChangeColumn;
std::unordered_set<int> ApplicationData::SmallColumn;
std::unordered_set<int> ApplicationData::RiskColumn;//需要转换风险代码的列
std::unordered_map<int, VICEFUNC*> ApplicationData::ViceFunc;
QString ApplicationData::smallChineseString = "";//细化代码的中文串组

void ApplicationData::ReadLoginDataFile()
{
    QSettings *setfile = new QSettings(LoginDataFileName, QSettings::IniFormat);
    UserID = setfile->value("i").toString();
    Password = setfile->value("w").toString();
    QString str = setfile->value("n").toString();
    if(str != "") IPPortNumber = str;
    delete setfile;
}
void ApplicationData::ReadSettingFile()
{
    ChangeColumn.clear();
    ColumnName.clear();
    auto getColumn = [&](QString &s, std::unordered_set<int> &set){
        QList<QString> cod = s.split(" ");
        for(QList<QString>::Iterator it = cod.begin(); it != cod.end(); it++)
            set.insert((*it).toInt());
    };
    QSettings *setfile = new QSettings(SettingFileName,QSettings::IniFormat);
    QString str = setfile->value("sl").toString();;
    if(str != "") SoundLongTime = str.toInt();
    str = setfile->value("sp").toString();
    if(str != "") SoundPath = str;
    str = setfile->value("rl").toString();
    if(str != "") RefreshLongTime = str.toInt();
    str = setfile->value("co").toString();
    if(str != "") getColumn(str, ChangeColumn);
    str = setfile->value("sm").toString();
    if(str != "") getColumn(str, SmallColumn);
    str = setfile->value("ri").toString();
    if(str != "") getColumn(str, RiskColumn);
    int i = 1;
    for(i = 1; (str = setfile->value("cn" + QString::number(i)).toString()) != ""; i++)
        ColumnName[i] = str;
    for(i = 1; (str = setfile->value("cnn" + QString::number(i)).toString()) != ""; i++)
    {
        QList<QString> val = str.split(" ");
        connectWeb[val.at(0).toInt()] = val.at(1);
    }
    if(ColumnName.empty())
    {
        QString string[] = {"报关单号", "申报日期", "经营单位名称", "商品名称", "风险布控号", "查验要求", "查验类型"};
        i = 1;
        for(QString s : string)
            ColumnName.insert(std::make_pair(i++, s));
    }
    delete setfile;
}
void ApplicationData::ReadSQLScriptFile()
{
    QSettings *setfile = new QSettings(SQLScriptFileName, QSettings::IniFormat);
    QString str = setfile->value("udb").toString();;
    if(str != "") UseDB = str;
    str = setfile->value("que").toString();
    if(str != "") SQLCode = str;
    delete setfile;
}
void ApplicationData::ReadSetSQLFile()
{
    QSettings *setfile = new QSettings(SQLSetFileName, QSettings::IniFormat);
    QString str = "";
    for(int i = 1; true; i++)
    {
        VICEFUNC *vice = new VICEFUNC;
        str = setfile->value(QMainWindow::tr("s") + QString::number(i)).toString();
        vice->sql = str;
        str = setfile->value(QMainWindow::tr("l") + QString::number(i)).toString();
        vice->title = str;
        str = setfile->value(QMainWindow::tr("t") + QString::number(i)).toString();
        if(str == "" && vice->sql == "" && vice->title == "") break;
        if(vice->title == "") vice->title = "0";
        vice->refreshTime = str.toInt();
        ViceFunc.insert(std::make_pair(i, vice));
    }
    delete setfile;
}
void ApplicationData::WriteSetSQLFile()
{
    QFile::remove(SQLSetFileName);
    QSettings *setfile = new QSettings(SQLSetFileName, QSettings::IniFormat);
    int length = ViceFunc.size() + 1;
    QString str;
    VICEFUNC *temp;
    for(int i = 1; i < length; i++)
    {
        temp = ViceFunc[i];
        str = QMainWindow::tr("s") + QString::number(i);
        setfile->setValue(str, temp->sql);
        str = QMainWindow::tr("t") + QString::number(i);
        setfile->setValue(str, temp->refreshTime);
        str = QMainWindow::tr("l") + QString::number(i);
        setfile->setValue(str, temp->title);
    }
    delete setfile;
}
void ApplicationData::WriteLoginDataFile()
{
    QFile::remove(LoginDataFileName);
    QFile file(LoginDataFileName);
    if(file.exists()) file.remove();
    QSettings *setfile = new QSettings(LoginDataFileName,QSettings::IniFormat);
    setfile->setValue("i", UserID);
    setfile->setValue("w", Password);
    setfile->setValue("n", IPPortNumber);
    delete setfile;
}
void ApplicationData::WriteSettingFile()
{
    QFile::remove(SettingFileName);
    QFile file(SettingFileName);
    if(file.exists()) file.remove();
    QSettings *setfile = new QSettings(SettingFileName,QSettings::IniFormat);
    int i = 1;
    QString str = "", Fstr = "", Rstr = "";
    for(int co : ChangeColumn)
        str += QString::number(co) + QMainWindow::tr(" ");
    for(int co : SmallColumn)
        Fstr += QString::number(co) + QMainWindow::tr(" ");
    for(int co : RiskColumn)
        Rstr += QString::number(co) + QMainWindow::tr(" ");
    for(auto it = connectWeb.begin(); it != connectWeb.end(); it++, i++)
        setfile->setValue("cnn" + QString::number(i), QString::number(it->first) + " " + it->second);
    setfile->setValue("co", str);
    setfile->setValue("sm", Fstr);
    setfile->setValue("ri", Rstr);
    setfile->setValue("sl", QString::number(SoundLongTime));
    setfile->setValue("sp", SoundPath);
    setfile->setValue("rl", QString::number(RefreshLongTime));
    int length = ColumnName.size() + 1;
    for(i = 1; i < length; i++)
        setfile->setValue(QMainWindow::tr("cn") + QString::number(i), ColumnName[i]);
    delete setfile;
}
void ApplicationData::WriteSQLScriptFile()
{
    QFile::remove(SQLScriptFileName);
    QFile file(SQLScriptFileName);
    if(file.exists()) file.remove();
    QSettings *setfile = new QSettings(SQLScriptFileName,QSettings::IniFormat);
    setfile->setValue("udb", UseDB);
    setfile->setValue("que", SQLCode);
    delete setfile;
}
void ApplicationData::SetLocalToCenter(QWidget *qwid)
{
    qwid->move((QApplication::desktop()->width() - qwid->width()) / 2, (QApplication::desktop()->height() - qwid->height()) / 2);
}
void ApplicationData::ReadCodeFile()
{
    auto readfile = [&](QString filename, std::unordered_map<int, QString> &cont){
        QFile file(filename);
        bool isS = filename == "small.txt";
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        QTextStream ts(&file);
        QString chin;
        int code;
        while(!ts.atEnd())
        {
            ts >> code >> chin;
            if(isS) smallChineseString += chin;
            cont.insert(std::make_pair(code, chin));
        }
    };
    readfile("Request.txt", codeString);
    readfile("small.txt", smallString);
    readfile("Risk.txt", riskString);
}
QString ApplicationData::GetRequestCodeString(int code)
{
    if(codeString.find(code) == codeString.end())
        return QString::number(code);
    return codeString[code];
}
QString ApplicationData::GetSmallCodeString(int code)
{
    if(smallString.find(code) == smallString.end())
        return QString::number(code);
    return smallString[code];
}
QString ApplicationData::GetRiskCodeString(int code)
{
    if(riskString.find(code) == riskString.end())
        return QString::number(code);
    return riskString[code];
}
QString ApplicationData::GetConnectWeb(int linenumber)
{
    if(connectWeb.find(linenumber) == connectWeb.end())
        return "";
    return connectWeb[linenumber];
}
void ApplicationData::SetConnectWeb(int linenumber, QString web)
{
    connectWeb[linenumber] = web;
}
void ApplicationData::ClearWebSite()
{
    connectWeb.clear();
}
