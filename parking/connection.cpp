#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Mallmate");//inserer le nom de la source de données
    db.setUserName("Mallmate");//inserer nom de l'utilisateur
    db.setPassword("Mallmate");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;



    return  test;
}
