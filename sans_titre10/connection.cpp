#include "connection.h"
#include <QSqlError> // Add this for QSqlError
Connection::Connection()
{

}

bool Connection::createconnect()
{
    // Ajout du pilote et configuration de la base de données
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={Oracle in XE};Dbq=//localhost:1521/XE;Uid=Mallmate;Pwd=Mallmate;");

    // Vérification de l'ouverture de la connexion
    if (!db.open()) {
        // Journalisation de l'erreur avec plus de détails
        qWarning() << "Erreur de connexion :"
                   << db.lastError().driverText()
                   << db.lastError().databaseText();
        return false; // Échec
    }

    qInfo() << "Connexion réussie à la base de données :" << db.databaseName();
    return true; // Succès
}
