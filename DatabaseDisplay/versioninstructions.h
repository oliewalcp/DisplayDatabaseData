#ifndef VERSIONINSTRUCTIONS_H
#define VERSIONINSTRUCTIONS_H

#include <QDialog>

namespace Ui {
class VersionInstructions;
}

class VersionInstructions : public QDialog
{
    Q_OBJECT

public:
    explicit VersionInstructions(QWidget *parent = 0);
    ~VersionInstructions();

private slots:
    void on_pushButton_clicked();

private:
    Ui::VersionInstructions *ui;
};

#endif // VERSIONINSTRUCTIONS_H
