#ifndef ASSISTANCE_H
#define ASSISTANCE_H

#include <QDialog>

namespace Ui {
class Assistance;
}

class Assistance : public QDialog
{
    Q_OBJECT

public:
    explicit Assistance(QWidget *parent = nullptr);
    ~Assistance();

private:
    Ui::Assistance *ui;
};

#endif // ASSISTANCE_H
