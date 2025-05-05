#include "settingswidget.h" // Include the header for this class

// Include necessary Qt headers
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout> // For export layout
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QSettings>
#include <QtDebug>

/**
 * @brief Constructor for the SettingsWidget.
 * @param parent The parent widget (optional).
 */
SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
    // Initialize all member pointers to nullptr
    , m_generalGroup(nullptr)
    , m_generalLayout(nullptr)
    , m_durationLabel(nullptr)
    , m_defaultDurationSpinBox(nullptr)
    , m_notifLabel(nullptr)
    , m_enableNotificationsCheckBox(nullptr)
    , m_defaultOfferTypeLabel(nullptr)
    , m_defaultOfferTypeCombo(nullptr)
    , m_suggestionThresholdLabel(nullptr) // Initialize new member
    , m_suggestionThresholdSpinBox(nullptr) // Initialize new member
    , m_databaseGroup(nullptr)
    , m_databaseLayout(nullptr)
    , m_dbDsnLabel(nullptr)
    , m_dbDsnEdit(nullptr)
    , m_dbUserLabel(nullptr)
    , m_dbUserEdit(nullptr)
    , m_dbPasswordLabel(nullptr)
    , m_dbPasswordEdit(nullptr)
    , m_testDbButton(nullptr)
    , m_dbStatusLabel(nullptr)
    , m_exportGroup(nullptr)
    , m_exportLayout(nullptr)
    , m_exportChkId(nullptr)
    , m_exportChkDesc(nullptr)
    , m_exportChkPercent(nullptr)
    , m_exportChkPrice(nullptr)
    , m_exportChkQty(nullptr)
    , m_exportChkStart(nullptr)
    , m_exportChkEnd(nullptr)
    , m_exportChkStoreId(nullptr)
    , m_exportChkStoreName(nullptr)
    , m_exportChkValidation(nullptr)
    , m_exportChkStatus(nullptr)
    , m_exportChkClaims(nullptr)
{
    qDebug() << "SettingsWidget Constructor called.";

    setupUi(); // Build the UI
    loadSettings(); // Load previously saved settings

    // --- Connect Signals and Slots ---
    if (m_testDbButton) {
        connect(m_testDbButton, &QPushButton::clicked, this, &SettingsWidget::onTestDbConnectionClicked);
    } else {
        qWarning() << "SettingsWidget Constructor: m_testDbButton is null!";
    }
    // API test button connection removed
}

/**
 * @brief Destructor.
 */
SettingsWidget::~SettingsWidget()
{
    qDebug() << "SettingsWidget Destructor called.";
    // Child widgets are automatically deleted by Qt's parent-child mechanism
}

/**
 * @brief Sets up the UI layout and widgets for the settings page.
 */
