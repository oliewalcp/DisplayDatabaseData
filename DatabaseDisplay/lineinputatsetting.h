#ifndef LINEINPUTATSETTING_H
#define LINEINPUTATSETTING_H
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include "applicationdata.h"
#include <QDialog>

namespace Ui {
class LineInputAtSetting;
}

class LineInputAtSetting : public QDialog
{
    Q_OBJECT

public:
    explicit LineInputAtSetting(QWidget *parent = 0);
    ~LineInputAtSetting();
    void SetNumber(int Number);//设置行号
    void SetColumnName(QString Column_Name);//设置列名
    void SetLinePropoties(int Number, QString Column_Name);//设置行属性
    QString GetInputString();//获取输入框的列名
    void AddTo(QVBoxLayout *parent);
    void RemoveFrom(QVBoxLayout *parent);
    void SetButtonText(QString text);//设置是否需要转换按钮文本
    void SetWebSite(QString webUrl);//设置网址
    void SetCanConnect(bool conn);//设置是否需要链接
    QString GetButtonText();//获取是否需要转换按钮文本
    QPushButton* getButtonObject();//获取是否需要转换按钮对象
    QString GetWebSite();//获取网址
    QString GetButtonSignal();//获取是否需要链接的文本
    int getNumber();//获取行号
private:
    Ui::LineInputAtSetting *ui;
    int LineNumber;
    QWidget *Panel;
    QLabel *ColumnNumber;//列号
    QLineEdit *ColumnNameInput;//列值输入
    QPushButton *NeedChange;//是否需要转换按钮
    QPushButton *ConnectWeb;//是否需要链接
    QLineEdit *WebSite;//网址
public slots:
    void MyClicked();
    void ConnectWebClicked();
};

#endif // LINEINPUTATSETTING_H
