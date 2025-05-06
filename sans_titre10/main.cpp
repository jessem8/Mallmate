#include "widget.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect();

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Connexion à la base de données"),
                                 QObject::tr("Connexion réussie ✅\n"
                                             "Cliquez sur OK pour continuer."), QMessageBox::Ok);

        Widget w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Connexion à la base de données"),
                              QObject::tr("Échec de la connexion ❌\n"
                                          "Cliquez sur OK pour quitter."), QMessageBox::Ok);
        return -1;
    }
}
