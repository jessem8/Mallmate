#ifndef PARKINGWINDOW_H
#define PARKINGWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

class ParkingWindow : public QWidget
{
    Q_OBJECT

public:
    ParkingWindow(QWidget *parent = nullptr);

private:
    QLabel *titleLabel;
    QLabel *totalPlacesLabel;
    QLabel *occupiedPlacesLabel;
    QLabel *availablePlacesLabel;
    QLabel *chargingStationsLabel;

    QLineEdit *plateInput;
    QLineEdit *entryTimeInput;
    QComboBox *vehicleTypeCombo;
    QPushButton *addButton;

    QLineEdit *searchInput;

    QPushButton *removeButton;
    QPushButton *historyButton;
    QPushButton *ticketButton;

    QWidget* createStatBox(const QString &icon, const QString &value, const QString &text);
};

#endif // PARKINGWINDOW_H
