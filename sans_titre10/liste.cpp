#include "liste.h"
#include "ui_liste.h"

Liste::Liste(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Liste)
{
    ui->setupUi(this);
}

Liste::~Liste()
{
    delete ui;
}
