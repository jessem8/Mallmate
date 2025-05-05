#include "kioskmonitordialog.h" // Include the header for this class
#include "ui_kioskmonitordialog.h" // Include the header generated from the .ui file
#include <QDateTime>             // For timestamping log messages

// =========================================================================
// CONSTRUCTOR
// =========================================================================
KioskMonitorDialog::KioskMonitorDialog(QWidget *parent) :
    QDialog(parent), // Call the base class constructor
    ui(new Ui::KioskMonitorDialog) // Create the UI object
{
    // Setup the user interface defined in kioskmonitordialog.ui
    ui->setupUi(this);

    // Connect the standard dialog buttons (e.g., Close)
    // This connection is usually already made in the .ui file's <connections> section
    // connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &KioskMonitorDialog::accept);
    // connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &KioskMonitorDialog::reject);

    // Set initial states for labels (optional, can also be set when shown)
    clearPatientInfo(); // Clear info fields initially
    ui->serialStatusLabel->setText("Indéterminé");
    ui->dbStatusLabel->setText("Indéterminé");
    ui->logTextEdit->append(QDateTime::currentDateTime().toString("HH:mm:ss") + " - Moniteur initialisé.");
}

// =========================================================================
// DESTRUCTOR
// =========================================================================
KioskMonitorDialog::~KioskMonitorDialog()
{
    // Delete the UI object to free memory
    delete ui;
}

// =========================================================================
// PUBLIC SLOTS (Implementation)
// =========================================================================

// Slot to update the serial connection status label
void KioskMonitorDialog::updateSerialStatus(const QString &statusMessage, bool connected)
{
    ui->serialStatusLabel->setText(statusMessage);
    // Set color based on connection status
    if (connected) {
        ui->serialStatusLabel->setStyleSheet("color: green;");
    } else {
        // Use different colors for different non-connected states if needed
        if (statusMessage.contains("Erreur", Qt::CaseInsensitive) || statusMessage.contains("Absent", Qt::CaseInsensitive)) {
            ui->serialStatusLabel->setStyleSheet("color: red;");
        } else if (statusMessage.contains("Non Trouve", Qt::CaseInsensitive)) {
            ui->serialStatusLabel->setStyleSheet("color: orange;");
        } else {
            ui->serialStatusLabel->setStyleSheet("color: grey;"); // Default disconnected color
        }
    }
}

// Slot to update the database connection status label
void KioskMonitorDialog::updateDbStatus(const QString &statusMessage, bool connected)
{
    ui->dbStatusLabel->setText(statusMessage);
    // Set color based on connection status
    if (connected) {
        ui->dbStatusLabel->setStyleSheet("color: green;");
    } else {
        if (statusMessage.contains("Erreur", Qt::CaseInsensitive) || statusMessage.contains("Absent", Qt::CaseInsensitive)) {
            ui->dbStatusLabel->setStyleSheet("color: red;");
        } else {
            ui->dbStatusLabel->setStyleSheet("color: grey;"); // Default disconnected color
        }
    }
}

// Slot to update the label showing the last received UID
void KioskMonitorDialog::updateLastUid(const QString &uid)
{
    ui->lastUidLabel->setText(uid);
}

// Slot to update the labels showing patient/result information
void KioskMonitorDialog::updatePatientInfo(const QString &name, const QString &line1, const QString &line2)
{
    ui->patientNameLabel->setText(name);
    ui->message1Label->setText(line1);
    ui->message2Label->setText(line2);
}

// Slot to add a message to the event log QTextEdit
void KioskMonitorDialog::addLogMessage(const QString &message)
{
    // Add timestamp for clarity
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->logTextEdit->append(timestamp + " - " + message);
}

// Slot to clear the patient information fields
void KioskMonitorDialog::clearPatientInfo()
{
    ui->lastUidLabel->setText("N/A");
    ui->patientNameLabel->setText("N/A");
    ui->message1Label->setText("N/A");
    ui->message2Label->setText("N/A");
}

