#include "addlostitemdialog.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDate>
#include <QPlainTextEdit> // Include needed implementation detail

// Constructor Implementation
AddLostItemDialog::AddLostItemDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Report New Lost Item");
    setMinimumWidth(400);

    // Create Widgets
    descriptionTextEdit = new QPlainTextEdit(this);
    descriptionTextEdit->setPlaceholderText("Detailed description of the lost item (e.g., Brand, Color, Size, Identifying marks)...");
    descriptionTextEdit->setMinimumHeight(80);

    dateLostEdit = new QDateEdit(this);
    dateLostEdit->setDate(QDate::currentDate());
    dateLostEdit->setCalendarPopup(true);
    dateLostEdit->setDisplayFormat("yyyy-MM-dd");
    dateLostEdit->setMaximumDate(QDate::currentDate());

    locationLineEdit = new QLineEdit(this);
    locationLineEdit->setPlaceholderText("Last known location or general area");

    contactInfoLineEdit = new QLineEdit(this);
    contactInfoLineEdit->setPlaceholderText("Required: Email or Phone (for notification if found)");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // Layout
    formLayout = new QFormLayout(this); // Parent layout to dialog
    formLayout->addRow("Description:", descriptionTextEdit);
    formLayout->addRow("Date Lost (Approx):", dateLostEdit);
    formLayout->addRow("Last Known Location:", locationLineEdit);
    formLayout->addRow("Contact Info:", contactInfoLineEdit);
    formLayout->addRow(buttonBox);
    //setLayout(formLayout); // Not needed - layout is parented

    // Connections
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    Q_ASSERT(okButton != nullptr); // Should exist

    // Lambda for validation check
    auto updateOkButtonState = [=]() {
        bool descriptionOk = !descriptionTextEdit->toPlainText().trimmed().isEmpty();
        bool contactOk = !contactInfoLineEdit->text().trimmed().isEmpty();
        okButton->setEnabled(descriptionOk && contactOk);
    };

    connect(descriptionTextEdit, &QPlainTextEdit::textChanged, updateOkButtonState);
    connect(contactInfoLineEdit, &QLineEdit::textChanged, updateOkButtonState);

    updateOkButtonState(); // Set initial state
}

// Getter Implementations
QString AddLostItemDialog::description() const {
    return descriptionTextEdit ? descriptionTextEdit->toPlainText().trimmed() : QString();
}

QDate AddLostItemDialog::dateLost() const {
    return dateLostEdit ? dateLostEdit->date() : QDate();
}

QString AddLostItemDialog::location() const {
    return locationLineEdit ? locationLineEdit->text().trimmed() : QString();
}

QString AddLostItemDialog::contactInfo() const {
    return contactInfoLineEdit ? contactInfoLineEdit->text().trimmed() : QString();
}

// Optional: setData Implementation (if needed later)
// void AddLostItemDialog::setData(...) { ... }
