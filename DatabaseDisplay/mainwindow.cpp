#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationdata.h"
//界面程序的开端
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resource/icon.png"));
    Visible = true;
    TimeQueue = new std::set<TIMEQUEUE*, compare>;
    //初始化表格
    resultpanel = ui->tableWidget;
    connect(resultpanel, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(Item_clicked(int,int)));//关联单元格双击事件
    resultpanel->setShowGrid(true);
    resultpanel->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    resultpanel->setSelectionBehavior(QAbstractItemView::SelectItems);  //设置选择行为时每次选择一格
    resultpanel->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置为不可编辑方式
    resultpanel->setAutoScroll(true);//设置自动滚动条
    resultpanel->setHorizontalHeaderItem(0, CreateItem(tr(" ")));
    connect(resultpanel, SIGNAL(itemChanged(QTableWidgetItem*)), resultpanel, SLOT(resizeRowsToContents()) );//设置自动调整行高
    //新建计时器
    timer = new QTimer(this);
    ViceTimer = new QTimer(this);
    //关联函数到计时器事件
    connect(timer, &QTimer::timeout, this, &this->timerEvent);
    connect(ViceTimer, &QTimer::timeout, this, &this->ViceTimerEvent);
    //初始化各界面
    sqlface = new SQLFace(this);
    settingface = new SettingFace(this);
    loginface = new LoginFace(timer, ViceTimer, this);
    viceFunc = new ViceFunction(this);
    versionInstruc = new VersionInstructions(this);
    //读取配置文件
    ApplicationData::ReadCodeFile();
    ApplicationData::SetLocalToCenter(this);
    ApplicationData::SetLocalToCenter(loginface);
    ApplicationData::ReadLoginDataFile();//读取登录信息
    loginface->show();//显示登录界面
    ApplicationData::ReadSetSQLFile();
    //将计时器事件添加到消息队列
    SetTray();//设置系统托盘图标
    SetMenuBar();//设置菜单栏
    ApplicationData::ReadSettingFile();//获取用户自定义设置数据
    DisplayColumnName();
    ApplicationData::ReadSQLScriptFile();//获取用户自定义SQL代码
}
void MainWindow::ViceTimerEvent()
{
    ViceTimer->stop();
    unsigned int min = 0x7fffffff;//最小刷新时间
    TIMEQUEUE *tempQueue;
    QVariant result;//保存SQL查询的结果
    std::set<TIMEQUEUE*, compare> *exchangeAnother = new std::set<TIMEQUEUE*, compare>;
    int i, length = ApplicationData::ViceFunc.size() + 1, childLength = leftResult.size() + rightResult.size();
    //添加消息队列
    for(i = 1; i < length; i++)
    {
        if(rightResult.find(i) != rightResult.end())
        {
            rightResult[i]->SetLabel(ApplicationData::ViceFunc[i]->title);
            continue;
        }
        if(leftResult.find(i) != leftResult.end())
        {
            leftResult[i]->SetLabel(ApplicationData::ViceFunc[i]->title);
            continue;
        }
        TIMEQUEUE *tq = new TIMEQUEUE;
        VICEFUNC *temp = ApplicationData::ViceFunc[i];
        ViceResult *vr;
        if(leftResult.size() <= rightResult.size())
        {
            //添加到左边容器
            vr = new ViceResult(ui->widget_2);
            ui->Left->addWidget(vr);
            this->leftResult[i] = vr;
        }
        else
        {
            //添加到右边容器
            vr = new ViceResult(ui->widget_3);
            ui->Right->addWidget(vr);
            this->rightResult[i] = vr;
        }
        vr->SetLabel(temp->title);
        tq->setValue(i, temp->refreshTime, temp->sql, vr);
        TimeQueue->insert(tq);//插入消息队列
    }
    //删除没有的部分
    for(; i <= childLength; i++)
    {
        auto it = rightResult.find(i);
        if(it != rightResult.end())
        {
            it->second->hide();
            ui->Right->removeWidget(it->second);
            rightResult.erase(it);
            continue;
        }
        it = leftResult.find(i);
        if(it != leftResult.end())
        {
            it->second->hide();
            ui->Left->removeWidget(it->second);
            leftResult.erase(it);
        }
    }
    //获取时间最短的消息
    for(auto it = TimeQueue->begin(); it != TimeQueue->end(); it++)
    {
        tempQueue = *it;
        if(ApplicationData::ViceFunc.find(tempQueue->id) == ApplicationData::ViceFunc.end()) continue;
        if(tempQueue->time > min)
        {
            tempQueue->time -= min;
            exchangeAnother->insert(tempQueue);
            continue;
        }
        else if(min == 0x7fffffff) min = tempQueue->time;
        bool success;
        QSqlQuery *query = loginface->getQueryObject(tempQueue->sql, success);//获取结果
        if(success)
        {
            if(query->next())
            {
                result = query->value(0);//获取数据
                QString res = result.isNull() ? "0" : result.toString();
                tempQueue->vr->SetResult(res);
            }
            tempQueue->time = ApplicationData::ViceFunc[tempQueue->id]->refreshTime;
            exchangeAnother->insert(tempQueue);//插入消息队列
        }
    }
    //替换消息队列
    delete TimeQueue;
    TimeQueue = exchangeAnother;
    auto it = TimeQueue->begin();
    if(it == TimeQueue->end()) ViceTimer->setInterval(2000);
    else ViceTimer->setInterval((*it)->time);//设置计时器时长为最短需求时间间隔
    ViceTimer->start();
}
void MainWindow::timerEvent()
{
    AlreadyPlaySound = false;
    timer->stop();
    DisplayResult();
    timer->setInterval(ApplicationData::RefreshLongTime);
    timer->start();//开始计时
}
void MainWindow::PlaySound()
{
    if(AlreadyPlaySound) return;
    AlreadyPlaySound = true;
    auto ps = [&](){
        std::chrono::milliseconds dura(ApplicationData::SoundLongTime);
        std::this_thread::sleep_for(dura);
        sound->stop();
    };
    sound = new QSound(ApplicationData::SoundPath);
    sound->play();
    std::thread t(ps);
    t.detach();
}
void MainWindow::StopSound()
{
    sound->stop();
}
void MainWindow::EnterSettingFace()
{
    settingface->SetInitData();
    settingface->show();
}
void MainWindow::EnterSQLFace()
{
    sqlface->EnterSQLSettingFace();
}
void MainWindow::EnterViceFuncFace()
{
    viceFunc->hide();
    viceFunc->InitComponent();
}
void MainWindow::EnterVersion()
{
    versionInstruc->show();
}
void MainWindow::SetMenuBar()
{
    QMenuBar *QMB = ui->menuBar;//获取菜单条
    //　新建菜单（QMenu）
    QMenu* FileMenu = new QMenu(tr("文件"));
    QMenu* EditMenu = new QMenu(tr("编辑"));
    QMenu* VersionMenu = new QMenu(tr("版本"));
    QMB->addMenu(FileMenu);
    QMB->addMenu(EditMenu);
    QMB->addMenu(VersionMenu);
    // 新建菜单项（QAction）
    QAction* Export = new QAction(tr("导出excel"));
    QAction* HideAction = new QAction(tr("隐藏"));
    QAction* StopSoundAction = new QAction(tr("停止音效"));
    QAction* ExitAction = new QAction(tr("退出"));
    QAction* SetAction = new QAction(tr("设置"));
    QAction* SQLAction = new QAction(tr("SQL"));
    QAction* ViceAction = new QAction(tr("副功能"));
    QAction* VersionAction = new QAction(tr("版本说明"));
    FileMenu->addAction(Export);
    FileMenu->addAction(HideAction);
    FileMenu->addAction(StopSoundAction);
    FileMenu->addAction(ExitAction);
    EditMenu->addAction(SetAction);
    EditMenu->addAction(SQLAction);
    EditMenu->addAction(ViceAction);
    VersionMenu->addAction(VersionAction);
    connect(Export, &QAction::triggered, this, &this->ExportExcel);//响应“导出excel”
    connect(StopSoundAction, &QAction::triggered, this, &this->StopSound);//响应“停止音效”
    connect(HideAction, &QAction::triggered, this, &this->SetHide);//响应“隐藏”
    connect(ExitAction, &QAction::triggered, this, &this->ExitApp);//响应“退出”
    connect(SetAction, &QAction::triggered, this, &this->EnterSettingFace);//响应“设置”
    connect(SQLAction, &QAction::triggered, this, &this->EnterSQLFace);//响应“SQL”
    connect(ViceAction, &QAction::triggered, this, &this->EnterViceFuncFace);//响应“副功能”
    connect(VersionAction, &QAction::triggered, this, &this->EnterVersion);//响应“版本说明”
}
void MainWindow::SetTray()
{
    QSystemTrayIcon *qsti = new QSystemTrayIcon(this);
    qsti->setIcon(QIcon(":/resource/icon.png"));
    qsti->setToolTip("海关报关单实时监控");
    QMenu *RightMouseMenu = new QMenu(this);
    QAction *Exit = new QAction(RightMouseMenu);
    QAction *Open = new QAction(RightMouseMenu);
    QAction *Edit = new QAction(RightMouseMenu);
    QAction *SQL = new QAction(RightMouseMenu);
    QAction* ViceAction = new QAction(RightMouseMenu);
    Exit->setText(tr("退出"));
    Open->setText(tr("显示"));
    Edit->setText(tr("设置"));
    SQL->setText(tr("SQL"));
    ViceAction->setText(tr("副功能"));
    RightMouseMenu->addAction(Open);
    RightMouseMenu->addAction(Edit);
    RightMouseMenu->addAction(SQL);
    RightMouseMenu->addAction(ViceAction);
    RightMouseMenu->addAction(Exit);
    connect(Exit, &QAction::triggered, this, &this->ExitApp);//响应“退出”
    connect(Open, &QAction::triggered, this, &this->SetShow);//响应“显示”
    connect(qsti, &QSystemTrayIcon::activated, this, &this->ApplicationResponse);//设置托盘图标的消息
    connect(Edit, &QAction::triggered, this, &this->EnterSettingFace);//响应“设置”
    connect(SQL, &QAction::triggered, this, &this->EnterSQLFace);//响应“SQL”
    connect(ViceAction, &QAction::triggered, this, &this->EnterViceFuncFace);//响应“副功能”
    qsti->setContextMenu(RightMouseMenu);//设置系统托盘右键菜单
    qsti->show();
}
void MainWindow::ApplicationResponse(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        SetApplicationVisible();
}
void MainWindow::Item_clicked(int row, int column)
{
    QTableWidgetItem* temp = ResultTableValue.getValue(row + 1, column + 1);
    QString webURL = ApplicationData::GetConnectWeb(column + 1);
    if(webURL == "") return;
    QDesktopServices::openUrl(QUrl(webURL + temp->text()));
}
void MainWindow::DisplayColumnName()
{
    QFont font;
    font.setPointSize(9);
    font.setBold(true);
    int length = ApplicationData::ColumnName.size() + 1, i;
    int widlength = ResultTableValue.ColumnSize(1);
    for(i = length; i < widlength; i++)
        resultpanel->removeColumn(i);
    resultpanel->setColumnCount(length - 1);
    QStringList header;
    for(int i = 1; i < length; i++)
        header << ApplicationData::ColumnName[i];
    resultpanel->setHorizontalHeaderLabels(header);
}
void MainWindow::SetApplicationVisible()
{
    if(Visible) SetHide();
    else SetShow();
}
void MainWindow::SetShow()
{
    this->show();
    Visible = true;
}
void MainWindow::SetHide()
{
    this->hide();
    Visible = false;
}
void MainWindow::ExitApp()
{
    exit(0);
}
void MainWindow::DisplayResult()
{
    DisplayColumnName();
    AlreadyPlaySound = false;
    bool success;
    QSqlQuery *query = loginface->getQueryObject(success);
    if(!success) return;
    QVariant result;//保存一个数据
    int i = 0, length = ApplicationData::ColumnName.size();
    int childNumber = ResultTableValue.LineSize();
    QTableWidgetItem *temp, *smallTemp = nullptr;
    QString changeTemp = "";
    bool Is23, SmallNotNull, isSmallItem;
    QRegExp re(ApplicationData::GetRequestCodeString(23)), timeStr("[\\d-:]{1,}[T]{1}[\\d-:]{1,}");
    //遍历结果集
    for(int number = 0; query->next(); number++)
    {
        Is23 = false;//是否是23
        SmallNotNull = false;//细化代码是否为空
        temp = nullptr;
        smallTemp = nullptr;
        changeTemp = "";
        //如果上次没有那么多行
        if(number >= childNumber)
        {
            resultpanel->insertRow(number);
            PlaySound();
        }
        //遍历结果
        for(i = 0; i < length; i++)
        {
            isSmallItem = false;
            result = query->value(i);//先获取一个数据
            QString res = result.isNull() ? "NULL" : result.toString();
            QString str = "";
            //如果是布控要求代码
            if(!ApplicationData::ChangeColumn.empty() && ApplicationData::ChangeColumn.find(i + 1) != ApplicationData::ChangeColumn.end())
            {
                str = GetCorrespondingString(ApplicationData::GetRequestCodeString, res);
                if(re.indexIn(str) != -1 && !Is23) Is23 = true;
                changeTemp = str;
            }
            //如果是细化代码
            else if(!ApplicationData::SmallColumn.empty() && ApplicationData::SmallColumn.find(i + 1) != ApplicationData::SmallColumn.end())
            {
                isSmallItem = true;
                str = GetCorrespondingString(ApplicationData::GetSmallCodeString, res);
                if(str != "" && str != "NULL") SmallNotNull = true;
            }
            //如果是风险代码
            else if(!ApplicationData::RiskColumn.empty() && ApplicationData::RiskColumn.find(i + 1) != ApplicationData::RiskColumn.end())
                str = GetCorrespondingString(ApplicationData::GetRiskCodeString, res);
            //如果是时间字符串
            else if(timeStr.indexIn(res) != -1)
                str = res.replace("T", " ");
            if(str != "")
                res = str;
            temp = ResultTableValue.getValue(number + 1, i + 1);
            if(temp == nullptr)//如果没有这一列（防止结果列数比设定的列数还多）
            {
                //为当前行增加列
                temp = CreateItem(res);
                resultpanel->setItem(number, i, temp);
                ResultTableValue.setValue(number + 1, i + 1, temp);
            }
            else temp->setText(res);//如果该列已存在，则设置文本框
            if(isSmallItem) smallTemp = temp;
        }
        //到此遍历完一条记录，开始处理
        SetLineColor(number + 1, new QColor(90, 90, 90));
        ui->ResultNumberLabel->setText(tr("共有") + QString::number(number + 1) + tr("条记录"));
        if(Is23 && SmallNotNull) continue;//如果要求代码是23，且细化代码不为空
        else if(!SmallNotNull && !Is23)
        {
            smallTemp->setText(tr("NULL"));
            SetLineColor(number + 1, new QColor(255, 0, 0));
            continue;
        }
        QList<QString> ChineseString = changeTemp.split("\n");//获取要求代码的中文串链表
//        Contain = true;//细化代码是否包含要求代码
        QString smallString = smallTemp->text();
        for(QString s : ChineseString)
        {
            QRegExp re(s);
            //如果细化代码没有该要求代码
            if(re.indexIn(smallString) == -1)
            {
//                Contain = false;
                SetLineColor(number + 1, new QColor(255, 0, 0));
                break;
            }
        }
//        //如果细化代码包含了当前的要求代码
//        if(Contain && smallTemp != nullptr)
//        {
//            QString oriString = smallTemp->text();
//            for(QString s : ChineseString)
//            {
//                QRegExp re(s);
//                if(re.indexIn(oriString) != -1) continue;
//                if(changeTemp != "") oriString = ((oriString == "NULL" || oriString == "") ? "" : (oriString + "\n")) + s;
//            }
//            smallTemp->setText(oriString);
//        }
//        else SetLineColor(number + 1, new QColor(255, 0, 0));
    }
}
MainWindow::~MainWindow()
{
    delete TimeQueue;
    delete ViceTimer;
    delete sqlface;
    delete settingface;
    delete loginface;
    delete sound;
    delete ui;
}
QTableWidgetItem* MainWindow::CreateItem(QString Text)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(Text);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    return item;
}
void MainWindow::SetLineColor(int lineNumber, QColor *Color)
{
    int RealColumnLength = ApplicationData::ColumnName.size();
    int length = ResultTableValue.ColumnSize(lineNumber);
    QTableWidgetItem *temp;
    int i;
    for(i = 0; i < RealColumnLength; i++)
    {
       temp = ResultTableValue.getValue(lineNumber, i + 1);
       temp->setTextColor(*Color);
    }
    for(; i < length; i++)
        ResultTableValue.removeObject(lineNumber, i + 1);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    exit(0);
}
QList<int> *MainWindow::getCode(QString codestring)
{
    QRegExp re("[^0-9]+");
    if(re.indexIn(codestring) != -1) return 0;
    else if(codestring.length() % 2 != 0 || codestring.isEmpty() || codestring == "") return 0;//字符串数量不是2的倍数
    QList<int> *list = new QList<int>();
    int length = codestring.length();
    for(int i = 0; i < length; i = i + 2)
    {
        int temp = codestring.mid(i, 2).toInt();
        list->push_back(temp);
    }
    return list;
}
QString MainWindow::GetCorrespondingString(QString (*func)(int), QString str)
{
    QString res = tr("");
    QList<int> *list = getCode(str);
    if(list != 0)
    {
        for(int n = 0; n < list->size(); n++)
        {
            QString temp = func(list->at(n));
            if(temp == "") continue;
            if(res != "") res += tr("\n");
            res += temp;
        }
    }
    return res;
}
void MainWindow::ExportExcel()
{
     QString fileName = QFileDialog::getSaveFileName(this, "保存", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Excel 文件(*.xls *.xlsx)");
     if (fileName != "")
     {
         QAxObject *excel = new QAxObject;
         if (excel->setControl("Excel.Application")) //连接Excel控件
         {
             excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
             excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
             QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
             workbooks->dynamicCall("Add");//新建一个工作簿
             QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
             QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

             int i,j;
             int colcount = resultpanel->columnCount();
             int rowcount = resultpanel->rowCount();
             QAxObject *cell, *col;
             //标题行
             QString title = tr("报关单表");
             cell = worksheet->querySubObject("Cells(int,int)", 1, 1);
             cell->dynamicCall("SetValue(const QString&)", title);
             cell->querySubObject("Font")->setProperty("Size", 18);
             //调整行高
             worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
             //合并标题行
             QString cellTitle;
             cellTitle.append("A1:");
             cellTitle.append(QChar(colcount - 1 + 'A'));
             cellTitle.append(QString::number(1));
             QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
             range->setProperty("WrapText", true);
             range->setProperty("MergeCells", true);
             range->setProperty("HorizontalAlignment", -4108);//水平居中
             range->setProperty("VerticalAlignment", -4108);//垂直居中
             //列标题
             for(i = 0; i < colcount; i++)
             {
                 QString columnName;
                 columnName.append(QChar(i  + 'A'));
                 columnName.append(":");
                 columnName.append(QChar(i + 'A'));
                 col = worksheet->querySubObject("Columns(const QString&)", columnName);
                 col->setProperty("ColumnWidth", resultpanel->columnWidth(i) / 6);
                 cell = worksheet->querySubObject("Cells(int,int)", 2, i + 1);
                 columnName = resultpanel->horizontalHeaderItem(i)->text();
                 cell->dynamicCall("SetValue(const QString&)", columnName);
                 cell->querySubObject("Font")->setProperty("Bold", true);
                 cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                 cell->setProperty("HorizontalAlignment", -4108);//水平居中
                 cell->setProperty("VerticalAlignment", -4108);//垂直居中
             }
             //QTableWidget 获取表格数据部分
             for(i = 0; i < rowcount; i++)
                 for(j = 0; j < colcount; j++)
                     worksheet->querySubObject("Cells(int,int)", i + 3, j + 1)->dynamicCall("SetValue(const QString&)", resultpanel->item(i, j) ? resultpanel->item(i, j)->text() : "");
             //画框线
             QString lrange;
             lrange.append("A2:");
             lrange.append(colcount - 1 + 'A');
             lrange.append(QString::number(rowcount + 2));
             range = worksheet->querySubObject("Range(const QString&)", lrange);
             range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
             range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
             //调整数据区行高
             QString rowsName;
             rowsName.append("2:");
             rowsName.append(QString::number(rowcount + 2));
             range = worksheet->querySubObject("Range(const QString&)", rowsName);
             range->setProperty("RowHeight", 20);
             workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));//保存至fileName
             workbook->dynamicCall("Close()");//关闭工作簿
             excel->dynamicCall("Quit()");//关闭excel
             delete excel;
             excel = NULL;
         }
         else
             QMessageBox::warning(NULL, "错误","未能创建 Excel 对象，请安装 Microsoft Excel。", QMessageBox::Apply);
     }
}