void SettingsWidget::setupUi() {
    // --- Main Layout (Vertical) ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // --- General Settings Group ---
    m_generalGroup = new QGroupBox("Paramètres Généraux", this);
    m_generalLayout = new QFormLayout(m_generalGroup);
    m_generalLayout->setContentsMargins(10, 15, 10, 10);
    m_generalLayout->setHorizontalSpacing(15); // Use consistent spacing
    m_generalLayout->setVerticalSpacing(12);   // Use consistent spacing

    // Duration
    m_durationLabel = new QLabel("Durée par défaut des offres:", m_generalGroup);
    m_defaultDurationSpinBox = new QSpinBox(m_generalGroup);
    m_defaultDurationSpinBox->setRange(1, 90); m_defaultDurationSpinBox->setValue(7);
    m_defaultDurationSpinBox->setSuffix(" jours");
    m_defaultDurationSpinBox->setToolTip("Définir la durée initiale pour les nouvelles offres.");
    m_generalLayout->addRow(m_durationLabel, m_defaultDurationSpinBox);

    // Notifications
    m_notifLabel = new QLabel("Activer les notifications:", m_generalGroup);
    m_enableNotificationsCheckBox = new QCheckBox(m_generalGroup);
    m_enableNotificationsCheckBox->setChecked(true);
    m_enableNotificationsCheckBox->setToolTip("Activer/désactiver les notifications (fonctionnalité future).");
    m_generalLayout->addRow(m_notifLabel, m_enableNotificationsCheckBox);

    // Default Offer Type
    m_defaultOfferTypeLabel = new QLabel("Type d'offre par défaut:", m_generalGroup);
    m_defaultOfferTypeCombo = new QComboBox(m_generalGroup);
    m_defaultOfferTypeCombo->addItem("Réduction en Pourcentage", "PERCENTAGE");
    m_defaultOfferTypeCombo->addItem("Prix Spécial Fixe", "FIXED_PRICE");
    m_defaultOfferTypeCombo->setToolTip("Choisir le type de réduction pour les nouvelles offres.");
    m_generalLayout->addRow(m_defaultOfferTypeLabel, m_defaultOfferTypeCombo);

    // Suggestion Threshold
    m_suggestionThresholdLabel = new QLabel("Seuil Stock Suggestion IA:", m_generalGroup);
    m_suggestionThresholdSpinBox = new QSpinBox(m_generalGroup);
    m_suggestionThresholdSpinBox->setObjectName("suggestionThresholdSpinBox"); // Optional: Set object name
    m_suggestionThresholdSpinBox->setRange(10, 1000); // Set a reasonable range
    m_suggestionThresholdSpinBox->setValue(100);     // Default value
    m_suggestionThresholdSpinBox->setSuffix(" unités");
    m_suggestionThresholdSpinBox->setToolTip("Quantité minimale en stock pour suggérer une offre.");
    m_generalLayout->addRow(m_suggestionThresholdLabel, m_suggestionThresholdSpinBox);

    mainLayout->addWidget(m_generalGroup);
    // <<< --- ADD DEFAULT DISCOUNT WIDGETS --- >>>
            m_defaultSuggestionDiscountLabel = new QLabel("Réduction Suggestion IA (%):", m_generalGroup);
    m_defaultSuggestionDiscountSpinBox = new QDoubleSpinBox(m_generalGroup);
    m_defaultSuggestionDiscountSpinBox->setObjectName("defaultSuggestionDiscountSpinBox");
    m_defaultSuggestionDiscountSpinBox->setRange(1.0, 90.0); // Allow percentages like 1.0 to 90.0
    m_defaultSuggestionDiscountSpinBox->setDecimals(1);     // Allow one decimal place (e.g., 12.5%)
    m_defaultSuggestionDiscountSpinBox->setValue(15.0);     // Default value
    m_defaultSuggestionDiscountSpinBox->setSuffix(" %");
    m_defaultSuggestionDiscountSpinBox->setToolTip("Pourcentage de réduction suggéré par défaut par l'IA.");
    m_generalLayout->addRow(m_defaultSuggestionDiscountLabel, m_defaultSuggestionDiscountSpinBox);
    // <<< --- END ADDED WIDGETS --- >>>
    // --- Database Settings Group ---
    m_databaseGroup = new QGroupBox("Paramètres Base de Données (Oracle ODBC)", this);
    m_databaseLayout = new QFormLayout(m_databaseGroup);
    m_databaseLayout->setContentsMargins(10, 15, 10, 10);
    m_databaseLayout->setHorizontalSpacing(15); // Use consistent spacing
    m_databaseLayout->setVerticalSpacing(12);   // Use consistent spacing

    // DSN
    m_dbDsnLabel = new QLabel("Nom Source de Données (DSN):", m_databaseGroup);
    m_dbDsnEdit = new QLineEdit(m_databaseGroup);
    m_dbDsnEdit->setObjectName("dbDsnEdit");
    m_dbDsnEdit->setPlaceholderText("Ex: Mallmate_DBN");
    m_dbDsnEdit->setToolTip("Le nom de la source de données ODBC configurée sur ce système.");
    m_databaseLayout->addRow(m_dbDsnLabel, m_dbDsnEdit);

    // User
    m_dbUserLabel = new QLabel("Utilisateur Oracle:", m_databaseGroup);
    m_dbUserEdit = new QLineEdit(m_databaseGroup);
    m_dbUserEdit->setObjectName("dbUserEdit");
    m_dbUserEdit->setPlaceholderText("Ex: mallmate_user");
    m_databaseLayout->addRow(m_dbUserLabel, m_dbUserEdit);

    // Password
    m_dbPasswordLabel = new QLabel("Mot de passe Oracle:", m_databaseGroup);
    m_dbPasswordEdit = new QLineEdit(m_databaseGroup);
    m_dbPasswordEdit->setObjectName("dbPasswordEdit");
    m_dbPasswordEdit->setEchoMode(QLineEdit::Password);
    m_databaseLayout->addRow(m_dbPasswordLabel, m_dbPasswordEdit);

    // Test Button & Status Label
    m_testDbButton = new QPushButton("Tester Connexion", m_databaseGroup);
    m_testDbButton->setObjectName("testDbButton");
    m_testDbButton->setToolTip("Tenter d'établir une connexion avec ces paramètres.");
    m_dbStatusLabel = new QLabel("", m_databaseGroup);
    m_dbStatusLabel->setObjectName("dbStatusLabel"); // Set object name for styling/access
    m_dbStatusLabel->setStyleSheet("font-style: italic; color: #666;");
    QHBoxLayout* dbTestLayout = new QHBoxLayout(); // Layout for status label and button
    dbTestLayout->addWidget(m_dbStatusLabel, 1); // Status label takes available space
    dbTestLayout->addWidget(m_testDbButton);
    m_databaseLayout->addRow("", dbTestLayout); // Add the horizontal layout to the form row

    mainLayout->addWidget(m_databaseGroup);

    // --- Export Options Group ---
    m_exportGroup = new QGroupBox("Options d'Exportation CSV", this);
    m_exportGroup->setObjectName("exportGroup");
    m_exportLayout = new QGridLayout(m_exportGroup); // Use Grid Layout
    m_exportLayout->setContentsMargins(10, 15, 10, 10);
    m_exportLayout->setHorizontalSpacing(20);
    m_exportLayout->setVerticalSpacing(8);

    // Create Checkboxes
    m_exportChkId = new QCheckBox("Offre ID", m_exportGroup);
    m_exportChkDesc = new QCheckBox("Description", m_exportGroup);
    m_exportChkPercent = new QCheckBox("Pourcentage Réduction", m_exportGroup);
    m_exportChkPrice = new QCheckBox("Prix Spécial", m_exportGroup);
    m_exportChkQty = new QCheckBox("Quantité Limitée", m_exportGroup);
    m_exportChkStart = new QCheckBox("Date Début", m_exportGroup);
    m_exportChkEnd = new QCheckBox("Date Fin", m_exportGroup);
    m_exportChkStoreId = new QCheckBox("Magasin ID", m_exportGroup);
    m_exportChkStoreName = new QCheckBox("Nom Magasin", m_exportGroup);
    m_exportChkValidation = new QCheckBox("Statut Validation", m_exportGroup);
    m_exportChkStatus = new QCheckBox("Statut Calculé", m_exportGroup);
    m_exportChkClaims = new QCheckBox("Nb Réclamations", m_exportGroup);

    // Add checkboxes to the grid layout (e.g., 3 columns)
    m_exportLayout->addWidget(m_exportChkId, 0, 0);
    m_exportLayout->addWidget(m_exportChkDesc, 0, 1);
    m_exportLayout->addWidget(m_exportChkPercent, 0, 2);

    m_exportLayout->addWidget(m_exportChkPrice, 1, 0);
    m_exportLayout->addWidget(m_exportChkQty, 1, 1);
    m_exportLayout->addWidget(m_exportChkStart, 1, 2);

    m_exportLayout->addWidget(m_exportChkEnd, 2, 0);
    m_exportLayout->addWidget(m_exportChkStoreId, 2, 1);
    m_exportLayout->addWidget(m_exportChkStoreName, 2, 2);

    m_exportLayout->addWidget(m_exportChkValidation, 3, 0);
    m_exportLayout->addWidget(m_exportChkStatus, 3, 1);
    m_exportLayout->addWidget(m_exportChkClaims, 3, 2);

    mainLayout->addWidget(m_exportGroup);

    // --- Add Stretch ---
    mainLayout->addStretch(1); // Push content upwards

    // Set the main layout for this widget
    setLayout(mainLayout);
}

