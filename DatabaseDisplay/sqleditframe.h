#ifndef SQLEDITFRAME_H
#define SQLEDITFRAME_H

#include <QDialog>
#include <QLabel>
#include <QString>

namespace Ui {
class SQLEditFrame;
}

class SQLEditFrame : public QDialog
{
    Q_OBJECT

public:
    explicit SQLEditFrame(QWidget *parent = 0);
    ~SQLEditFrame();
    void EnterEdit(QLabel *label);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SQLEditFrame *ui;

    QLabel *label;
};

#endif // SQLEDITFRAME_H
