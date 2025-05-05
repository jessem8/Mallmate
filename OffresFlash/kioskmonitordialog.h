#ifndef KIOSKMONITORDIALOG_H
#define KIOSKMONITORDIALOG_H

#include <QDialog>  // Base class for dialog windows
#include <QString>  // For passing text data

// Forward declaration for the UI class generated from kioskmonitordialog.ui
QT_BEGIN_NAMESPACE
namespace Ui { class KioskMonitorDialog; }
QT_END_NAMESPACE

class KioskMonitorDialog : public QDialog
{
    Q_OBJECT // Required macro for classes with signals/slots

public:
    // Constructor
    explicit KioskMonitorDialog(QWidget *parent = nullptr);
    // Destructor
    ~KioskMonitorDialog();

public slots:
    // Slots to update the UI elements from outside (e.g., from OffersFlashScreen)
    void updateSerialStatus(const QString &statusMessage, bool connected);
    void updateDbStatus(const QString &statusMessage, bool connected);
    void updateLastUid(const QString &uid);
    void updatePatientInfo(const QString &name, const QString &line1, const QString &line2);
    void addLogMessage(const QString &message);
    void clearPatientInfo(); // Slot to clear fields when needed

private:
    // Pointer to the UI elements defined in the .ui file
    Ui::KioskMonitorDialog *ui;

}; // End of KioskMonitorDialog class declaration

#endif // KIOSKMONITORDIALOG_H