// --- Slot Implementations ---

/**
 * @brief Slot called when the "Test Connexion" button for the database is clicked.
 */
void SettingsWidget::onTestDbConnectionClicked() {
    if (!m_dbDsnEdit || !m_dbUserEdit || !m_dbPasswordEdit) {
        qWarning() << "SettingsWidget::onTestDbConnectionClicked: DB input fields are null!";
        return;
    }
    QString dsn = m_dbDsnEdit->text().trimmed();
    QString user = m_dbUserEdit->text().trimmed();
    QString password = m_dbPasswordEdit->text(); // Get password

    if (dsn.isEmpty() || user.isEmpty()) {
        QMessageBox::warning(this, "Paramètres Incomplets", "Veuillez entrer le nom DSN et l'utilisateur Oracle avant de tester.");
        return;
    }

    // Update status label to indicate testing
    if (m_dbStatusLabel) {
        m_dbStatusLabel->setText("<i>Test en cours...</i>");
        m_dbStatusLabel->setStyleSheet("font-style: italic; color: #666;"); // Reset style
    }

    // Emit signal to the parent widget (OffresFlashScreen) to perform the test
    emit testDatabaseConnectionRequested(dsn, user, password);
}

/**
 * @brief Slot to receive the result of the Database connection test from the parent.
 * @param success True if the connection succeeded, False otherwise.
 * @param message A message indicating the result or error.
 */
