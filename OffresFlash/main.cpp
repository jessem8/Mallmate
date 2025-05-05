
#include "offersflashscreen.h" // Include the header for OffresFlashScreen

#include <QApplication>
#include <QIcon>
#include <QFile>       // Needed for loading the stylesheet
#include <QTextStream> // Needed for reading the stylesheet file
#include <QDebug>      // Needed for qWarning/qDebug output

int main(int argc, char *argv[])
{
    // 1. Create the Qt Application instance
    QApplication a(argc, argv);

    // --- Load the global stylesheet ---
    // Make sure the path ":/styles/style.qss" exactly matches
    // the prefix and filename in your resources.qrc file.
    QFile styleFile(":/style/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        QString styleSheet = stream.readAll();
        a.setStyleSheet(styleSheet); // Apply the stylesheet to the entire application
        styleFile.close();
        qDebug() << "Stylesheet loaded successfully from :/styles/style.qss";
    } else {
        qWarning() << "Could not find or open stylesheet file: :/styles/style.qss - Check resources.qrc and rebuild.";
        // You might want to add more robust error handling here if the style is critical
    }
    // --- Stylesheet loaded ---

    // 2. Create an instance of OffresFlashScreen
    OffresFlashScreen offersWindow; // Create an instance of OffresFlashScreen
    // Ensure the logo path in resources.qrc is correct (e.g., under /images prefix)
    offersWindow.setWindowIcon(QIcon(":/images/mallmate_logo.ico"));

    // 3. Show the OffresFlashScreen widget
    offersWindow.show(); // Show the OffresFlashScreen

    // 4. Enter the Qt event loop
    return a.exec();
}
