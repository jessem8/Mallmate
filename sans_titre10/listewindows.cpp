#include "listewindows.h"
#include "ui_listewindows.h"

ListeWindows::ListeWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListeWindows)
{
    ui->setupUi(this);
}

ListeWindows::~ListeWindows()
{
    delete ui;
}
