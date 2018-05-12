#ifndef VICEFUNCTION_H
#define VICEFUNCTION_H

#include "applicationdata.h"
#include "sqleditframe.h"
#include "vicefunctionitem.h"
#include "viceresult.h"

namespace Ui {
class ViceFunction;
}

class ViceFunction : public QDialog
{
    Q_OBJECT

public:
    explicit ViceFunction(QWidget *parent = 0);
    ~ViceFunction();
    void InitComponent();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ViceFunction *ui;
    std::unordered_map<int, ViceFunctionItem*> content;
};

#endif // VICEFUNCTION_H
