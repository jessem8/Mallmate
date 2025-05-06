#ifndef LISTEWINDOWS_H
#define LISTEWINDOWS_H

#include <QWidget>

namespace Ui {
class ListeWindows;
}

class ListeWindows : public QWidget
{
    Q_OBJECT

public:
    explicit ListeWindows(QWidget *parent = nullptr);
    ~ListeWindows();

private:
    Ui::ListeWindows *ui;
};

#endif // LISTEWINDOWS_H
