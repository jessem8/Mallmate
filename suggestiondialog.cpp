#include "suggestiondialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QApplication> // For palette access
#include <QDebug>       // For debug output
#include <QSizePolicy> // Keep include if needed elsewhere, but not used here now

SuggestionDialog::SuggestionDialog(const QStringList &suggestions, QWidget *parent) :
    QDialog(parent),
    m_promptLabel(nullptr),
    m_listWidget(nullptr),
    m_okButton(nullptr),
    m_cancelButton(nullptr),
    m_mainLayout(nullptr),
    m_buttonLayout(nullptr),
    m_selectedText("")
{
    setupUi(suggestions); // Call the UI setup function

    // Set window properties
    setWindowTitle("Suggestions de Description (IA)");
    setMinimumWidth(400); // Reset minimum width or adjust as needed
    setModal(true);

    // Apply consistent background color using QApplication palette
    setStyleSheet(QString("QDialog { background-color: %1; }")
                      .arg(parent ? parent->palette().window().color().name() : QApplication::palette().window().color().name()));
}

SuggestionDialog::~SuggestionDialog()
{
    // Qt handles child widget deletion automatically due to parent assignment
}

/**
 * @brief Sets up the user interface elements for the SuggestionDialog.
 * @param suggestions A list of suggestion strings to display.
 */
void SuggestionDialog::setupUi(const QStringList &suggestions)
{
    // --- Create Widgets ---
    m_promptLabel = new QLabel("Choisissez une suggestion ou annulez:", this);

    m_listWidget = new QListWidget(this);
    m_listWidget->addItems(suggestions);
    m_listWidget->setWordWrap(true);
    m_listWidget->setStyleSheet("QListWidget { border: 1px solid #cccccc; background-color: white; padding: 5px; }");
    if (!suggestions.isEmpty()) {
        m_listWidget->setCurrentRow(0);
    }

    // --- Create Buttons ---
    m_okButton = new QPushButton(this); // Text set later
    m_cancelButton = new QPushButton("Annuler", this);

    // --- Create Layouts ---
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->addStretch(); // Stretch pushes buttons to the right
    m_buttonLayout->addWidget(m_okButton);
    m_buttonLayout->addWidget(m_cancelButton);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_promptLabel);
    m_mainLayout->addWidget(m_listWidget, 1); // List widget stretches vertically
    m_mainLayout->addLayout(m_buttonLayout);

    setLayout(m_mainLayout);

    // --- Set Button Properties AFTER Layout ---
    m_okButton->setText("Sélectionner"); // Use shorter text
    m_okButton->setDefault(true);

    // REMOVED setMinimumWidth and setSizePolicy - letting stylesheet handle it


    // --- Connect Signals and Slots ---
    connect(m_okButton, &QPushButton::clicked, this, &SuggestionDialog::acceptSelection);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &SuggestionDialog::onItemDoubleClicked);

    qDebug() << "SuggestionDialog::setupUi completed. OK button text set to:" << m_okButton->text();
}

// ... (rest of the functions: getSelectedSuggestion, onItemDoubleClicked, acceptSelection) ...

QString SuggestionDialog::getSelectedSuggestion() const { return m_selectedText; }
void SuggestionDialog::onItemDoubleClicked(QListWidgetItem *item) { Q_UNUSED(item); acceptSelection(); }
void SuggestionDialog::acceptSelection() {
    QListWidgetItem *selectedItem = m_listWidget->currentItem();
    m_selectedText = selectedItem ? selectedItem->text() : "";
    QDialog::accept();
}
