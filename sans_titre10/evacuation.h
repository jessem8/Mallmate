#ifndef EVACUATION_H
#define EVACUATION_H
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>

namespace Ui {
class Evacuation;
}

class Evacuation : public QDialog
{
    Q_OBJECT

public:
    explicit Evacuation(QWidget *parent = nullptr);
    ~Evacuation();

private slots:
    void onAddStepClicked();
    void undoStep();

private:
    Ui::Evacuation *ui;
    QVBoxLayout *stepLayout;
    QList<QLabel*> stepsList;


};

#endif // EVACUATION_H
