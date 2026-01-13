#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include "../common/configmanager.h"

Connection::Connection()
{
}

bool Connection::createconnect()
{
    // --- Load credentials from ConfigManager (env vars / QSettings) ---
    if (!ConfigManager::isDatabaseConfigured()) {
        qWarning() << "Database configuration is missing!";
        qWarning() << ConfigManager::getMissingConfigMessage();
        return false;
    }

    QString dsn = ConfigManager::getDatabaseDsn();
    QString user = ConfigManager::getDatabaseUser();
    QString pass = ConfigManager::getDatabasePassword();

    // Build connection string
    QString connectionString;
    if (dsn.startsWith("Driver=", Qt::CaseInsensitive) || dsn.contains(";")) {
        // DSN is already a full connection string
        connectionString = dsn;
        if (!connectionString.contains("Uid=", Qt::CaseInsensitive)) {
            connectionString += QString(";Uid=%1").arg(user);
        }
        if (!connectionString.contains("Pwd=", Qt::CaseInsensitive)) {
            connectionString += QString(";Pwd=%1").arg(pass);
        }
    } else {
        // DSN is a simple name, build Oracle connection string
        connectionString = QString("Driver={Oracle in XE};Dbq=//%1;Uid=%2;Pwd=%3;")
                               .arg(dsn).arg(user).arg(pass);
    }

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qWarning() << "Erreur de connexion:"
                   << db.lastError().driverText()
                   << db.lastError().databaseText();
        qWarning() << "Please check your database configuration (see CONFIG.md).";
        return false;
    }

    qInfo() << "Connexion réussie à la base de données.";
    return true;
}
