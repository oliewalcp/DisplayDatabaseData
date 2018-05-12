#ifndef VICEFUNCTIONITEM_H
#define VICEFUNCTIONITEM_H

#include <QWidget>
#include "sqleditframe.h"

namespace Ui {
class ViceFunctionItem;
}

class ViceFunctionItem : public QWidget
{
    Q_OBJECT

public:
    explicit ViceFunctionItem(QWidget *parent = 0);
    ~ViceFunctionItem();
    QString getTitle();
    int getTime();
    QString getSQLCode();
    void setSQLCode(QString sql);
    void setTime(QString time);
    void setTitle(QString title);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ViceFunctionItem *ui;
    SQLEditFrame *editFrame;
};

#endif // VICEFUNCTIONITEM_H
