#ifndef CONNECTDBFAILED_H
#define CONNECTDBFAILED_H

#include <QDialog>

namespace Ui {
class ConnectDBFailed;
}

class ConnectDBFailed : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDBFailed(QWidget *parent = 0);
    ~ConnectDBFailed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ConnectDBFailed *ui;
};

#endif // CONNECTDBFAILED_H
