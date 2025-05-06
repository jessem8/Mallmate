#include "assistance.h"
#include "ui_assistance.h"

Assistance::Assistance(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Assistance)
{
    ui->setupUi(this);
}

Assistance::~Assistance()
{
    delete ui;
}
