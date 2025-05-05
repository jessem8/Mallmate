#ifndef SUGGESTIONDIALOG_H
#define SUGGESTIONDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>

// --- NEW: Include for QListWidgetItem ---
#include <QListWidgetItem> // <<< ADDED THIS INCLUDE

// Forward declarations
class QListWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;

/**
 * @brief A custom dialog to display a list of suggestions (e.g., from AI)
 * and allow the user to select one.
 */
class SuggestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SuggestionDialog(const QStringList &suggestions, QWidget *parent = nullptr);
    ~SuggestionDialog() override;

    QString getSelectedSuggestion() const;

private slots:
    // Parameter type QListWidgetItem* is now recognized
    void onItemDoubleClicked(QListWidgetItem *item);
    void acceptSelection();


private:
    QLabel      *m_promptLabel;
    QListWidget *m_listWidget;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_buttonLayout;
    QString m_selectedText;

    void setupUi(const QStringList &suggestions);
};

#endif // SUGGESTIONDIALOG_H
