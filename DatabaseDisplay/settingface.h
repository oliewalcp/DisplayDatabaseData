#ifndef SETTINGFACE_H
#define SETTINGFACE_H

#include <QSystemTrayIcon>
#include <QTextCodec>
#include <QValidator>
#include <QFileDialog>
#include "applicationdata.h"
#include "lineinputatsetting.h"

namespace Ui {
class SettingFace;
}

class SettingFace : public QDialog
{
    Q_OBJECT

public:
    explicit SettingFace(QWidget *parent = 0);
    ~SettingFace();

    void SetInitData();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    QWidget *Application;//主窗口
    QVBoxLayout *ColumnAreaParent;
    Ui::SettingFace *ui;
    std::unordered_map<int, LineInputAtSetting*> LineList;
};

#endif // SETTINGFACE_H
