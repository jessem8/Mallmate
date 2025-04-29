#include <QApplication>
#include <QString>
#include <QFile>
#include <QMessageBox> // Include for potential error messages
#include <QDebug>      // Include for qDebug output

// --- Qt SQL Includes ---
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant> // Might be needed indirectly by QSql

#include "founditemsadminwidget.h"
// #include "databaseconnection.h" // <<< REMOVED: No longer using the separate class

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("Mallmate");
    a.setOrganizationName("YourOrganization"); // Keep or change as needed
    a.setApplicationVersion("1.0.0");

    // --- Establish Database Connection (ODBC Connection String Method) ---
    qDebug() << "[main] Attempting database connection via ODBC Connection String...";

    QString connectionName = QSqlDatabase::defaultConnection; // Use the default connection name

    // Check if a connection with this name already exists (e.g., from a previous run if not cleaned up)
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase existingDb = QSqlDatabase::database(connectionName);
        if (existingDb.isOpen()) {
            qDebug() << "[main] Warning: Connection '" << connectionName << "' already exists and is open. Closing before creating new one.";
            existingDb.close();
        }
        qDebug() << "[main] Removing existing (closed) connection '" << connectionName << "'.";
        QSqlDatabase::removeDatabase(connectionName);
    }

    // Add the database driver
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", connectionName); // Specify the connection name
    qDebug() << "[main] Added QODBC driver for connection '" << connectionName << "'.";

    // --- Define Connection String Parameters ---
    // IMPORTANT: Use the correct values for YOUR Oracle XE setup
    QString driverName = "{Oracle in XE}"; // Driver name as identified in ODBC Data Sources (x64)
    QString tnsServiceName = "XE";          // Your TNS Service Name or SID for Oracle XE
    QString user = "Mallmate";             // Your Oracle Database Username
    QString pass = "Mallmate";             // Your Oracle Database Password (use the correct one)
    // Note: The user/pass "Mallmate" from the original main.cpp seems different from MainWindow.cpp's "DRIBBTECH".
    // I've used "DRIBBTECH" here based on the MainWindow code you provided. Adjust if needed.

    // Construct the connection string
    QString connectionString = QString("DRIVER=%1;DBQ=%2;UID=%3;PWD=%4;")
                                   .arg(driverName)
                                   .arg(tnsServiceName)
                                   .arg(user)
                                   .arg(pass);

    qDebug() << "[main]  Conn Str (PWD omitted):" << QString("DRIVER=%1;DBQ=%2;UID=%3;PWD=*****;").arg(driverName).arg(tnsServiceName).arg(user);

    // Set the connection string as the "database name" for the QODBC driver
    db.setDatabaseName(connectionString);

    qDebug() << "[main] Attempting db.open() using Connection String...";
    bool ok = db.open(); // Try to open the connection

    // Check if the connection was successful
    if (!ok) {
        qCritical() << "[main] FATAL: Database connection failed using Connection String.";
        qCritical() << "[main]  QSqlError:" << db.lastError().text();
        qCritical() << "[main]  ODBC Driver Error:" << db.lastError().driverText();
        qCritical() << "[main]  Oracle Database Error:" << db.lastError().databaseText();

        // Show a detailed error message to the user
        QMessageBox::critical(nullptr, "Fatal Database Error",
                              QString("Could not connect to the database using ODBC.\n\n"
                                      "Driver: %1\n"
                                      "Service/DBQ: %2\n"
                                      "User: %3\n\n"
                                      "Error: %4\n\n"
                                      "Please check:\n"
                                      "1. Oracle ODBC driver (64-bit if app is 64-bit) is installed.\n"
                                      "2. Connection details (Driver, DBQ, UID, PWD) are correct in main.cpp.\n"
                                      "3. Oracle Listener service is running and database is accessible.\n\n"
                                      "Application will exit.")
                                  .arg(driverName)
                                  .arg(tnsServiceName)
                                  .arg(user)
                                  .arg(db.lastError().text()));
        return 1; // Exit application if connection fails
    }

    qDebug() << "[main] Database connection successful using Connection String.";
    // --- End Database Connection ---


    // --- QSS Styling (Unchanged from your original main.cpp) ---
    QString qss = R"(

        /* Global Window Background */
        QWidget {
            background-color: #F5F7FA;
            font-family: Segoe UI, Arial, sans-serif;
            color: #333333;
        }

        /* Sidebar Container Styling */
        #sidebarWidget {
            background-color: #FFFFFF;
            min-width: 200px;
            max-width: 250px;
        }

        /* Sidebar List Widget */
        #sidebarListWidget {
            background-color: transparent;
            border: none;
        }
        #sidebarListWidget::item {
            padding: 12px 20px; color: #555555; min-height: 30px;
        }
        #sidebarListWidget::item:selected {
            background-color: #E3F2FD; color: #0D47A1;
            border-left: 3px solid #0D47A1; font-weight: bold; padding-left: 17px;
        }
         #sidebarListWidget::item:hover:!selected { background-color: #f0f0f0; }

        /* General Button Styling */
        QPushButton {
            background-color: #F0F0F0; color: #333333; border: 1px solid #CCCCCC;
            padding: 5px 12px; border-radius: 4px; min-height: 28px; text-align: center;
        }
        QPushButton:hover { background-color: #E0E0E0; }
        QPushButton:pressed { background-color: #D0D0D0; }
        QPushButton:disabled { background-color: #F5F5F5; color: #AAAAAA; border-color: #DDDDDD; }

        /* Primary Button Styling (#addFoundItemButton) */
        #addFoundItemButton {
            background-color: #4A90E2; color: white; padding: 8px 16px;
            border: none; font-weight: bold; min-height: 30px; icon-size: 16px;
        }
        #addFoundItemButton:hover { background-color: #357ABD; }
        #addFoundItemButton:pressed { background-color: #2A5E8E; }

        /* Icon-only Button (#editViewButton) */
        #editViewButton {
            background-color: transparent; border: none; padding: 4px;
            min-height: unset; min-width: unset;
        }
        #editViewButton:hover { background-color: #e0e0e0; border-radius: 4px; }
        #editViewButton:pressed { background-color: #d0d0d0; border-radius: 4px; }
        #editViewButton:disabled { background-color: transparent; border: none; }

        /* Search Line Edit */
        #searchLineEdit {
            border: 1px solid #D0D0D0; border-radius: 4px;
            padding: 6px 10px 6px 28px; /* Left padding for icon */
            background-color: white; min-height: 28px;
            background-image: url(:/icons/search.png); /* Ensure this resource exists */
            background-repeat: no-repeat; background-position: left 8px center;
            background-origin: padding-box;
        }
        #searchLineEdit:focus { border-color: #4A90E2; background-image: url(:/icons/search.png); }
        #searchLineEdit::clear-button { margin-right: 4px; }

        /* Table View Styling (General) */
        QTableView {
            border: 1px solid #E0E0E0; gridline-color: #E5E5E5; background-color: white;
            alternate-background-color: #F9F9F9; selection-background-color: #A6D7FF;
            selection-color: #111111;
        }
        QTableView::item { padding: 8px 10px; border-bottom: 1px solid #EEEEEE; }
        QTableView::item:selected { border-bottom: 1px solid #A6D7FF; }
        QTableView::item:hover:!selected { background-color: #F0F8FF; }

        /* Table Header Styling */
        QHeaderView::section {
            background-color: #F5F7FA; padding: 8px; border: none;
            border-bottom: 1px solid #D0D0D0; font-weight: bold;
            color: #444444; min-height: 24px; text-align: left;
        }

        /* Labels (General) */
        QLabel { background-color: transparent; color: #333333; padding: 1px; }

        /* Stats Cards Styling */
        #statsCard { background-color: #FFFFFF; border-radius: 8px; border: 1px solid #E0E0E0; }
        #statsCard QLabel { background-color: transparent; color: #555555; padding: 0px; }
        #statsCard QLabel#statsValueLabel { color: #1A237E; font-size: 20pt; font-weight: bold; margin-top: 5px; }
        #statsCard QLabel#statsTitleLabel { color: #757575; font-size: 9pt; font-weight: normal; }

        /* Empty Table Label Styling */
        #emptyTableLabel { color: #757575; font-style: italic; padding: 50px; background-color: white; border: none; }

        /* Sidebar Buttons Styling */
        #sidebarWidget QPushButton {
            text-align: left; padding-left: 20px; background-color: transparent;
            border: none; color: #444444; min-height: 35px; icon-size: 16px;
        }
        #sidebarWidget QPushButton:hover { background-color: #f0f0f0; color: #111111; }
        #sidebarWidget QPushButton:pressed { background-color: #e0e0e0; }

        /* Date Navigation Buttons Styling */
        #itemsManagementWidget QPushButton#prevDateButton,
        #itemsManagementWidget QPushButton#nextDateButton {
             min-width: 30px; max-width: 30px; padding: 5px; font-weight: bold;
             background-color: #E8EAF6; border-color: #C5CAE9;
        }
        #itemsManagementWidget QPushButton#prevDateButton:hover,
        #itemsManagementWidget QPushButton#nextDateButton:hover { background-color: #D1D9F7; }
        #itemsManagementWidget QPushButton#prevDateButton:pressed,
        #itemsManagementWidget QPushButton#nextDateButton:pressed { background-color: #B3BCF5; }
        #itemsManagementWidget QLabel#dateLabel { font-weight: bold; color: #555; padding: 0 10px; }

        /* Lost Items Page Specific Styles */
        #lostItemsPage QLabel#lostItemsTitle {
             /* Add specific title styles here if needed */
        }
        #lostItemsPage QPushButton#addLostReportButton {
             background-color: #81C784; /* Greenish */
             color: white;
             border: none;
             font-weight: bold;
             min-height: 30px;
             icon-size: 16px;
        }
         #lostItemsPage QPushButton#addLostReportButton:hover { background-color: #66BB6A; }
         #lostItemsPage QPushButton#addLostReportButton:pressed { background-color: #4CAF50; }

         #lostItemsPage QPushButton#deleteLostItemButton {
             background-color: #E57373; /* Reddish */
             color: white;
             border: none;
             font-weight: bold;
             min-height: 30px;
             icon-size: 16px;
         }
         #lostItemsPage QPushButton#deleteLostItemButton:hover { background-color: #EF5350; }
         #lostItemsPage QPushButton#deleteLostItemButton:pressed { background-color: #F44336; }
         #lostItemsPage QPushButton#deleteLostItemButton:disabled {
             background-color: #FFCDD2; color: #E57373; border: none;
         }

    )"; // End of raw string literal

    a.setStyleSheet(qss);

    // Create the main window AFTER attempting DB connection
    FoundItemsAdminWidget w; // Assuming this widget now uses QSqlDatabase::database() internally
    w.setWindowTitle("Mallmate - Lost & Found Management");
    w.resize(1200, 800);
    w.show();

    // Optional: Connect the application's aboutToQuit signal to close the database connection
    // This helps ensure resources are released cleanly.
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        QString connNameToClose = QSqlDatabase::defaultConnection;
        if (QSqlDatabase::contains(connNameToClose)) {
            QSqlDatabase dbToClose = QSqlDatabase::database(connNameToClose, false); // Get connection without adding if missing
            if (dbToClose.isOpen()) {
                dbToClose.close();
                qDebug() << "[main::aboutToQuit] Database connection '" << connNameToClose << "' closed.";
            }
            // Optionally remove the connection definition as well, though closing is usually sufficient
            // QSqlDatabase::removeDatabase(connNameToClose);
            // qDebug() << "[main::aboutToQuit] Database connection '" << connNameToClose << "' removed.";
        }
    });

    return a.exec();
}