void SettingsWidget::onDatabaseTestResult(bool success, const QString &message) {
    if (!m_dbStatusLabel) return; // Check if label exists

    if (success) {
        m_dbStatusLabel->setText("<i>Connexion réussie.</i>");
        m_dbStatusLabel->setStyleSheet("font-style: italic; color: green;"); // Green for success
    } else {
        // Display the error message received from the parent
        m_dbStatusLabel->setText(QString("<i><b>Échec:</b> %1</i>").arg(message));
        m_dbStatusLabel->setStyleSheet("font-style: italic; color: red; font-weight: bold;"); // Red and bold for failure
    }
}

/**
 * @brief Slot to receive the result of the API key test. (Currently unused UI-wise)
 * @param success True if the test succeeded, False otherwise.
 * @param message A message indicating the result.
 */
void SettingsWidget::onApiKeyTestResult(bool success, const QString &message) {
    // We don't have m_apiKeyStatusLabel anymore, so just log it for now
    qDebug() << "SettingsWidget received API Key Test Result - Success:" << success << "Message:" << message;
    // If UI elements were present, update them here.
}


// --- Settings Load/Save Implementations ---

/**
 * @brief Loads settings from persistent storage (QSettings).
 */
void SettingsWidget::loadSettings() {
    qDebug() << "Loading settings...";
    QSettings settings; // Uses default constructor (app/org name)

    // --- Load General Settings ---
    int defaultDuration = settings.value("OfferSettings/defaultDuration", 7).toInt();
    if (m_defaultDurationSpinBox) m_defaultDurationSpinBox->setValue(defaultDuration);

    bool notificationsEnabled = settings.value("OfferSettings/notificationsEnabled", true).toBool();
    if (m_enableNotificationsCheckBox) m_enableNotificationsCheckBox->setChecked(notificationsEnabled);

    QString defaultOfferType = settings.value("OfferSettings/defaultOfferType", "PERCENTAGE").toString();
    if (m_defaultOfferTypeCombo) {
        int index = m_defaultOfferTypeCombo->findData(defaultOfferType);
        m_defaultOfferTypeCombo->setCurrentIndex(index > -1 ? index : 0);
    }

    // Load Suggestion Threshold
    int suggestionThreshold = settings.value("OfferSettings/suggestionStockThreshold", 100).toInt();
    if (m_suggestionThresholdSpinBox) m_suggestionThresholdSpinBox->setValue(suggestionThreshold);
    // <<< --- ADD DEFAULT DISCOUNT LOAD --- >>>
    double defaultDiscount = settings.value("OfferSettings/defaultSuggestionDiscount", 15.0).toDouble(); // Default 15.0%
    if (m_defaultSuggestionDiscountSpinBox) m_defaultSuggestionDiscountSpinBox->setValue(defaultDiscount);
    // <<< --- END ADDED LOAD --- >>>
    // --- Load Database Settings ---
    QString dbDsn = settings.value("Database/dsn", "Mallmate_DBN").toString();
    if (m_dbDsnEdit) m_dbDsnEdit->setText(dbDsn);

    QString dbUser = settings.value("Database/username", "mallmate_user").toString();
    if (m_dbUserEdit) m_dbUserEdit->setText(dbUser);

    // Note: Loading saved password into a QLineEdit is generally okay for local apps,
    // but be aware it's stored unencrypted by QSettings by default.
    QString dbPassword = settings.value("Database/password", "mallmate_password").toString();
    if (m_dbPasswordEdit) m_dbPasswordEdit->setText(dbPassword);

    if (m_dbStatusLabel) m_dbStatusLabel->clear(); // Clear status on load

    // --- Load Export Settings ---
    // Default to true (checked) if setting doesn't exist
    if(m_exportChkId) m_exportChkId->setChecked(settings.value("Export/includeId", true).toBool());
    if(m_exportChkDesc) m_exportChkDesc->setChecked(settings.value("Export/includeDescription", true).toBool());
    if(m_exportChkPercent) m_exportChkPercent->setChecked(settings.value("Export/includePercent", true).toBool());
    if(m_exportChkPrice) m_exportChkPrice->setChecked(settings.value("Export/includePrice", true).toBool());
    if(m_exportChkQty) m_exportChkQty->setChecked(settings.value("Export/includeQuantity", true).toBool());
    if(m_exportChkStart) m_exportChkStart->setChecked(settings.value("Export/includeStartDate", true).toBool());
    if(m_exportChkEnd) m_exportChkEnd->setChecked(settings.value("Export/includeEndDate", true).toBool());
    if(m_exportChkStoreId) m_exportChkStoreId->setChecked(settings.value("Export/includeStoreId", true).toBool());
    if(m_exportChkStoreName) m_exportChkStoreName->setChecked(settings.value("Export/includeStoreName", true).toBool());
    if(m_exportChkValidation) m_exportChkValidation->setChecked(settings.value("Export/includeValidationStatus", true).toBool());
    if(m_exportChkStatus) m_exportChkStatus->setChecked(settings.value("Export/includeCalculatedStatus", true).toBool());
    if(m_exportChkClaims) m_exportChkClaims->setChecked(settings.value("Export/includeClaimCount", true).toBool());

    qDebug() << "Settings loaded.";
}

