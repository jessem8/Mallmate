#ifndef PARKING_H
#define PARKING_H

#include <QWidget>
#include <QSqlQuery>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QSerialPort>           // Ajout pour la communication série
#include <QSerialPortInfo>       // Ajout pour la détection des ports
#include <QJsonDocument>         // Ajout pour le traitement JSON
#include <QJsonObject>           // Ajout pour le traitement JSON
// Forward declarations
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QStackedWidget;
class QTableWidget;
class QTableWidgetItem;
class QHBoxLayout;

class ParkingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingWindow(QWidget *parent = nullptr);
    ~ParkingWindow();

    void generateParkingTicket();

private slots:
    void goToHomePage();
    void onAddButtonClicked();
    void onViewButtonClicked(int row);
    void onModifyButtonClicked(int row);
    void onDeleteButtonClicked(int row);
    void afficherHistorique();
    void showModifyForm(int row);
    void sortHistory(const QString &sortOption);
    void exportToPDF();
    void exportHistoryToCSV();
    void updateStats();
    void setupCharts();
    void checkParkingStatus();
    void showNotification(const QString &title, const QString &message);

    // Nouveaux slots pour la communication série
    void readSerialData();
    void processSerialData(const QByteArray &data);
    void handleSerialError(QSerialPort::SerialPortError error);
    // In parking.h
    void updateParkingDisplay(int available, float sensor1, float sensor2, float sensor3);
    void updateDatabase(int availableSpots); // Declaration

private:
    // UI Widgets
    QLabel *titleLabel;
    QLineEdit *plateInput;
    QLineEdit *entryTimeInput;
    QComboBox *vehicleTypeCombo;
    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QLineEdit *numeroInput;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *historyButton;
    QPushButton *ticketButton;
    QPushButton *notificationButton;
    QComboBox *sortComboBox;
    QPushButton *sortButton;
    QString arduino_port_name;
    // Éléments pour les graphiques
    QChartView *chartView;
    QLineSeries *occupiedSeries;
    QLineSeries *electricSeries;
    QBarSeries *availableSeries;
    QChart *chart;
    QLineEdit *searchLineEdit;
    QComboBox *searchFieldCombo;
    bool isParkingFull();

    void showParkingSpotStatus();
    void onParkingSpotClicked();
    QLabel* createLegendItem(const QString &text, const QString &color);
    void filterHistory();

    // Tableau d'historique
    QTableWidget *historyTable;
    static const int ACTIONS_COLUMN_INDEX = 5;

    // Navigation
    QStackedWidget *stackedWidget;

    // Système de notifications
    QSystemTrayIcon *trayIcon;
    QTimer *notificationTimer;
    int lastOccupiedCount;
    bool isParkingFullNotified;

    // Communication série avec Arduino
    QSerialPort *arduino;
    QByteArray serialBuffer;
    static const quint16 arduino_vendor_id = 6790;  // VID Arduino
    static const quint16 arduino_product_id = 29987;   // PID Arduino Uno
    bool arduino_is_available;

    // Méthodes utilitaires
    void setupHistoryRow(int row, const QString &placeId, const QString &numero,
                         const QString &statut, const QString &localisation);
    void setupHistoryRowWidgets(int row, const QString &placeId, const QString &numero,
                                const QString &statut, const QString &localisation);
    QWidget* createStatBox(const QString &icon, const QString &value, const QString &text);
    bool supprimerPlaceDeLaBase(const QString &placeId);
    QVector<QPointF> getHourlyData(const QString &status);
    int getCurrentCount(const QString &status);
    void setupNotificationSystem();

    // Nouvelles méthodes pour Arduino
    void setupSerialConnection();
    void sendCommandToArduino(const QString &command);
    void addParkingEntryToDatabase();
};

#endif // PARKING_H
