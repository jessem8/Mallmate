#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QtSql/QSqlDatabase> // Include necessary headers
#include <QtSql/QSqlError>
#include <QString>
#include <QDebug>
#include <QMessageBox> // To show potential errors easily

class DatabaseConnection
{
public:
    // Static function to establish the default database connection.
    // Returns true on success, false on failure.
    // Default parameters match your requirements but can be overridden if needed.
    static bool connect(const QString& dsnName = "Mallmate", // <<< DSN name as requested
                        const QString& user = "Mallmate",
                        const QString& pass = "Mallmate",
                        const QString& driver = "QODBC") // Defaulting to ODBC
    {
        // Check if the default connection already exists and is open
        QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false); // Don't add if exists
        if (db.isValid() && db.isOpen()) {
            qDebug() << "Database connection already exists and is open.";
            return true;
        }

        // Add the database connection (uses default connection name implicitly)
        db = QSqlDatabase::addDatabase(driver);
        qDebug() << "Attempting to connect via" << driver << "...";
        qDebug() << "Available Qt SQL drivers:" << QSqlDatabase::drivers();
        qDebug() << "Is" << driver << "driver available?" << QSqlDatabase::isDriverAvailable(driver);

        if (!QSqlDatabase::isDriverAvailable(driver)) {
            qWarning() << driver << "driver not available!";
            QMessageBox::critical(nullptr, "Database Driver Error",
                                  QString("The required database driver (%1) is not available.\n"
                                          "Please check your Qt installation and environment setup.\n"
                                          "Available drivers: %2")
                                      .arg(driver, QSqlDatabase::drivers().join(", ")));
            return false;
        }

        // Set connection parameters using DSN for ODBC
        db.setDatabaseName(dsnName); // Use the DSN name
        db.setUserName(user);
        db.setPassword(pass);

        // Attempt to open the connection
        if (!db.open()) {
            qWarning() << driver << "Database connection failed for DSN:" << dsnName;
            qWarning() << "Qt Error:" << db.lastError().text();
            qWarning() << "Database Error:" << db.lastError().databaseText(); // Specific DB error
            qWarning() << "Driver Error:" << db.lastError().driverText(); // Driver specific error

            QMessageBox::critical(nullptr, "Database Connection Error",
                                  QString("Failed to connect to the database via %1.\n"
                                          "DSN: %2\n"
                                          "Error: %3\n\n"
                                          "Please check ODBC configuration, Oracle client setup, and database status.")
                                      .arg(driver, dsnName, db.lastError().databaseText().isEmpty() ? db.lastError().text() : db.lastError().databaseText()));
            return false;
        }

        qDebug() << driver << "connection successful for DSN:" << dsnName;
        return true;
    }

    // Optional: Static function to explicitly close the connection if needed
    static void close() {
        if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
            QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
            if (db.isOpen()) {
                db.close();
                qDebug() << "Default database connection closed.";
            }
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            qDebug() << "Default database connection removed.";
        }
    }
};

#endif // DATABASECONNECTION_H