/**
 * @brief Saves the current settings to persistent storage (QSettings). (Private helper)
 */
void SettingsWidget::saveSettings() {
    qDebug() << "Saving settings...";
    QSettings settings; // Uses default constructor

    // --- Save General Settings ---
    if (m_defaultDurationSpinBox) settings.setValue("OfferSettings/defaultDuration", m_defaultDurationSpinBox->value());
    if (m_enableNotificationsCheckBox) settings.setValue("OfferSettings/notificationsEnabled", m_enableNotificationsCheckBox->isChecked());
    if (m_defaultOfferTypeCombo) settings.setValue("OfferSettings/defaultOfferType", m_defaultOfferTypeCombo->currentData().toString());
    // Save Suggestion Threshold
    if (m_suggestionThresholdSpinBox) settings.setValue("OfferSettings/suggestionStockThreshold", m_suggestionThresholdSpinBox->value());

    // <<< --- ADD DEFAULT DISCOUNT SAVE --- >>>
    if (m_defaultSuggestionDiscountSpinBox) settings.setValue("OfferSettings/defaultSuggestionDiscount", m_defaultSuggestionDiscountSpinBox->value());
    // <<< --- END ADDED SAVE --- >>>
    // --- Save Database Settings ---
    if (m_dbDsnEdit) settings.setValue("Database/dsn", m_dbDsnEdit->text());
    if (m_dbUserEdit) settings.setValue("Database/username", m_dbUserEdit->text());
    // Note: Saving password unencrypted. Consider more secure methods if needed.
    if (m_dbPasswordEdit) settings.setValue("Database/password", m_dbPasswordEdit->text());

    // --- Save Export Settings ---
    if(m_exportChkId) settings.setValue("Export/includeId", m_exportChkId->isChecked());
    if(m_exportChkDesc) settings.setValue("Export/includeDescription", m_exportChkDesc->isChecked());
    if(m_exportChkPercent) settings.setValue("Export/includePercent", m_exportChkPercent->isChecked());
    if(m_exportChkPrice) settings.setValue("Export/includePrice", m_exportChkPrice->isChecked());
    if(m_exportChkQty) settings.setValue("Export/includeQuantity", m_exportChkQty->isChecked());
    if(m_exportChkStart) settings.setValue("Export/includeStartDate", m_exportChkStart->isChecked());
    if(m_exportChkEnd) settings.setValue("Export/includeEndDate", m_exportChkEnd->isChecked());
    if(m_exportChkStoreId) settings.setValue("Export/includeStoreId", m_exportChkStoreId->isChecked());
    if(m_exportChkStoreName) settings.setValue("Export/includeStoreName", m_exportChkStoreName->isChecked());
    if(m_exportChkValidation) settings.setValue("Export/includeValidationStatus", m_exportChkValidation->isChecked());
    if(m_exportChkStatus) settings.setValue("Export/includeCalculatedStatus", m_exportChkStatus->isChecked());
    if(m_exportChkClaims) settings.setValue("Export/includeClaimCount", m_exportChkClaims->isChecked());

    qDebug() << "Settings saved.";
}

