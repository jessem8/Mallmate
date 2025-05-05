#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>
// Forward declarations for standard Qt Widgets used as members
class QLabel;
class QSpinBox;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
class QGridLayout;
class QGroupBox;

/**
 * @brief The SettingsWidget class provides a UI for configuring application settings.
 * It includes sections for general, database, and export options.
 * It communicates with its parent widget (e.g., OffresFlashScreen) to test connections.
 */
class SettingsWidget : public QWidget
{
    Q_OBJECT // Required for signals and slots

public:
    /**
     * @brief Constructor for the SettingsWidget.
     * @param parent The parent widget (optional).
     */
    explicit SettingsWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SettingsWidget() override;

    // --- Getters for retrieving current settings values ---
    int getDefaultOfferDuration() const;
    bool areNotificationsEnabled() const;
    QString getDefaultOfferType() const;
    QString getDbDsn() const;
    QString getDbUsername() const;
    QString getDbPassword() const;
    bool exportIncludeId() const;
    bool exportIncludeDescription() const;
    bool exportIncludePercent() const;
    bool exportIncludePrice() const;
    bool exportIncludeQuantity() const;
    bool exportIncludeStartDate() const;
    bool exportIncludeEndDate() const;
    bool exportIncludeStoreId() const;
    bool exportIncludeStoreName() const;
    bool exportIncludeValidationStatus() const;
    bool exportIncludeCalculatedStatus() const;
    bool exportIncludeClaimCount() const;
    // Note: getApiKey() getter was removed as API section is removed

    /**
     * @brief Public function to trigger saving the current UI settings
     * to persistent storage (QSettings).
     */
    void applySettings();

public slots:
    /**
     * @brief Slot to receive the result of the Database connection test
     * from the parent widget and update the status label.
     * @param success True if the connection succeeded, False otherwise.
     * @param message A message indicating the result or error details.
     */
    void onDatabaseTestResult(bool success, const QString &message);

    /**
     * @brief Slot to receive the result of the API key test.
     * (Note: Corresponding UI elements are removed, this slot might be unused).
     * @param success True if the test succeeded, False otherwise.
     * @param message A message indicating the result.
     */
    void onApiKeyTestResult(bool success, const QString &message);


private:
    // --- UI Elements ---

    // General Settings Group
    QGroupBox *m_generalGroup = nullptr;
    QFormLayout *m_generalLayout = nullptr;
    QLabel *m_durationLabel = nullptr;
    QSpinBox *m_defaultDurationSpinBox = nullptr;
    QLabel *m_notifLabel = nullptr;
    QCheckBox *m_enableNotificationsCheckBox = nullptr;
    QLabel *m_defaultOfferTypeLabel = nullptr;
    QComboBox *m_defaultOfferTypeCombo = nullptr;
    QLabel *m_suggestionThresholdLabel = nullptr;     // <<< ADDED for Threshold
    QSpinBox *m_suggestionThresholdSpinBox = nullptr;
    QLabel *m_defaultSuggestionDiscountLabel = nullptr;     // <<< ADD THIS LINE
    QDoubleSpinBox *m_defaultSuggestionDiscountSpinBox = nullptr;     // <<< ADDED for Threshold

    // Database Settings Group
    QGroupBox *m_databaseGroup = nullptr;
    QFormLayout *m_databaseLayout = nullptr;
    QLabel *m_dbDsnLabel = nullptr;
    QLineEdit *m_dbDsnEdit = nullptr;
    QLabel *m_dbUserLabel = nullptr;
    QLineEdit *m_dbUserEdit = nullptr;
    QLabel *m_dbPasswordLabel = nullptr;
    QLineEdit *m_dbPasswordEdit = nullptr;
    QPushButton *m_testDbButton = nullptr;
    QLabel *m_dbStatusLabel = nullptr;

    // Export Options Group
    QGroupBox *m_exportGroup = nullptr;
    QGridLayout *m_exportLayout = nullptr;
    QCheckBox *m_exportChkId = nullptr;
    QCheckBox *m_exportChkDesc = nullptr;
    QCheckBox *m_exportChkPercent = nullptr;
    QCheckBox *m_exportChkPrice = nullptr;
    QCheckBox *m_exportChkQty = nullptr;
    QCheckBox *m_exportChkStart = nullptr;
    QCheckBox *m_exportChkEnd = nullptr;
    QCheckBox *m_exportChkStoreId = nullptr;
    QCheckBox *m_exportChkStoreName = nullptr;
    QCheckBox *m_exportChkValidation = nullptr;
    QCheckBox *m_exportChkStatus = nullptr;
    QCheckBox *m_exportChkClaims = nullptr;

    // --- Helper Functions ---
    /**
     * @brief Creates and arranges all the UI widgets within the settings panel.
     * Called by the constructor.
     */
    void setupUi();

    /**
     * @brief Loads settings values from persistent storage (QSettings)
     * and populates the UI widgets. Called by the constructor.
     */
    void loadSettings();

    /**
     * @brief Saves the current values from the UI widgets to persistent
     * storage (QSettings). Called by applySettings().
     */
    void saveSettings();

private slots:
    /**
     * @brief Slot triggered when the "Tester Connexion" button for the database
     * is clicked. Emits the testDatabaseConnectionRequested signal.
     */
    void onTestDbConnectionClicked();

    // Note: onTestApiKeyClicked slot was removed as API section is removed

signals:
    /**
     * @brief Signal emitted when the user clicks the "Tester Connexion" button
     * for the database. The parent widget should connect to this signal
     * to perform the actual connection test.
     * @param dsn The DSN entered by the user.
     * @param user The username entered by the user.
     * @param password The password entered by the user.
     */
    void testDatabaseConnectionRequested(const QString &dsn, const QString &user, const QString &password);

    /**
     * @brief Signal emitted when the user clicks the API key test button.
     * (Note: Corresponding UI elements are removed, this signal might be unused).
     * @param apiKey The API key entered by the user.
     */
    void testApiKeyRequested(const QString &apiKey);

};

#endif // SETTINGSWIDGET_H
