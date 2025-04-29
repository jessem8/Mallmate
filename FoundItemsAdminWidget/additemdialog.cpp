#include "additemdialog.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDate>
#include <QPropertyAnimation> // <<< Include animation header
#include <QShowEvent>        // <<< Include show event header

AddItemDialog::AddItemDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add New Found Item");

    // --- Start with window transparent ---
    setWindowOpacity(0.0);

    // Create Widgets (same as before)
    descriptionLineEdit = new QLineEdit(this);
    dateFoundEdit = new QDateEdit(this); dateFoundEdit->setDate(QDate::currentDate()); dateFoundEdit->setCalendarPopup(true); dateFoundEdit->setDisplayFormat("yyyy-MM-dd");
    locationLineEdit = new QLineEdit(this);
    statusComboBox = new QComboBox(this); statusComboBox->addItem("Unclaimed"); statusComboBox->addItem("Claimed"); statusComboBox->addItem("Returned"); statusComboBox->addItem("Disposed"); statusComboBox->setCurrentText("Unclaimed");
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // Layout (same as before)
    formLayout = new QFormLayout(this);
    formLayout->addRow("Description:", descriptionLineEdit);
    formLayout->addRow("Date Found:", dateFoundEdit);
    formLayout->addRow("Location Found:", locationLineEdit);
    formLayout->addRow("Status:", statusComboBox);
    formLayout->addRow(buttonBox);
    setLayout(formLayout);

    // Connections (same as before)
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    descriptionLineEdit->setPlaceholderText("Required");
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(!descriptionLineEdit->text().isEmpty());
    connect(descriptionLineEdit, &QLineEdit::textChanged, [okButton](const QString &text){ okButton->setEnabled(!text.isEmpty()); });

    setMinimumWidth(350);

    // --- Setup Fade-In Animation ---
    fade_in_animation = new QPropertyAnimation(this, "windowOpacity", this); // Target this dialog's opacity
    fade_in_animation->setDuration(300); // Animation duration in milliseconds
    fade_in_animation->setStartValue(0.0); // Start fully transparent
    fade_in_animation->setEndValue(1.0);   // End fully opaque
    fade_in_animation->setEasingCurve(QEasingCurve::InOutQuad); // Make it smooth (optional)

}

// --- Method to set data when editing (same as before) ---
void AddItemDialog::setData(const QString &description, const QDate &dateFound, const QString &location, const QString &status)
{
    setWindowTitle("Edit Found Item");
    descriptionLineEdit->setText(description);
    dateFoundEdit->setDate(dateFound);
    locationLineEdit->setText(location);
    statusComboBox->setCurrentText(status);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(!descriptionLineEdit->text().isEmpty());
}

// --- Override showEvent to start animation ---
void AddItemDialog::showEvent(QShowEvent *event)
{
    // Call base class implementation first
    QDialog::showEvent(event);

    // Start the fade-in animation when the dialog is shown
    if (fade_in_animation) { // Check if animation object exists
        fade_in_animation->start();
    }
}


// --- Getter Implementations (same as before) ---
QString AddItemDialog::description() const { return descriptionLineEdit->text().trimmed(); }
QDate AddItemDialog::dateFound() const { return dateFoundEdit->date(); }
QString AddItemDialog::location() const { return locationLineEdit->text().trimmed(); }
QString AddItemDialog::status() const { return statusComboBox->currentText(); }
