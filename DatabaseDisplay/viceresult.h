#ifndef VICERESULT_H
#define VICERESULT_H

#include <QWidget>

namespace Ui {
class ViceResult;
}

class ViceResult : public QWidget
{
    Q_OBJECT

public:
    explicit ViceResult(QWidget *parent = 0);
    ~ViceResult();

    void SetLabel(QString str);
    void SetResult(QString str);

private:
    Ui::ViceResult *ui;
};

#endif // VICERESULT_H
