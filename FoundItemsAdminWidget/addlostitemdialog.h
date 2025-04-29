#ifndef ADDLOSTITEMDIALOG_H
#define ADDLOSTITEMDIALOG_H

#include <QDialog>
#include <QDate>
#include <QString>

// Forward declarations
class QLineEdit;
class QDateEdit;
class QDialogButtonBox;
class QFormLayout;
class QPlainTextEdit;

class AddLostItemDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor Declaration
    explicit AddLostItemDialog(QWidget *parent = nullptr);

    // Getter Declarations
    QString description() const;
    QDate dateLost() const;
    QString location() const;
    QString contactInfo() const;

    // Optional: Setter Declaration (if needed for editing later)
    // void setData(const QString &description, const QDate &dateLost, const QString &location, const QString &contact);

private:
    // UI Element Pointers (Declarations)
    QPlainTextEdit *descriptionTextEdit;
    QDateEdit *dateLostEdit;
    QLineEdit *locationLineEdit;
    QLineEdit *contactInfoLineEdit;
    QDialogButtonBox *buttonBox;
    QFormLayout *formLayout;

    // Private helper slot/function declarations if needed
    // void updateOkButtonState(); // Example if made into a private slot/func
};

#endif // ADDLOSTITEMDIALOG_H