/**
 * @brief Public function to trigger saving the settings.
 * Typically called when the user clicks an "Apply" or "Save" button
 * that might be located outside this specific widget (e.g., in OffresFlashScreen).
 */
void SettingsWidget::applySettings() {
    qDebug() << "SettingsWidget applySettings called.";
    saveSettings();
    // Optionally show a confirmation message
    // QMessageBox::information(this, "Paramètres", "Paramètres enregistrés avec succès.");
}


// --- Getter Implementations ---

int SettingsWidget::getDefaultOfferDuration() const {
    return m_defaultDurationSpinBox ? m_defaultDurationSpinBox->value() : 7; // Return default if null
}
bool SettingsWidget::areNotificationsEnabled() const {
    return m_enableNotificationsCheckBox ? m_enableNotificationsCheckBox->isChecked() : true; // Return default if null
}
QString SettingsWidget::getDefaultOfferType() const {
    return m_defaultOfferTypeCombo ? m_defaultOfferTypeCombo->currentData().toString() : "PERCENTAGE"; // Return default if null
}
QString SettingsWidget::getDbDsn() const {
    return m_dbDsnEdit ? m_dbDsnEdit->text() : QString(); // Return empty if null
}
QString SettingsWidget::getDbUsername() const {
    return m_dbUserEdit ? m_dbUserEdit->text() : QString(); // Return empty if null
}
QString SettingsWidget::getDbPassword() const {
    return m_dbPasswordEdit ? m_dbPasswordEdit->text() : QString(); // Return empty if null
}

// --- Export Setting Getters ---
bool SettingsWidget::exportIncludeId() const { return m_exportChkId ? m_exportChkId->isChecked() : true; }
bool SettingsWidget::exportIncludeDescription() const { return m_exportChkDesc ? m_exportChkDesc->isChecked() : true; }
bool SettingsWidget::exportIncludePercent() const { return m_exportChkPercent ? m_exportChkPercent->isChecked() : true; }
bool SettingsWidget::exportIncludePrice() const { return m_exportChkPrice ? m_exportChkPrice->isChecked() : true; }
bool SettingsWidget::exportIncludeQuantity() const { return m_exportChkQty ? m_exportChkQty->isChecked() : true; }
bool SettingsWidget::exportIncludeStartDate() const { return m_exportChkStart ? m_exportChkStart->isChecked() : true; }
bool SettingsWidget::exportIncludeEndDate() const { return m_exportChkEnd ? m_exportChkEnd->isChecked() : true; }
bool SettingsWidget::exportIncludeStoreId() const { return m_exportChkStoreId ? m_exportChkStoreId->isChecked() : true; }
bool SettingsWidget::exportIncludeStoreName() const { return m_exportChkStoreName ? m_exportChkStoreName->isChecked() : true; }
bool SettingsWidget::exportIncludeValidationStatus() const { return m_exportChkValidation ? m_exportChkValidation->isChecked() : true; }
bool SettingsWidget::exportIncludeCalculatedStatus() const { return m_exportChkStatus ? m_exportChkStatus->isChecked() : true; }
bool SettingsWidget::exportIncludeClaimCount() const { return m_exportChkClaims ? m_exportChkClaims->isChecked() : true; }
