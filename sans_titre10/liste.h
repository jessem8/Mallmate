#ifndef LISTE_H
#define LISTE_H

#include <QWidget>

namespace Ui {
class Liste;
}

class Liste : public QWidget
{
    Q_OBJECT

public:
    explicit Liste(QWidget *parent = nullptr);
    ~Liste();

private:
    Ui::Liste *ui;
};

#endif // LISTE_H
