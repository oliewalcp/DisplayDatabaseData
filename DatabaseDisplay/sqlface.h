#ifndef SQLFACE_H
#define SQLFACE_H

#include <QDialog>
#include "applicationdata.h"
namespace Ui {
class SQLFace;
}

class SQLFace : public QDialog
{
    Q_OBJECT

public:
    explicit SQLFace(QWidget *parent = 0);
    ~SQLFace();

    void EnterSQLSettingFace();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SQLFace *ui;
};

#endif // SQLFACE_H
