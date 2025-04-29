#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QDate>
#include <QString>
#include <QEasingCurve> // <<< Include for easing curve enum

// Forward declarations
class QLineEdit;
class QDateEdit;
class QComboBox;
class QDialogButtonBox;
class QFormLayout;
class QPropertyAnimation; // <<< Forward declare animation class
class QShowEvent;        // <<< Forward declare show event class

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = nullptr);

    void setData(const QString &description, const QDate &dateFound, const QString &location, const QString &status);

    QString description() const;
    QDate dateFound() const;
    QString location() const;
    QString status() const;

protected:
    // Override showEvent to trigger animation
    void showEvent(QShowEvent *event) override; // <<< Override declaration

private:
    // UI Elements
    QLineEdit *descriptionLineEdit;
    QDateEdit *dateFoundEdit;
    QLineEdit *locationLineEdit;
    QComboBox *statusComboBox;
    QDialogButtonBox *buttonBox;

    // Layout
    QFormLayout *formLayout;

    // Animation Member
    QPropertyAnimation *fade_in_animation; // <<< Animation object pointer
};

#endif // ADDITEMDIALOG_H
