#include "offersflashscreen.h"
#include "ui_offresflashscreen.h"
#include "offerdata.h"
#include "statswidget.h"
#include "settingswidget.h"
#include "suggestiondialog.h" // <<< ADD THIS INCLUDE
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QWidget>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QUuid>
#include <QCalendarWidget>
#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <utility> // For std::as_const
#include <QTimer>
#include <QStackedWidget>
#include <QApplication>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QIntValidator>
#include <QMetaType>   // For QVariant fix (QMetaType constructor)
#include <algorithm> // For std::sort
#include <QtSql/qsqlfield.h>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>
// --- NEW Includes from updated header ---
// #include "offeritemdelegate.h" // Already included via offresflashscreen.h
// #include <QGraphicsDropShadowEffect> // Already included via offresflashscreen.h
// #include <QButtonGroup> // Already included via offresflashscreen.h
#include <QAbstractButton> // Needed for slot parameter type
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest> // Added for creating requests
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl> // Added for API endpoint URL
#include <QUrlQuery>
#include <QSettings>
#include <algorithm>
#include "kioskmonitordialog.h"
#include <QRandomGenerator>
// Added for potential query parameters
// Define a connection name
const QString OFFERS_DB_CONNECTION_NAME = "oracle_mallmate_connection";

// --- REMOVED createStatBox helper function ---
// The stat boxes are now defined directly in the UI file and styled via QSS/C++.
// The labels within them (statValueLabelX, statTitleLabelX) will be updated directly.

// --- Constructor ---
// --- Constructor ---
// --- Constructor ---
// --- Constructor ---
OffresFlashScreen::OffresFlashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OffresFlashScreen), // Initialize ui first
    m_networkManager(nullptr),
    // Initialize pointers to nullptr
    m_mainMenu(nullptr),
    m_actionLogout(nullptr),
    m_actionQuit(nullptr),
    m_offerDelegate(nullptr),
    m_filterButtonGroup(nullptr),
    m_sidebarButtonGroup(nullptr),
    // Initialize state variables
    m_isEditMode(false),
    m_editingOfferId(-1),
    m_currentStatusFilter("Active"), // Default filter
    // Initialize sorting criteria
    m_currentSortColumn(SortColumn::ByDate),
    m_currentSortOrder(Qt::DescendingOrder),
    m_settingsWidgetInstance(nullptr), // Initialize settings widget instance pointer
    // ADD INITIALIZATION FOR NEW MEMBERS:
    m_kioskMonitorDialog(nullptr),     // Initialize Kiosk Monitor pointer
    m_arduinoSerialPort(nullptr),      // Initialize Serial Port pointer
    m_partialDataBuffer("")            // Initialize serial buffer
{
    qDebug() << "OffresFlashScreen Constructor called.";

    ui->setupUi(this); // Apply the .ui file design
    m_networkManager = new QNetworkAccessManager(this); // Initialize network manager

    // --- Load and Set Logo ---
    if (ui->topLogoLabel) {
        QPixmap logoPixmap(":/images/mallmate_logo.png"); // Ensure this path is correct in resources.qrc
        if(!logoPixmap.isNull()) {
            int logoWidth = 160; // Define desired width
            int logoHeight = 160; // Define desired height (adjust as needed)
            ui->topLogoLabel->setPixmap(logoPixmap.scaled(logoWidth, logoHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->topLogoLabel->setAlignment(Qt::AlignCenter);
        } else {
            qWarning("Logo resource ':/images/mallmate_logo.png' not found! Check resources.qrc.");
            ui->topLogoLabel->setText("MALLMATE"); // Fallback text
            ui->topLogoLabel->setAlignment(Qt::AlignCenter);
        }
    } else {
        qWarning() << "Constructor: ui->topLogoLabel not found in UI file! Check object name.";
    }

    // --- Menu Setup ---
    m_mainMenu = new QMenu(this);
    m_actionLogout = new QAction(QIcon(":/icons/logout.png"), "Deconnexion", this);
    m_actionQuit = new QAction(QIcon(":/icons/quit.png"), "Quitter", this);
    m_mainMenu->addAction(m_actionLogout);
    m_mainMenu->addAction(m_actionQuit);
    if (ui->topMenuButton) {
        ui->topMenuButton->setMenu(m_mainMenu);
        ui->topMenuButton->setPopupMode(QToolButton::InstantPopup);
        QIcon menuIcon(":/icons/menu.png"); // Load icon
        if (!menuIcon.isNull()) { ui->topMenuButton->setIcon(menuIcon); }
        else { ui->topMenuButton->setText("☰"); qWarning("Menu icon ':/icons/menu.png' not found!"); }
        ui->topMenuButton->setIconSize(QSize(24, 24));
        ui->topMenuButton->setAutoRaise(true);
        ui->topMenuButton->setCursor(Qt::PointingHandCursor);
    } else {
        qWarning() << "Constructor: ui->topMenuButton is null! Check object name in .ui file.";
    }

    // --- Set Icons for Action Bar Buttons ---
    qDebug() << "Setting icons for action bar buttons...";
    QSize iconSize(16, 16); // Define a standard size for the icons
    auto setButtonIcon = [&](QPushButton* button, const QString& resourcePath) {
        if (button) {
            QIcon icon(resourcePath);
            if (!icon.isNull()) {
                button->setIcon(icon);
                button->setIconSize(iconSize);
            } else {
                qWarning() << "Icon not found for button" << button->objectName() << "at path:" << resourcePath;
            }
        } else {
            qWarning() << "Button pointer is null when trying to set icon for path:" << resourcePath;
        }
    };
    setButtonIcon(ui->actionButtonModify, ":/icons/edit.png");
    setButtonIcon(ui->actionButtonDelete, ":/icons/delete.png");
    setButtonIcon(ui->actionButtonConsult, ":/icons/consult.png");
    setButtonIcon(ui->actionButtonViewClaims, ":/icons/claims.png");
    setButtonIcon(ui->actionButtonValidate, ":/icons/check_circle.png");
    setButtonIcon(ui->actionButtonReject, ":/icons/reject.png");
    // --- End of Icon Setting Block ---

    // --- Setup UI Enhancements ---
    setupOfferListDelegate(); // Set the custom delegate for the list
    setupFilterButtonGroup(); // Group the filter tabs
    setupSidebarButtonGroup(); // Setup sidebar group
    applyGraphicalEffects(); // Apply shadows etc.
    setupConnections(); // Setup signal/slot connections

    // --- Validator for quantity ---
    if(ui->formQuantityEdit) {
        ui->formQuantityEdit->setValidator(new QIntValidator(0, 999999, this));
    } else {
        qWarning() << "Constructor: ui->formQuantityEdit is null!";
    }

    this->setObjectName("OffresFlashScreen"); // Set object name for potential top-level styling

    // --- Initialize Database & Load Data ---
    if (initializeDatabaseConnection()) { // Call your existing DB connection function
        loadStoresIntoComboBox();   // Load stores first
        loadArticlesIntoComboBox(); // Load articles
        loadInitialData();          // Load offers (will sort and refresh)

        // --- Automatic Suggestion Generation on Startup ---
        // ... (keep existing suggestion code here) ...

    } else {
        QMessageBox::critical(this, "Erreur Critique",
                              "Impossible d'initialiser la connexion a la base de donnees.\n"
                              "L'application ne peut pas fonctionner correctement.");
        // Consider disabling UI elements or exiting if DB is critical
    }

    // --- ADDED: Initialize Serial Port ---
    setupSerialPort(); // Attempt to connect to Arduino

    // --- Initialize UI state ---
    resetFormState(); // Includes enabling/disabling buttons

    // --- Set initial view ---
    if (ui->contentStackedWidget) {
        ui->contentStackedWidget->setCurrentIndex(0); // Show the main content page (offreManagementPage)
        // Set the initial view WITHIN the main content page's stacked widget
        if (ui->centralStackedWidget) {
            ui->centralStackedWidget->setCurrentIndex(1); // Start on the list view (listPage, index 1)
        } else {
            qWarning() << "Constructor: ui->centralStackedWidget is null!";
        }
    } else {
        qWarning() << "Constructor: ui->contentStackedWidget is null!";
    }

    // --- Set initial sort combo box index ---
    if (ui->actionBarSortComboBox) {
        if (m_currentSortColumn == SortColumn::ByDate && m_currentSortOrder == Qt::DescendingOrder) {
            ui->actionBarSortComboBox->setCurrentIndex(0);
        } else if (m_currentSortColumn == SortColumn::ByDate && m_currentSortOrder == Qt::AscendingOrder) {
            ui->actionBarSortComboBox->setCurrentIndex(1);
        } else if (m_currentSortColumn == SortColumn::ByDescription && m_currentSortOrder == Qt::AscendingOrder) {
            ui->actionBarSortComboBox->setCurrentIndex(2);
        }
        qDebug() << "Default sort criteria set: Date Descending.";
    } else {
        qWarning() << "Constructor: ui->actionBarSortComboBox is null! Cannot set initial index.";
    }

    // --- Set the initial active filter tab visually ---
    updateFilterTabStyles();
    if (ui->formSuggestDescButton) connect(ui->formSuggestDescButton, &QPushButton::clicked, this, &OffresFlashScreen::onSuggestDescriptionClicked);
    else qWarning() << "setupConnections: ui->formSuggestDescButton is null!"; // Keep this connection

    // --- Ensure initial sidebar button state ---
    if(ui->sidebarButtonOffers) {
        ui->sidebarButtonOffers->setChecked(true);
    }

    qDebug() << "OffresFlashScreen Constructor finished.";
} // End of constructor

// --- Destructor ---
OffresFlashScreen::~OffresFlashScreen()
{
    qDebug() << "OffresFlashScreen Destructor called.";
    if (m_database.isOpen()) {
        QString connName = m_database.connectionName();
        m_database.close();
        QSqlDatabase::removeDatabase(connName);
        qDebug() << "Database connection" << connName << "closed and removed.";
    }
    // Delete objects created with new in the constructor or setup methods
    // The delegate and button group are parented to 'this', so Qt might handle them,
    // but explicit deletion is safer if ownership is clear.
    // delete m_offerDelegate; // Delegate has 'this' as parent, Qt manages it.
    // delete m_filterButtonGroup; // Button group has 'this' as parent, Qt manages it.
    delete ui; // Delete the UI object
}

// --- New Helper Function Implementations ---

/**
 * @brief Sets up signal-slot connections for the OffresFlashScreen widget.
 * Connects UI elements like buttons, list widgets, search fields, etc.,
 * to their corresponding handler slots in this class.
 */
void OffresFlashScreen::setupConnections()
{
    qDebug() << "Setting up connections...";

    // --- Form Page Connections ---
    if (ui->formSubmitButton) connect(ui->formSubmitButton, &QPushButton::clicked, this, &OffresFlashScreen::onFormSubmitClicked);
    else qWarning() << "setupConnections: ui->formSubmitButton is null!";
    if (ui->formCancelButton) connect(ui->formCancelButton, &QPushButton::clicked, this, &OffresFlashScreen::onFormCancelClicked);
    else qWarning() << "setupConnections: ui->formCancelButton is null!";
    if (ui->formShowListButton) connect(ui->formShowListButton, &QPushButton::clicked, this, &OffresFlashScreen::onFormShowListClicked);
    else qWarning() << "setupConnections: ui->formShowListButton is null!";

    // --- AI Suggestion Buttons Connections (REMOVED/COMMENTED OUT) ---
    // if (ui->formSuggestDescButton) connect(ui->formSuggestDescButton, &QPushButton::clicked, this, &OffresFlashScreen::onSuggestDescriptionClicked); // Keep if description suggestion remains
    // else qWarning() << "setupConnections: ui->formSuggestDescButton is null!";
    // if (ui->formSuggestButton) connect(ui->formSuggestButton, &QPushButton::clicked, this, &OffresFlashScreen::onSuggestDetailsClicked); // REMOVED
    // else qWarning() << "setupConnections: ui->formSuggestButton is null!"; // REMOVED

    // --- Action Bar Connections (List Page) ---
    if (ui->actionButtonModify) connect(ui->actionButtonModify, &QPushButton::clicked, this, &OffresFlashScreen::onModifierClicked);
    else qWarning() << "setupConnections: ui->actionButtonModify is null!";
    if (ui->actionButtonDelete) connect(ui->actionButtonDelete, &QPushButton::clicked, this, &OffresFlashScreen::onSupprimerClicked);
    else qWarning() << "setupConnections: ui->actionButtonDelete is null!";
    if (ui->actionButtonConsult) connect(ui->actionButtonConsult, &QPushButton::clicked, this, &OffresFlashScreen::onConsulterClicked);
    else qWarning() << "setupConnections: ui->actionButtonConsult is null!";
    if (ui->actionButtonViewClaims) connect(ui->actionButtonViewClaims, &QPushButton::clicked, this, &OffresFlashScreen::onViewClaimsClicked);
    else qWarning() << "setupConnections: ui->actionButtonViewClaims is null!";
    if (ui->actionButtonValidate) connect(ui->actionButtonValidate, &QPushButton::clicked, this, &OffresFlashScreen::onValidateOfferClicked);
    else qWarning() << "setupConnections: ui->actionButtonValidate is null!";
    if (ui->actionButtonReject) connect(ui->actionButtonReject, &QPushButton::clicked, this, &OffresFlashScreen::onRejectOfferClicked);
    else qWarning() << "setupConnections: ui->actionButtonReject is null!";
    if(ui->actionBarSearchEdit) connect(ui->actionBarSearchEdit, &QLineEdit::textChanged, this, &OffresFlashScreen::onSearchTextChanged);
    else qWarning() << "setupConnections: ui->actionBarSearchEdit is null!";
    if (ui->actionBarSortComboBox) connect(ui->actionBarSortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OffresFlashScreen::onSortCriteriaChanged);
    else qWarning() << "setupConnections: ui->actionBarSortComboBox is null!";

    // --- List Page Connections ---
    if (ui->offersListWidget) connect(ui->offersListWidget, &QListWidget::currentItemChanged, this, &OffresFlashScreen::onOfferSelectionChanged);
    else qWarning() << "setupConnections: ui->offersListWidget is null!";
    if(ui->listShowFormButton) connect(ui->listShowFormButton, &QPushButton::clicked, this, &OffresFlashScreen::onListShowFormClicked);
    else qWarning() << "setupConnections: ui->listShowFormButton is null!";
    if (m_filterButtonGroup) connect(m_filterButtonGroup, &QButtonGroup::buttonClicked, this, &OffresFlashScreen::onFilterGroupButtonClicked);
    else qWarning() << "setupConnections: m_filterButtonGroup is null!";

    // --- NEW: Connection for Stock-Based Suggestion Button ---
    if (ui->generateSuggestionsButton) { // <<< ADD THIS BLOCK
        connect(ui->generateSuggestionsButton, &QPushButton::clicked, this, &OffresFlashScreen::onGenerateSuggestionsClicked);
    } else {
        qWarning() << "setupConnections: ui->generateSuggestionsButton is null!";
    } // <<< END ADDED BLOCK

    // --- Menu Action Connections ---
    if (m_actionLogout) connect(m_actionLogout, &QAction::triggered, this, &OffresFlashScreen::onLogoutActionTriggered);
    if (m_actionQuit) connect(m_actionQuit, &QAction::triggered, this, &OffresFlashScreen::onQuitTriggered);

    // --- Sidebar Button Connections ---
    if (ui->sidebarButtonOffers) connect(ui->sidebarButtonOffers, &QPushButton::clicked, this, &OffresFlashScreen::onSidebarOffersClicked);
    else qWarning() << "setupConnections: ui->sidebarButtonOffers is null!";
    if (ui->sidebarButtonStats) connect(ui->sidebarButtonStats, &QPushButton::clicked, this, &OffresFlashScreen::onSidebarStatsClicked);
    else qWarning() << "setupConnections: ui->sidebarButtonStats is null!";
    if (ui->sidebarButtonSettings) connect(ui->sidebarButtonSettings, &QPushButton::clicked, this, &OffresFlashScreen::onSidebarSettingsClicked);
    else qWarning() << "setupConnections: ui->sidebarButtonSettings is null!";
    if (ui->sidebarButtonExport) connect(ui->sidebarButtonExport, &QPushButton::clicked, this, &OffresFlashScreen::onSidebarExportClicked);
    else qWarning() << "setupConnections: ui->sidebarButtonExport is null!";
    if (ui->sidebarButtonAbout) connect(ui->sidebarButtonAbout, &QPushButton::clicked, this, &OffresFlashScreen::onSidebarAboutClicked);
    else qWarning() << "setupConnections: ui->sidebarButtonAbout is null!";
    // <<< --- ADD QR CODE BUTTON CONNECTION --- >>>
    if (ui->sidebarButtonQrCode) {
        connect(ui->sidebarButtonQrCode, &QPushButton::clicked, this, &OffresFlashScreen::onQrCodeGeneratorClicked);
    } else {
        qWarning() << "setupConnections: ui->sidebarButtonQrCode is null!";
    }
    // <<< --- END QR CODE BUTTON CONNECTION --- >>>
    // --- Network Manager Connections (if needed for new feature, otherwise keep for description) ---
    // If you keep the description suggestion:
    // if (m_networkManager) {
    //     // Connections for network replies are usually made when the request is sent,
    //     // not necessarily here, but ensure m_networkManager is valid.
    // } else {
    //     qWarning() << "setupConnections: m_networkManager is null!";
    // }

    qDebug() << "Connections setup finished.";
}

void OffresFlashScreen::applyGraphicalEffects() {
    qDebug() << "Applying graphical effects...";

    // Lambda function to apply shadow effect consistently
    auto applyShadow = [this](QWidget* targetWidget) {
        if (!targetWidget) {
            qWarning() << "applyShadow: Target widget is null.";
            return;
        }
        // Check if an effect already exists, potentially remove it first if needed
        if (targetWidget->graphicsEffect()) {
            qDebug() << "Widget" << targetWidget->objectName() << "already has an effect. Replacing.";
            // Ownership note: If the effect is parented to 'this' or the widget,
            // deleting the old one might not be necessary if setGraphicsEffect handles it.
            // However, explicitly deleting might be safer if managing effect instances manually.
            // delete targetWidget->graphicsEffect(); // Use with caution depending on ownership
        }

        QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this); // Parent to screen
        shadowEffect->setBlurRadius(15); // Adjust blur radius
        shadowEffect->setColor(QColor(0, 0, 0, 70)); // Adjust color and alpha (e.g., 70/255 opacity)
        shadowEffect->setOffset(3, 5); // Adjust offset (horizontal, vertical)
        targetWidget->setGraphicsEffect(shadowEffect);

        // Important: Ensure the widget draws its background for the effect to be visible.
        // This might be handled by QSS setting a background-color, or explicitly:
        targetWidget->setAutoFillBackground(true);
        QPalette pal = targetWidget->palette();
        pal.setColor(QPalette::Window, Qt::white); // Ensure background color is set if QSS doesn't
        targetWidget->setPalette(pal);

        qDebug() << "Applied shadow effect to widget:" << targetWidget->objectName();
    };

    // Apply shadow to statistics cards (using new object names)
    if (ui->statsCard1) applyShadow(ui->statsCard1); else qWarning() << "applyGraphicalEffects: ui->statsCard1 is null!";
    if (ui->statsCard2) applyShadow(ui->statsCard2); else qWarning() << "applyGraphicalEffects: ui->statsCard2 is null!";
    if (ui->statsCard3) applyShadow(ui->statsCard3); else qWarning() << "applyGraphicalEffects: ui->statsCard3 is null!";
    if (ui->statsCard4) applyShadow(ui->statsCard4); else qWarning() << "applyGraphicalEffects: ui->statsCard4 is null!";

    // Optionally apply shadow to the form frame as well
    if (ui->formFrame) {
        // applyShadow(ui->formFrame); // Uncomment to add shadow to form
    } else {
        qWarning() << "applyGraphicalEffects: ui->formFrame is null!";
    }
    qDebug() << "Graphical effects applied.";
}

void OffresFlashScreen::setupOfferListDelegate() {
    if (!ui->offersListWidget) {
        qWarning() << "setupOfferListDelegate: ui->offersListWidget is null!";
        return;
    }
    // Create and set the custom delegate
    m_offerDelegate = new OfferItemDelegate(this); // Parent delegate to 'this'
    ui->offersListWidget->setItemDelegate(m_offerDelegate);
    qDebug() << "OfferItemDelegate set for offersListWidget.";

    // Optional: Adjust list widget properties for delegate usage
    ui->offersListWidget->setSpacing(2); // Add a small visual space between delegate-drawn items
    ui->offersListWidget->setUniformItemSizes(false); // Important if sizeHint returns variable sizes
    // Ensure view recalculates layout when model changes significantly
    // ui->offersListWidget->setResizeMode(QListView::Adjust); // Might impact performance slightly
}

void OffresFlashScreen::setupFilterButtonGroup() {
    if (!ui->listFilterActiveButton || !ui->listFilterEndedButton || !ui->listFilterValidatedButton || !ui->listFilterPendingButton || !ui->listFilterAllButton) {
        qWarning() << "setupFilterButtonGroup: One or more filter buttons are null!";
        return;
    }
    m_filterButtonGroup = new QButtonGroup(this);
    m_filterButtonGroup->setExclusive(true); // Only one button can be checked at a time

    // Add buttons to the group
    m_filterButtonGroup->addButton(ui->listFilterActiveButton);
    m_filterButtonGroup->addButton(ui->listFilterEndedButton);
    m_filterButtonGroup->addButton(ui->listFilterValidatedButton);
    m_filterButtonGroup->addButton(ui->listFilterPendingButton);
    m_filterButtonGroup->addButton(ui->listFilterAllButton);

    // Set the default checked button based on initial filter state
    if (m_currentStatusFilter.compare("Active", Qt::CaseInsensitive) == 0 || m_currentStatusFilter.compare("En cours", Qt::CaseInsensitive) == 0) {
        ui->listFilterActiveButton->setChecked(true);
    } else if (m_currentStatusFilter.compare("Terminee", Qt::CaseInsensitive) == 0) {
        ui->listFilterEndedButton->setChecked(true);
    } else if (m_currentStatusFilter.compare("Validee", Qt::CaseInsensitive) == 0) {
        ui->listFilterValidatedButton->setChecked(true);
    } else if (m_currentStatusFilter.compare("En Attente", Qt::CaseInsensitive) == 0) {
        ui->listFilterPendingButton->setChecked(true);
    } else if (m_currentStatusFilter.compare("All", Qt::CaseInsensitive) == 0) {
        ui->listFilterAllButton->setChecked(true);
    } else {
        // Default to 'Active' if initial state is unknown or invalid
        ui->listFilterActiveButton->setChecked(true);
        m_currentStatusFilter = "Active"; // Ensure state matches button
    }


    qDebug() << "Filter button group set up. Initial filter:" << m_currentStatusFilter;
    // Connection moved to setupConnections()
}
// --- setupSidebarButtonGroup (NEW Implementation) ---
void OffresFlashScreen::setupSidebarButtonGroup() {
    // Check if UI elements exist before using them
    if (!ui->sidebarButtonOffers || !ui->sidebarButtonStats || !ui->sidebarButtonSettings) {
        qWarning() << "setupSidebarButtonGroup: One or more checkable sidebar buttons are null in the UI file! Cannot create group.";
        return; // Stop if essential buttons are missing
    }

    // Create the button group and parent it to 'this' for automatic memory management
    m_sidebarButtonGroup = new QButtonGroup(this);
    m_sidebarButtonGroup->setExclusive(true); // Ensure only one button can be checked at a time

    // Add only the checkable navigation buttons to the group
    // These buttons were made checkable in the .ui file
    m_sidebarButtonGroup->addButton(ui->sidebarButtonOffers);
    m_sidebarButtonGroup->addButton(ui->sidebarButtonStats);
    m_sidebarButtonGroup->addButton(ui->sidebarButtonSettings);

    // The default checked state (sidebarButtonOffers) was set in the constructor/UI file.

    qDebug() << "Sidebar button group set up successfully.";
    // Note: We are connecting individual button clicked() signals in setupConnections,
    // so connecting the group's buttonClicked() signal is optional here unless
    // you want a single handler for all sidebar navigation clicks.
}

// --- Database Initialization (No changes needed) ---
bool OffresFlashScreen::initializeDatabaseConnection() {
    qDebug() << "Attempting to connect to Oracle via ODBC...";
    const QString connectionName = OFFERS_DB_CONNECTION_NAME;
    if (QSqlDatabase::contains(connectionName)) {
        m_database = QSqlDatabase::database(connectionName);
        if (m_database.isOpen()) {
            qDebug() << "Oracle connection" << connectionName << "already open.";
            return true;
        }
        // If connection exists but is closed, remove it before adding again
        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << "Removed existing closed Oracle connection:" << connectionName;
    }
    m_database = QSqlDatabase::addDatabase("QODBC", connectionName);
    if (!m_database.isValid()) {
        qWarning() << "Failed to add QODBC driver!";
        QMessageBox::critical(this, "Driver Error", "Failed to load the QODBC driver.");
        return false;
    }
    // --- IMPORTANT: Use correct DSN, User, Password ---
    QString dsnName = "Mallmate_DBN"; // Your ODBC Data Source Name
    QString oracleUser = "mallmate_user"; // Your Oracle Username
    QString oraclePassword = "mallmate_password"; // Your Oracle Password
    // --------------------------------------------------
    m_database.setDatabaseName(dsnName);
    m_database.setUserName(oracleUser);
    m_database.setPassword(oraclePassword);
    qDebug() << "Attempting to open connection for DSN:" << dsnName << "User:" << oracleUser;
    if (!m_database.open()) {
        qWarning() << "Failed to open Oracle database connection" << connectionName << ":" << m_database.lastError().text();
        QMessageBox::critical(this, "Database Connection Error",
                              QString("Could not open Oracle database via ODBC DSN '%1'.\n\n"
                                      "Driver: %2\n"
                                      "Error: %3\n\n"
                                      "Please check ODBC configuration and credentials.")
                                  .arg(dsnName)
                                  .arg(m_database.driverName())
                                  .arg(m_database.lastError().text()) );
        return false;
    }
    qDebug() << "Oracle database connection" << connectionName << "opened successfully via DSN:" << dsnName;
    return true;
}


// --- Create Table (Body Removed - Handled externally) ---

// --- Load Stores into ComboBox (Use new object name) ---
void OffresFlashScreen::loadStoresIntoComboBox() {
    // Use new object name
    if (!ui->formStoreComboBox) {
        qWarning() << "loadStoresIntoComboBox: ui->formStoreComboBox is null!";
        return;
    }
    if (!m_database.isOpen()) {
        qWarning() << "loadStoresIntoComboBox: Database is not open.";
        return;
    }

    QSqlQuery query(m_database);
    // Ensure correct table and column names for stores
    QString sql = "SELECT MagasinID, Nom FROM Magasins ORDER BY Nom ASC";
    qDebug() << "Executing SQL query:" << sql;

    if (!query.exec(sql)) {
        qWarning() << "Failed to execute SELECT query on MAGASINS table:" << query.lastError().text();
        QMessageBox::warning(this, "Database Error", "Could not load stores: " + query.lastError().text());
        return;
    }
    // Clear previous items except the placeholder
    int currentCount = ui->formStoreComboBox->count();
    for (int i = currentCount - 1; i > 0; --i) { // Start from end, stop before placeholder at index 0
        ui->formStoreComboBox->removeItem(i);
    }
    // Ensure placeholder exists
    if(ui->formStoreComboBox->count() == 0 || ui->formStoreComboBox->itemData(0).toInt() != -1) {
        ui->formStoreComboBox->clear(); // Clear completely just in case
        ui->formStoreComboBox->addItem("Choisir un magasin...", -1); // Add placeholder with data -1
    }


    int count = 0;
    while (query.next()) {
        int magasinId = query.value("MagasinID").toInt();
        QString nomMagasin = query.value("Nom").toString();
        if (magasinId > 0 && !nomMagasin.isEmpty()) {
            ui->formStoreComboBox->addItem(nomMagasin, magasinId); // Add item text and store ID as data
            count++;
        }
    }
    qDebug() << "Loaded" << count << "stores into ComboBox.";
}


// --- Load Initial Data (from Database - No significant changes needed here) ---
void OffresFlashScreen::loadInitialData() {
    qDebug() << "Loading offer data from Oracle database...";
    m_offerList.clear(); // Clear the in-memory list first

    if (!m_database.isOpen()) {
        qWarning() << "loadInitialData: Database is not open. Cannot load data.";
        if (!initializeDatabaseConnection()) {
            QMessageBox::critical(this, "Erreur DB", "Connexion impossible. Impossible de charger les offres.");
            return;
        }
    }

    QSqlQuery query(m_database);

    // SQL Query WITHOUT the ORDER BY clause - Sorting handled in memory
    // Added STATUT_VALIDATION and ARTICLEID columns
    QString selectSql = R"(
        SELECT
            o.OffreID, o.Description, o.PourcentageReduction, o.PrixSpecial,
            o.QuantiteLimitee, o.DateDebut, o.DateFin, o.MagasinID,
            o.STATUT_VALIDATION, -- ** Ensure this is your actual validation status column **
            o.ARTICLEID,         -- <<< ADDED ARTICLEID >>>
            (SELECT COUNT(*) FROM ReclamationsOffres rc WHERE rc.OffreID = o.OffreID) AS ClaimCount
        FROM
            OFFERS o
    )";

    qDebug() << "Executing SQL query:" << selectSql;
    if (!query.exec(selectSql)) {
        qWarning() << "Failed to execute SELECT query on OFFERS table:" << query.lastError().text();
        QMessageBox::warning(this, "Database Error", "Could not load offers from database: " + query.lastError().text());
        return;
    }

    // Iterate through the query results
    int count = 0;
    while (query.next()) {
        OfferData offer; // Assumes OfferData struct has claimCount, validationStatus, and articleId members

        offer.offreId = query.value("OffreID").toInt();
        offer.description = query.value("Description").toString();
        offer.percentageReduction = query.value("PourcentageReduction").isNull() ? 0.0 : query.value("PourcentageReduction").toDouble();
        offer.prixSpecial = query.value("PrixSpecial").isNull() ? 0.0 : query.value("PrixSpecial").toDouble();
        offer.quantiteLimitee = query.value("QuantiteLimitee").isNull() ? 0 : query.value("QuantiteLimitee").toInt();
        offer.startDateTime = query.value("DateDebut").toDateTime();
        offer.endDateTime = query.value("DateFin").toDateTime();
        offer.magasinId = query.value("MagasinID").toInt();
        offer.validationStatus = query.value("STATUT_VALIDATION").toString().trimmed().toUpper();
        offer.claimCount = query.value("ClaimCount").toInt();
        // <<< --- READ ARTICLE ID --- >>>
        // Use QVariant::isNull() to check for database NULL before converting
        QVariant articleIdVariant = query.value("ARTICLEID");
        offer.articleId = articleIdVariant.isNull() ? -1 : articleIdVariant.toInt(); // Store -1 if NULL in DB
        // <<< --- END READ ARTICLE ID --- >>>


        // Basic validation (excluding articleId check for now)
        if (offer.offreId <= 0 /* || !offer.startDateTime.isValid() || !offer.endDateTime.isValid() */) { // Allow invalid dates for now if needed
            // Only skip if ID is invalid, allow potentially invalid dates from DB for now
            qWarning() << "Skipping invalid record loaded from DB (OffreID:" << offer.offreId << ")";
            continue;
        }
        // Add specific date validation if required for existing offers
        if (!offer.startDateTime.isValid() || !offer.endDateTime.isValid()) {
            qWarning() << "Offer ID" << offer.offreId << "has invalid dates from DB.";
        }


        m_offerList.append(offer);
        count++;
    }

    if (query.lastError().isValid()) {
        qWarning() << "Error during query iteration:" << query.lastError().text();
    }
    qDebug() << "Loaded" << count << "offers from the database.";

    // Sort the list AFTER loading
    sortOfferList(); // Sort the data according to current criteria

    // After loading and sorting, refresh UI elements
    updateSummaryBoxes();
    refreshOfferList(); // Refresh the list display
}
// --- refreshOfferList Implementation (MODIFIED for Delegate) ---
void OffresFlashScreen::refreshOfferList() {
    if (!ui->offersListWidget) {
        qWarning() << "refreshOfferList: ui->offersListWidget is null! Cannot refresh.";
        return;
    }
    // Use new object name for search edit
    QString searchTerm = ui->actionBarSearchEdit ? ui->actionBarSearchEdit->text().trimmed() : "";

    qDebug() << "Refreshing list with filter:" << m_currentStatusFilter << "and search:" << searchTerm;

    // Store previously selected ID to attempt reselection
    int previouslySelectedId = -1;
    QListWidgetItem* currentItem = ui->offersListWidget->currentItem();
    if (currentItem) {
        previouslySelectedId = currentItem->data(Qt::UserRole).toInt(); // Assuming ID is stored in UserRole
    }

    ui->offersListWidget->clear(); // Clear existing items visually

    QListWidgetItem* itemToReselect = nullptr;

    // Iterate through the sorted in-memory list (m_offerList)
    for (const OfferData &offerData : std::as_const(m_offerList)) {
        // Determine the status based on dates and validation status
        QString currentActualStatus = offerData.getCurrentStatus(); // Use helper from OfferData

        // --- Apply Filter ---
        bool filterMatch = false;
        if (m_currentStatusFilter.compare("All", Qt::CaseInsensitive) == 0) {
            filterMatch = true; // Show all if 'All' filter is active
        } else if (m_currentStatusFilter.compare("Active", Qt::CaseInsensitive) == 0 || m_currentStatusFilter.compare("En cours", Qt::CaseInsensitive) == 0) {
            filterMatch = (currentActualStatus == "En cours");
        } else if (m_currentStatusFilter.compare("Terminee", Qt::CaseInsensitive) == 0) {
            filterMatch = (currentActualStatus == "Terminee");
        } else if (m_currentStatusFilter.compare("Validee", Qt::CaseInsensitive) == 0) {
            // Show only offers explicitly marked as VALIDATED and potentially active/upcoming
            filterMatch = (offerData.validationStatus == "VALIDATED");
            // Optionally add date checks here if 'Validated' should only show current/future ones
        } else if (m_currentStatusFilter.compare("En Attente", Qt::CaseInsensitive) == 0) {
            filterMatch = (offerData.validationStatus == "PENDING");
        } else if (m_currentStatusFilter.compare("Rejected", Qt::CaseInsensitive) == 0) { // Optional filter
            filterMatch = (offerData.validationStatus == "REJECTED");
        }
        // Add more specific filter logic if needed (e.g., combining status and dates)

        if (!filterMatch) {
            continue; // Skip item if it doesn't match the current filter
        }

        // --- Apply Search ---
        bool searchMatch = searchTerm.isEmpty() ||
                           offerData.description.contains(searchTerm, Qt::CaseInsensitive);
        // || offerData.storeName.contains(searchTerm, Qt::CaseInsensitive); // Add store name search later if needed

        if (!searchMatch) {
            continue; // Skip item if it doesn't match search term
        }

        // --- Add Item to List for Delegate ---
        QListWidgetItem *listItem = new QListWidgetItem(); // Create a standard item

        // Set data for ALL roles the delegate's paint() method expects
        listItem->setData(Qt::UserRole, offerData.offreId); // Store ID for identification
        listItem->setData(Qt::DisplayRole, offerData.description); // Role used by delegate for description

        // Format discount string for delegate
        QString discountDisplay;
        if (offerData.percentageReduction > 0) {
            discountDisplay = QString::number(offerData.percentageReduction, 'f', 1) + "%"; // Format as needed
        } else if (offerData.prixSpecial > 0) {
            discountDisplay = QString::number(offerData.prixSpecial, 'f', 2) + " TND"; // Add currency if needed
        } else {
            discountDisplay = "N/A";
        }
        listItem->setData(Qt::UserRole + 1, discountDisplay); // Role for discount/price string

        listItem->setData(Qt::UserRole + 2, offerData.startDateTime); // Role for start date
        listItem->setData(Qt::UserRole + 3, offerData.endDateTime);   // Role for end date
        listItem->setData(Qt::UserRole + 4, currentActualStatus);     // Role for calculated status string
        // Get store name (requires lookup or storing it in OfferData)
        QString storeNameDisplay = "Magasin ID: " + QString::number(offerData.magasinId); // Fallback
        if(ui->formStoreComboBox) {
            int storeIndex = ui->formStoreComboBox->findData(offerData.magasinId);
            if (storeIndex > -1) {
                storeNameDisplay = ui->formStoreComboBox->itemText(storeIndex);
            }
        }
        listItem->setData(Qt::UserRole + 5, storeNameDisplay); // Role for store name string

        // Add any other data roles the delegate might need

        ui->offersListWidget->addItem(listItem); // Add the item to the list widget

        // --- REMOVED setItemWidget call ---
        // ui->offersListWidget->setItemWidget(listItem, itemWidget); // Not needed with delegate

        // --- REMOVED connect for offerToggled ---

        // Check if this item should be reselected
        if (offerData.offreId == previouslySelectedId) {
            itemToReselect = listItem;
        }
    } // End of loop through m_offerList

    // Reselect item or clear selection
    if (itemToReselect) {
        ui->offersListWidget->setCurrentItem(itemToReselect);
        qDebug() << "Reselected item with ID:" << previouslySelectedId;
    } else {
        // If the previously selected item is no longer visible, clear selection
        if (ui->offersListWidget->currentItem()) {
            ui->offersListWidget->setCurrentItem(nullptr); // Explicitly clear selection
        }
        qDebug() << "No item reselected (or previous selection not found/visible).";
        // Update button states since selection changed (or was cleared)
        onOfferSelectionChanged(nullptr, nullptr);
    }

    qDebug() << "Offer list refresh complete. Items displayed:" << ui->offersListWidget->count();
}


// --- Slot Implementations (Updated/New) ---

// Renamed from onAjouterClicked
void OffresFlashScreen::onFormSubmitClicked() {
    qDebug() << "Form Submit button clicked! Edit Mode:" << m_isEditMode << "Editing ID:" << m_editingOfferId;

    // 1. Get data from form fields
    QString description = ui->formDescriptionEdit ? ui->formDescriptionEdit->text().trimmed() : "";
    QString discountStr = ui->formDiscountEdit ? ui->formDiscountEdit->text().trimmed() : "";
    QVariant storeIdVariant = ui->formStoreComboBox ? ui->formStoreComboBox->currentData() : QVariant();
    QVariant articleIdVariant = ui->formArticleComboBox ? ui->formArticleComboBox->currentData() : QVariant(); // <<< GET ARTICLE ID VARIANT
    QDateTime startDateTime = ui->formStartDateEdit ? ui->formStartDateEdit->dateTime() : QDateTime();
    QDateTime endDateTime = ui->formEndDateEdit ? ui->formEndDateEdit->dateTime() : QDateTime();
    QString quantiteStr = ui->formQuantityEdit ? ui->formQuantityEdit->text().trimmed() : "0";

    // 2. Validation
    QStringList errors;
    if (description.isEmpty()) errors << "- La description est obligatoire.";
    if (discountStr.isEmpty()) errors << "- Le champ Reduction / Prix Special est obligatoire.";
    if (!storeIdVariant.isValid() || storeIdVariant.toInt() <= 0) errors << "- Veuillez choisir un magasin valide.";
    // Article selection is OPTIONAL, so no validation needed for articleIdVariant unless required
    if (!startDateTime.isValid()) errors << "- La date de debut est invalide.";
    if (!endDateTime.isValid()) errors << "- La date de fin est invalide.";
    if (startDateTime.isValid() && endDateTime.isValid() && startDateTime >= endDateTime) {
        errors << "- La date de fin doit etre posterieure a la date de debut.";
    }

    double percentageReduction = 0.0;
    double prixSpecial = 0.0;
    bool discountIsPercentage = discountStr.contains('%');
    bool conversionOk = false;

    if (!discountStr.isEmpty()) {
        if (discountIsPercentage) {
            percentageReduction = discountStr.remove('%').trimmed().toDouble(&conversionOk);
            if (!conversionOk || percentageReduction < 0 || percentageReduction > 100) {
                errors << "- Le pourcentage de reduction est invalide (ex: 25%).";
            }
        } else {
            prixSpecial = discountStr.toDouble(&conversionOk);
            if (!conversionOk || prixSpecial < 0) {
                errors << "- Le prix special est invalide (ex: 9.99).";
            }
        }
    } // else: error already added if empty

    int quantiteLimitee = quantiteStr.isEmpty() ? 0 : quantiteStr.toInt(&conversionOk);
    if (!conversionOk || quantiteLimitee < 0) {
        errors << "- La quantite limitee doit etre un nombre positif ou 0.";
        quantiteLimitee = 0;
    }

    if (!errors.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez corriger les erreurs suivantes:\n" + errors.join("\n"));
        return;
    }
    int magasinId = storeIdVariant.toInt();
    // <<< --- GET ARTICLE ID (handle placeholder -1) --- >>>
    int articleId = articleIdVariant.isValid() ? articleIdVariant.toInt() : -1;
    if (articleId <= 0) {
        articleId = -1; // Treat 0 or invalid as -1 (meaning no specific article)
    }
    qDebug() << "Selected Article ID for saving:" << articleId;
    // <<< --- END GET ARTICLE ID --- >>>


    // 3. Check DB Connection
    if (!m_database.isOpen()) {
        qWarning() << "onFormSubmitClicked: Database is not open. Attempting to reconnect...";
        if (!initializeDatabaseConnection()) {
            QMessageBox::critical(this, "Erreur DB", "Connexion base de donnees perdue. Operation annulee.");
            return;
        }
    }

    // 4. DB Action (CORRECTED LOGIC + ARTICLEID)
    QSqlQuery query(m_database);
    QString queryString;
    bool success = false;
    bool performInsert = false;

    // Determine if we need to INSERT or UPDATE
    if (m_isEditMode && m_editingOfferId > 0) {
        // --- UPDATE existing offer ---
        performInsert = false;
        qDebug() << "Attempting DB UPDATE for OffreID:" << m_editingOfferId;
        queryString = "UPDATE OFFERS SET "
                      "Description = :desc, PourcentageReduction = :percent, PrixSpecial = :price, "
                      "QuantiteLimitee = :qty, DateDebut = :start, DateFin = :end, MagasinID = :magasin, "
                      "ARTICLEID = :articleid " // <<< ADD ARTICLEID to UPDATE
                      "WHERE OffreID = :id";
        if (!query.prepare(queryString)) {
            qWarning() << "Failed to prepare UPDATE query:" << query.lastError().text();
            QMessageBox::critical(this, "Erreur DB", "Erreur interne lors de la préparation de la requête de mise à jour.");
            return;
        }
        query.bindValue(":id", m_editingOfferId);

    } else {
        // --- INSERT new offer (either adding new or saving a suggestion) ---
        performInsert = true;
        if (m_isEditMode && m_editingOfferId == -1) {
            qDebug() << "Attempting DB INSERT for a previously unsaved suggestion.";
        } else {
            qDebug() << "Attempting DB INSERT for a new offer.";
        }
        queryString = "INSERT INTO OFFERS (Description, PourcentageReduction, PrixSpecial, QuantiteLimitee, DateDebut, DateFin, MagasinID, STATUT_VALIDATION, ARTICLEID) " // <<< ADD ARTICLEID
                      "VALUES (:desc, :percent, :price, :qty, :start, :end, :magasin, :status, :articleid)"; // <<< ADD :articleid
        if (!query.prepare(queryString)) {
            qWarning() << "Failed to prepare INSERT query:" << query.lastError().text();
            QMessageBox::critical(this, "Erreur DB", "Erreur interne lors de la préparation de la requête d'insertion.");
            return;
        }
        query.bindValue(":status", "PENDING");
    }

    // Bind common values
    query.bindValue(":desc", description);
    query.bindValue(":start", startDateTime);
    query.bindValue(":end", endDateTime);
    query.bindValue(":magasin", magasinId);
    query.bindValue(":percent", discountIsPercentage ? QVariant(percentageReduction) : QVariant(QMetaType(QMetaType::Double)));
    query.bindValue(":price", !discountIsPercentage ? QVariant(prixSpecial) : QVariant(QMetaType(QMetaType::Double)));
    query.bindValue(":qty", quantiteLimitee > 0 ? QVariant(quantiteLimitee) : QVariant(QMetaType(QMetaType::Int)));
    // <<< --- BIND ARTICLE ID (Handle NULL) --- >>>
    query.bindValue(":articleid", articleId > 0 ? QVariant(articleId) : QVariant(QMetaType(QMetaType::Int))); // Bind NULL if articleId is -1
    // <<< --- END BIND ARTICLE ID --- >>>


    // Execute the query
    qDebug() << "Executing Query:" << queryString;
    success = query.exec();

    // Handle results
    if (!success) {
        qWarning() << "Failed to" << (performInsert ? "insert" : "update") << "offer in database:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur DB", QString("Impossible d'%1 l'offre.\nError: %2")
                                                    .arg(performInsert ? "ajouter" : "mettre à jour")
                                                    .arg(query.lastError().text()));
    } else {
        qDebug() << "Successfully" << (performInsert ? "inserted" : "updated") << "offer"
                 << (performInsert ? "" : QString(" ID %1").arg(m_editingOfferId)) << "in database.";

        // Reload all data to reflect changes and get new IDs if inserted
        loadInitialData();

        resetFormState();
        QMessageBox::information(this, "Succès", QString("Offre %1 avec succès.")
                                                     .arg(performInsert ? "ajoutée" : "modifiée"));

        // Optionally switch back to list view after saving
        // if (ui->centralStackedWidget) ui->centralStackedWidget->setCurrentIndex(1);
    }
}

void OffresFlashScreen::handleApiKeyTestRequest(const QString &apiKey) {
    qDebug() << "Received request to test API Key:" << apiKey.left(4) + "...";
    qWarning() << "API Key testing UI has been removed, test not performed.";

    // We no longer have the API status label or the slot on SettingsWidget
    // to report back to. If testing were still needed, the result handling
    // would need to be different. For now, we just log that it's not active.

    // Example of sending a dummy result back if the slot still existed:
    // if (m_settingsWidgetInstance) {
    //     m_settingsWidgetInstance->onApiKeyTestResult(false, "API testing désactivé.");
    // }
}

void OffresFlashScreen::handleDatabaseTestRequest(const QString &dsn, const QString &user, const QString &password) {
    qDebug() << "Received request to test Database connection. DSN:" << dsn << "User:" << user;

    if (!m_settingsWidgetInstance) {
        qWarning() << "handleDatabaseTestRequest: m_settingsWidgetInstance is null!";
        return;
    }

    // Attempt to open a temporary database connection for testing
    // Use a unique connection name to avoid conflicts with the main connection
    const QString testConnectionName = "db_test_connection";

    // Remove any existing connection with the same name first
    if (QSqlDatabase::contains(testConnectionName)) {
        QSqlDatabase::removeDatabase(testConnectionName);
    }

    // Add the database driver
    QSqlDatabase testDb = QSqlDatabase::addDatabase("QODBC", testConnectionName);
    if (!testDb.isValid()) {
        qWarning() << "Failed to add QODBC driver for testing!";
        m_settingsWidgetInstance->onDatabaseTestResult(false, "Driver QODBC invalide.");
        return;
    }

    // Set connection parameters
    testDb.setDatabaseName(dsn);
    testDb.setUserName(user);
    testDb.setPassword(password);

    qDebug() << "Attempting to open test connection for DSN:" << dsn << "User:" << user;
    bool success = testDb.open();
    QString message;

    if (success) {
        message = "Connexion réussie.";
        qDebug() << "Database test connection successful.";
        testDb.close(); // Close the connection immediately after testing
    } else {
        message = testDb.lastError().text(); // Get the specific error message
        qWarning() << "Database test connection failed:" << message;
    }

    // Remove the temporary connection from Qt's list
    QSqlDatabase::removeDatabase(testConnectionName);

    // Send result back to SettingsWidget
    m_settingsWidgetInstance->onDatabaseTestResult(success, message);
}
// New Slot Implementation
void OffresFlashScreen::onFormCancelClicked() {
    qDebug() << "Form Cancel button clicked.";
    resetFormState(); // Clear form and exit edit mode
    // Switch back to the list view
    if (ui->centralStackedWidget) {
        ui->centralStackedWidget->setCurrentIndex(1); // Index 1 should be the listPage
    }
}

// Updated implementation for Modifier button
void OffresFlashScreen::onModifierClicked() {
    qDebug() << "Modifier button clicked!";
    QListWidgetItem *selectedItem = ui->offersListWidget ? ui->offersListWidget->currentItem() : nullptr;
    if (!selectedItem) {
        QMessageBox::information(this, "Action impossible", "Veuillez selectionner une offre dans la liste pour la modifier.");
        return;
    }

    bool conversionOk;
    int selectedOfferId = selectedItem->data(Qt::UserRole).toInt(&conversionOk);
    const OfferData* offerDataToLoad = nullptr; // Pointer to hold the data to load into the form

    // --- Find the OfferData to load ---
    if (conversionOk && selectedOfferId == -1) {
        // --- Handle Unsaved Suggestion (ID == -1) ---
        qDebug() << "Finding data for unsaved AI suggestion in m_offerList.";
        // We need to find the unsaved suggestion in m_offerList.
        // Since ID is -1, we might need a better way, but let's try matching description for now.
        // A better approach would be to store a pointer or unique temporary ID.
        QString selectedDesc = selectedItem->data(Qt::DisplayRole).toString();
        for (const auto& offer : std::as_const(m_offerList)) {
            // Match based on description AND having ID -1
            if (offer.offreId == -1 && offer.description == selectedDesc) {
                offerDataToLoad = &offer;
                qDebug() << "Found unsaved suggestion data in list. ArticleID:" << offerDataToLoad->articleId;
                break;
            }
        }
        if (!offerDataToLoad) {
            qWarning() << "Could not find matching unsaved suggestion in m_offerList for description:" << selectedDesc;
            QMessageBox::warning(this, "Erreur Interne", "Impossible de retrouver les données de la suggestion sélectionnée.");
            return;
        }
        m_editingOfferId = -1; // Confirm we are editing an unsaved item

    } else if (conversionOk && selectedOfferId > 0) {
        // --- Handle Existing Offer (ID > 0) ---
        qDebug() << "Finding data for existing offer ID:" << selectedOfferId;
        m_editingOfferId = selectedOfferId; // Store the valid ID
        for (const auto& offer : std::as_const(m_offerList)) {
            if (offer.offreId == m_editingOfferId) {
                offerDataToLoad = &offer;
                qDebug() << "Found existing offer data in list. ArticleID:" << offerDataToLoad->articleId;
                break;
            }
        }
        if (!offerDataToLoad) {
            qWarning() << "Could not find data in m_offerList for offer ID" << m_editingOfferId;
            QMessageBox::warning(this, "Erreur", "Impossible de trouver les donnees de l'offre selectionnee.");
            m_editingOfferId = -1; // Reset ID on error
            return;
        }
    } else {
        // --- Handle Invalid ID ---
        qWarning() << "Selected item has invalid or missing offer ID data:" << selectedItem->data(Qt::UserRole);
        QMessageBox::warning(this, "Erreur", "Impossible de recuperer l'ID de l'offre selectionnee.");
        m_editingOfferId = -1; // Ensure ID is reset
        return; // Don't proceed
    }

    // --- Populate Form Fields ---
    if (!ui->formDescriptionEdit || !ui->formDiscountEdit || !ui->formStoreComboBox ||
        !ui->formArticleComboBox || // <<< Check new combo box
        !ui->formStartDateEdit || !ui->formEndDateEdit || !ui->formSubmitButton || !ui->formQuantityEdit)
    {
        qWarning() << "One or more form widgets are null in onModifierClicked.";
        QMessageBox::critical(this, "Erreur Interne", "Certains champs du formulaire n'ont pas pu etre initialises.");
        return;
    }

    ui->formDescriptionEdit->setText(offerDataToLoad->description);
    // Populate discount/price field
    if (offerDataToLoad->percentageReduction > 0) {
        ui->formDiscountEdit->setText(QString::number(offerDataToLoad->percentageReduction, 'f', 1) + "%");
    } else if (offerDataToLoad->prixSpecial > 0) {
        ui->formDiscountEdit->setText(QString::number(offerDataToLoad->prixSpecial, 'f', 2));
    } else {
        ui->formDiscountEdit->clear();
    }
    ui->formQuantityEdit->setText(offerDataToLoad->quantiteLimitee > 0 ? QString::number(offerDataToLoad->quantiteLimitee) : "");

    // Select store in combobox
    int storeIndex = ui->formStoreComboBox->findData(offerDataToLoad->magasinId);
    ui->formStoreComboBox->setCurrentIndex(storeIndex > -1 ? storeIndex : 0); // Select store or default placeholder

    // <<< --- SELECT ARTICLE IN COMBOBOX --- >>>
    int articleIndex = ui->formArticleComboBox->findData(offerDataToLoad->articleId);
    ui->formArticleComboBox->setCurrentIndex(articleIndex > -1 ? articleIndex : 0); // Select article or placeholder
    qDebug() << "Attempting to select ArticleID:" << offerDataToLoad->articleId << "Found at index:" << articleIndex;
    // <<< --- END SELECT ARTICLE --- >>>

    // Set dates (handle potentially invalid dates for suggestions)
    if (offerDataToLoad->startDateTime.isValid()) {
        ui->formStartDateEdit->setDateTime(offerDataToLoad->startDateTime);
    } else {
        ui->formStartDateEdit->setDateTime(QDateTime::currentDateTime()); // Default for suggestions
    }
    if (offerDataToLoad->endDateTime.isValid()) {
        ui->formEndDateEdit->setDateTime(offerDataToLoad->endDateTime);
    } else {
        ui->formEndDateEdit->setDateTime(QDateTime::currentDateTime().addDays(7)); // Default for suggestions
    }


    // --- Enter Edit Mode ---
    m_isEditMode = true;
    qDebug() << "Entering edit mode for offer ID:" << m_editingOfferId << "(or -1 for suggestion)";
    ui->formSubmitButton->setText("Enregistrer");
    ui->formSubmitButton->setToolTip(m_editingOfferId == -1 ? "Enregistrer la nouvelle offre suggérée" : "Enregistrer les modifications de l'offre");


    // --- Common actions after setting up the form ---
    // Switch to the form page
    if (ui->centralStackedWidget) ui->centralStackedWidget->setCurrentIndex(0); // Index 0 is formPage
    if(ui->formDescriptionEdit) ui->formDescriptionEdit->setFocus(); // Focus first field
}
void OffresFlashScreen::onSupprimerClicked() {
    qDebug() << "Supprimer button clicked!";
    QListWidgetItem *selectedItem = ui->offersListWidget ? ui->offersListWidget->currentItem() : nullptr;
    if (!selectedItem) {
        QMessageBox::information(this, "Action impossible", "Veuillez selectionner une offre a supprimer.");
        return;
    }

    bool conversionOk;
    int offerIdToDelete = selectedItem->data(Qt::UserRole).toInt(&conversionOk);
    QString offerDesc = selectedItem->data(Qt::DisplayRole).toString(); // Get description for confirmation

    // --- NEW: Handle Unsaved Suggestion (ID == -1) ---
    if (conversionOk && offerIdToDelete == -1) {
        qDebug() << "Attempting to remove unsaved AI suggestion from the list.";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Confirmation Suppression Suggestion",
                                      QString("Etes-vous sur de vouloir supprimer la suggestion \"%1\" de la liste ?").arg(offerDesc),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Remove the item directly from the list widget
            int row = ui->offersListWidget->row(selectedItem);
            delete ui->offersListWidget->takeItem(row); // takeItem removes and returns, delete frees memory
            qDebug() << "Removed suggestion item from list widget.";

            // Optional: Remove the corresponding unsaved OfferData from m_offerList
            // This requires finding it based on something other than ID (e.g., description or pointer if stored)
            // For simplicity now, we might skip removing from m_offerList, it will just disappear on next full reload.
            // Or, if performance is critical, implement removal logic here.

            // Refresh summary boxes as pending count might change
            updateSummaryBoxes();
            // Ensure button states are updated as selection is now gone
            onOfferSelectionChanged(nullptr, nullptr);

            QMessageBox::information(this, "Succès", "Suggestion supprimée de la liste.");
        } else {
            qDebug() << "User cancelled suggestion removal.";
        }
        return; // Stop here for unsaved suggestions
    }

    // --- Handle Existing Offer (ID > 0) ---
    else if (conversionOk && offerIdToDelete > 0) {
        qDebug() << "Attempting to delete existing offer ID:" << offerIdToDelete;
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Confirmation de suppression",
                                      QString("Etes-vous sur de vouloir supprimer l'offre \"%1\" (ID: %2) de la base de données ?\nCette action est irreversible.")
                                          .arg(offerDesc).arg(offerIdToDelete),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            qDebug() << "User confirmed deletion for offer ID:" << offerIdToDelete;
            if (!m_database.isOpen()) {
                qWarning() << "onSupprimerClicked: Database is not open. Attempting to reconnect...";
                if (!initializeDatabaseConnection()) {
                    QMessageBox::critical(this, "Erreur DB", "Connexion base de donnees perdue. Operation annulee.");
                    return;
                }
            }

            // --- Database Deletion Logic (Original) ---
            // Consider related data (e.g., claims)
            // QSqlQuery deleteClaimsQuery(m_database); ... etc ...

            QSqlQuery query(m_database);
            query.prepare("DELETE FROM OFFERS WHERE OffreID = :id");
            query.bindValue(":id", offerIdToDelete);
            if(!query.exec()) {
                qWarning() << "Failed to delete offer from database:" << query.lastError().text();
                QMessageBox::warning(this, "Erreur DB", "Impossible de supprimer l'offre.\n" + query.lastError().text());
            } else {
                qDebug() << "Successfully deleted offer" << offerIdToDelete << "from database. Rows affected:" << query.numRowsAffected();
                loadInitialData(); // Reload data and refresh UI
                QMessageBox::information(this, "Succes", "Offre supprimee avec succes.");
            }
            // --- End Database Deletion Logic ---

        } else {
            qDebug() << "User cancelled deletion for offer ID:" << offerIdToDelete;
        }
        return; // Stop here for existing offers
    }

    // --- Handle Invalid ID ---
    else {
        qWarning() << "Selected item has invalid or missing offer ID data! Cannot delete.";
        QMessageBox::warning(this, "Erreur", "Impossible de recuperer l'ID de l'offre selectionnee.");
        return;
    }
}
// Updated implementation for Consulter button
void OffresFlashScreen::onConsulterClicked() {
    qDebug() << "Consulter button clicked!";
    // Use new object name for list widget
    QListWidgetItem *selectedItem = ui->offersListWidget ? ui->offersListWidget->currentItem() : nullptr;
    if (!selectedItem) {
        QMessageBox::information(this, "Action impossible", "Veuillez selectionner une offre dans la liste pour la consulter.");
        return;
    }
    bool conversionOk;
    int offerIdToConsult = selectedItem->data(Qt::UserRole).toInt(&conversionOk); // ID stored in UserRole
    if (!conversionOk || offerIdToConsult <= 0) {
        qWarning() << "Selected item has invalid or missing offer ID data!";
        QMessageBox::warning(this, "Erreur", "Impossible de recuperer l'ID de l'offre selectionnee.");
        return;
    }
    // Find the offer data in the in-memory list
    const OfferData* selectedOfferData = nullptr;
    for (const auto& offer : std::as_const(m_offerList)) {
        if (offer.offreId == offerIdToConsult) {
            selectedOfferData = &offer;
            break;
        }
    }
    if (selectedOfferData) {
        // Retrieve display strings directly from the item data roles (as set in refreshOfferList)
        QString desc = selectedItem->data(Qt::DisplayRole).toString();
        QString discountInfo = selectedItem->data(Qt::UserRole + 1).toString();
        QDateTime startDate = selectedItem->data(Qt::UserRole + 2).toDateTime();
        QDateTime endDate = selectedItem->data(Qt::UserRole + 3).toDateTime();
        QString statusInfo = selectedItem->data(Qt::UserRole + 4).toString();
        QString storeInfo = selectedItem->data(Qt::UserRole + 5).toString();
        QString quantiteInfo = (selectedOfferData->quantiteLimitee > 0)
                                   ? QString("Quantite Limitee: %1").arg(selectedOfferData->quantiteLimitee)
                                   : QString("Quantite Limitee: Illimitee");

        QString details = QString(
                              "<b>Details de l'offre :</b><br><br>"
                              "<b>Offre ID :</b> %1<br>"
                              "<b>Description :</b> %2<br>"
                              "<b>%3</b><br>" // Discount/Price already formatted
                              "%4<br>" // Quantite
                              "<b>Magasin :</b> %5<br>" // Store Name already formatted
                              "<b>Debut :</b> %6<br>"
                              "<b>Fin :</b> %7<br>"
                              "<b>Statut Actuel :</b> %8"
                              )
                              .arg(selectedOfferData->offreId)
                              .arg(desc)
                              .arg(discountInfo)
                              .arg(quantiteInfo)
                              .arg(storeInfo)
                              .arg(startDate.toString("dd/MM/yyyy hh:mm"))
                              .arg(endDate.toString("dd/MM/yyyy hh:mm"))
                              .arg(statusInfo); // Status already formatted

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Details de l'offre");
        msgBox.setTextFormat(Qt::RichText); // Allow HTML formatting
        msgBox.setText(details);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton("OK", QMessageBox::AcceptRole);
        msgBox.exec();
    } else {
        qWarning() << "Could not find offer ID" << offerIdToConsult << "in m_offerList to consult.";
        QMessageBox::warning(this, "Erreur", "Impossible de trouver les details pour l'offre selectionnee.");
    }
}

// Updated implementation for selection changes
void OffresFlashScreen::onOfferSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    Q_UNUSED(previous);
    bool itemSelected = (current != nullptr);
    qDebug() << "onOfferSelectionChanged: Item selected:" << itemSelected;

    // --- Default states ---
    bool enableModify = false;
    bool enableDelete = false;
    bool enableConsult = false;
    bool enableClaims = false;
    bool enableValidationButtons = false; // For Validate/Reject

    if (itemSelected) {
        bool conversionOk = false;
        int selectedId = current->data(Qt::UserRole).toInt(&conversionOk);

        if (conversionOk && selectedId == -1) {
            // --- Handle AI Suggestion (Not Saved Yet) ---
            qDebug() << "Selected item is an unsaved AI suggestion (ID: -1)";
            enableModify = true;  // Allow modifying to fill details
            enableDelete = true;  // Allow deleting the suggestion *before* saving (optional, but often useful)
            enableConsult = false; // Cannot consult DB details for unsaved item
            enableClaims = false; // Cannot view claims for unsaved item
            enableValidationButtons = true; // It's PENDING, so allow Validate/Reject

        } else if (conversionOk && selectedId > 0) {
            // --- Handle Regular Offer (From DB) ---
            qDebug() << "Selected item is a regular offer (ID:" << selectedId << ")";
            enableModify = true;
            enableDelete = true;
            enableConsult = true;
            enableClaims = true; // Enable claims button for existing offers

            // Check validation status for Validate/Reject buttons
            for(const auto& offer : std::as_const(m_offerList)) {
                if (offer.offreId == selectedId) {
                    // Enable Validate/Reject only if status is PENDING
                    if (offer.validationStatus == "PENDING") {
                        enableValidationButtons = true;
                        qDebug() << "Offer ID" << selectedId << "is PENDING, enabling validation buttons.";
                    } else {
                        qDebug() << "Offer ID" << selectedId << "is NOT PENDING (Status:" << offer.validationStatus << "), disabling validation buttons.";
                    }
                    break; // Found the offer
                }
            }
        } else {
            // --- Handle Invalid ID ---
            qWarning() << "Selected item has invalid or missing offer ID data:" << current->data(Qt::UserRole);
            // Keep all buttons disabled
        }
    } else {
        // --- No Item Selected ---
        qDebug() << "No item selected.";
        // All buttons remain disabled (as per default states)
    }

    // --- Apply Enabled States to Buttons ---
    if(ui->actionButtonModify) ui->actionButtonModify->setEnabled(enableModify);
    if(ui->actionButtonDelete) ui->actionButtonDelete->setEnabled(enableDelete);
    if(ui->actionButtonConsult) ui->actionButtonConsult->setEnabled(enableConsult);
    if(ui->actionButtonViewClaims) ui->actionButtonViewClaims->setEnabled(enableClaims);
    if(ui->actionButtonValidate) ui->actionButtonValidate->setEnabled(enableValidationButtons);
    if(ui->actionButtonReject) ui->actionButtonReject->setEnabled(enableValidationButtons);

    qDebug() << "Button states updated - Modify:" << enableModify << "Delete:" << enableDelete
             << "Consult:" << enableConsult << "Claims:" << enableClaims << "Validate/Reject:" << enableValidationButtons;
}

// Updated implementation for resetting form state
void OffresFlashScreen::resetFormState() {
    qDebug() << "Resetting form state.";
    // Clear form fields (using new object names)
    if(ui->formDescriptionEdit) ui->formDescriptionEdit->clear();
    if(ui->formDiscountEdit) ui->formDiscountEdit->clear();
    if(ui->formQuantityEdit) ui->formQuantityEdit->clear();
    if(ui->formStoreComboBox) ui->formStoreComboBox->setCurrentIndex(0); // Reset to placeholder
    if(ui->formStartDateEdit) ui->formStartDateEdit->setDateTime(QDateTime::currentDateTime());
    if(ui->formEndDateEdit) ui->formEndDateEdit->setDateTime(QDateTime::currentDateTime().addDays(7));

    // Exit edit mode
    m_isEditMode = false;
    m_editingOfferId = -1;

    // Reset Add/Save button (using new object name)
    if(ui->formSubmitButton) {
        ui->formSubmitButton->setText("Ajouter");
        ui->formSubmitButton->setToolTip("Ajouter une nouvelle offre flash");
    }

    // Update action buttons based on list selection ONLY if list view is active
    bool itemIsSelectedInVisibleList = (ui->offersListWidget &&
                                        ui->centralStackedWidget &&
                                        ui->centralStackedWidget->currentIndex() == 1 && // Index 1 is listPage
                                        ui->offersListWidget->currentItem() != nullptr);

    // Use new action button names
    if(ui->actionButtonModify) ui->actionButtonModify->setEnabled(itemIsSelectedInVisibleList);
    if(ui->actionButtonDelete) ui->actionButtonDelete->setEnabled(itemIsSelectedInVisibleList);
    if(ui->actionButtonConsult) ui->actionButtonConsult->setEnabled(itemIsSelectedInVisibleList);
    if(ui->actionButtonViewClaims) ui->actionButtonViewClaims->setEnabled(itemIsSelectedInVisibleList);
    // Validate/Reject button state depends on selection status, handled in onOfferSelectionChanged
    if (!itemIsSelectedInVisibleList) {
        if(ui->actionButtonValidate) ui->actionButtonValidate->setEnabled(false);
        if(ui->actionButtonReject) ui->actionButtonReject->setEnabled(false);
    } else {
        // Re-trigger selection change logic to set validate/reject state correctly
        onOfferSelectionChanged(ui->offersListWidget->currentItem(), nullptr);
    }
}


// Renamed from onShowFiltersClicked
void OffresFlashScreen::onFormShowListClicked() {
    qDebug() << "'Voir la liste des offres' (from form) clicked.";
    if (ui->centralStackedWidget) {
        ui->centralStackedWidget->setCurrentIndex(1); // Switch to listPage (index 1)
        // Ensure list is populated based on current filter
        if (m_currentStatusFilter.isEmpty()) {
            // If filter was somehow cleared, set a default and refresh
            m_currentStatusFilter = "Active"; // Or your preferred default
            qDebug() << "No filter active, setting default filter to:" << m_currentStatusFilter;
            updateFilterTabStyles(); // Update button appearance
            refreshOfferList();
        } else {
            // Filter exists, ensure list is up-to-date (might be redundant if always loaded)
            qDebug() << "Switching to list view with filter:" << m_currentStatusFilter;
            // refreshOfferList(); // Uncomment if list needs explicit refresh on view switch
        }
        // Update button states based on current selection in the (now visible) list
        onOfferSelectionChanged(ui->offersListWidget ? ui->offersListWidget->currentItem() : nullptr, nullptr);
    } else {
        qWarning() << "onFormShowListClicked: ui->centralStackedWidget is null!";
    }
}

// Renamed from onHideFiltersClicked - Now switches TO form view
void OffresFlashScreen::onListShowFormClicked() {
    qDebug() << "'Ajouter une offre' (from list) clicked.";
    resetFormState(); // Reset form before showing it for adding
    if (ui->centralStackedWidget) {
        ui->centralStackedWidget->setCurrentIndex(0); // Switch to formPage (index 0)
    } else {
        qWarning() << "onListShowFormClicked: ui->centralStackedWidget is null!";
    }
    // Disable action bar buttons when form is shown
    if(ui->actionButtonModify) ui->actionButtonModify->setEnabled(false);
    if(ui->actionButtonDelete) ui->actionButtonDelete->setEnabled(false);
    if(ui->actionButtonConsult) ui->actionButtonConsult->setEnabled(false);
    if(ui->actionButtonViewClaims) ui->actionButtonViewClaims->setEnabled(false);
    if(ui->actionButtonValidate) ui->actionButtonValidate->setEnabled(false);
    if(ui->actionButtonReject) ui->actionButtonReject->setEnabled(false);
}

// New Slot for Filter Button Group
void OffresFlashScreen::onFilterGroupButtonClicked(QAbstractButton *button) {
    if (!button) {
        qWarning() << "onFilterGroupButtonClicked: Received null button!";
        return;
    }
    QString newFilter = "";
    // Determine filter based on which button was clicked (using new object names)
    if (button == ui->listFilterActiveButton) newFilter = "Active"; // Use consistent internal names
    else if (button == ui->listFilterEndedButton) newFilter = "Terminee";
    else if (button == ui->listFilterValidatedButton) newFilter = "Validee";
    else if (button == ui->listFilterPendingButton) newFilter = "En Attente";
    else if (button == ui->listFilterAllButton) newFilter = "All";
    else {
        qWarning() << "onFilterGroupButtonClicked: Clicked button" << button->objectName() << "is not a known filter button.";
        return;
    }

    if (m_currentStatusFilter != newFilter) {
        qDebug() << "Filter changed via button group to:" << newFilter;
        m_currentStatusFilter = newFilter;
        updateFilterTabStyles(); // Update dynamic properties for QSS
        refreshOfferList(); // Refresh the list view
    } else {
        qDebug() << "Filter button clicked, but filter (" << newFilter << ") did not change.";
        // Ensure the correct button remains visually checked if clicked again
        button->setChecked(true);
        updateFilterTabStyles();
    }
}

// Updated implementation for search text changes
void OffresFlashScreen::onSearchTextChanged(const QString &text) {
    Q_UNUSED(text); // Text parameter not directly used, just triggers refresh
    // Only refresh if the list view is currently visible
    if (ui->centralStackedWidget && ui->centralStackedWidget->currentIndex() == 1) { // index 1 is listPage
        qDebug() << "Search text changed, refreshing list.";
        refreshOfferList();
    } else {
        qDebug() << "Search text changed, but list view is not active. Ignoring refresh.";
    }
}

// Updated implementation for sort criteria changes
void OffresFlashScreen::onSortCriteriaChanged(int index) {
    qDebug() << "Sort criteria changed, index:" << index;
    SortColumn oldColumn = m_currentSortColumn;
    Qt::SortOrder oldOrder = m_currentSortOrder;

    // Determine new sort criteria based on combobox index
    switch (index) {
    case 0: // "Date (Plus récentes)"
        m_currentSortColumn = SortColumn::ByDate;
        m_currentSortOrder = Qt::DescendingOrder;
        break;
    case 1: // "Date (Plus anciennes)"
        m_currentSortColumn = SortColumn::ByDate;
        m_currentSortOrder = Qt::AscendingOrder;
        break;
    case 2: // "Description (A-Z)"
        m_currentSortColumn = SortColumn::ByDescription;
        m_currentSortOrder = Qt::AscendingOrder;
        break;
    default:
        qWarning() << "Unknown sort index:" << index << ". Defaulting to Date Desc.";
        m_currentSortColumn = SortColumn::ByDate;
        m_currentSortOrder = Qt::DescendingOrder;
        break;
    }

    // Only resort and refresh if criteria actually changed
    if (m_currentSortColumn != oldColumn || m_currentSortOrder != oldOrder) {
        sortOfferList(); // Sort the internal data list
        refreshOfferList(); // Refresh the list widget display
    } else {
        qDebug() << "Sort criteria selected, but it matches current criteria. No resort needed.";
    }
}

// Updated implementation for updating summary boxes
void OffresFlashScreen::updateSummaryBoxes() {
    qDebug() << "Updating summary boxes...";
    int activeCount = 0;
    int validatedCount = 0; // Count offers explicitly marked VALIDATED
    int pendingCount = 0;   // Count offers explicitly marked PENDING
    int totalClaims = 0;    // Sum of claims across all offers

    // Iterate through the master list
    for(const auto& offer : std::as_const(m_offerList)) {
        // Count based on calculated status for active offers
        if (offer.getCurrentStatus() == "En cours") { // Use helper
            activeCount++;
        }
        // Count based on explicit validation status
        if (offer.validationStatus == "VALIDATED") {
            validatedCount++;
        } else if (offer.validationStatus == "PENDING") {
            pendingCount++;
        }
        totalClaims += offer.claimCount; // Sum claims
    }

    // Update UI labels directly (using new object names)
    // Stat Card 1: Active Offers
    if(ui->statValueLabel1) {
        ui->statValueLabel1->setText(QString::number(activeCount));
    } else {
        qWarning() << "updateSummaryBoxes: ui->statValueLabel1 is null!";
    }
    // Title label was already correct
    if(ui->statTitleLabel1) {
        ui->statTitleLabel1->setText("Offres Actives"); // Set title text
    } else {
        qWarning() << "updateSummaryBoxes: ui->statTitleLabel1 is null!";
    }

    // Stat Card 2: Validated Offers
    if(ui->statValueLabel2) {
        ui->statValueLabel2->setText(QString::number(validatedCount));
    } else {
        qWarning() << "updateSummaryBoxes: ui->statValueLabel2 is null!";
    }
    if(ui->statTitleLabel2) {
        ui->statTitleLabel2->setText("Offres Validées");
    } else {
        qWarning() << "updateSummaryBoxes: ui->statTitleLabel2 is null!";
    }

    // Stat Card 3: Pending Offers
    if(ui->statValueLabel3) {
        ui->statValueLabel3->setText(QString::number(pendingCount));
    } else {
        qWarning() << "updateSummaryBoxes: ui->statValueLabel3 is null!";
    }
    if(ui->statTitleLabel3) {
        ui->statTitleLabel3->setText("Offres en Attente");
    } else {
        qWarning() << "updateSummaryBoxes: ui->statTitleLabel3 is null!";
    }

    // Stat Card 4: Total Claims
    if(ui->statValueLabel4) {
        ui->statValueLabel4->setText(QString::number(totalClaims));
    } else {
        qWarning() << "updateSummaryBoxes: ui->statValueLabel4 is null!";
    }
    if(ui->statTitleLabel4) {
        // Corrected potential encoding issue in string literal
        ui->statTitleLabel4->setText(QString::fromUtf8("Réclamations Totales"));
    } else {
        qWarning() << "updateSummaryBoxes: ui->statTitleLabel4 is null!";
    }


    qDebug() << "Summary boxes updated - Active:" << activeCount << "Validated:" << validatedCount << "Pending:" << pendingCount << "Total Claims:" << totalClaims;
}


// Updated implementation for updating filter tab styles (using dynamic properties)
void OffresFlashScreen::updateFilterTabStyles() {
    qDebug() << "Updating filter tab styles property for active filter:" << m_currentStatusFilter;

    // Lambda to set the property on a button
    auto setFilterActiveProperty = [](QPushButton* button, bool isActive) {
        if (button) {
            // Set or remove the dynamic property used by QSS selector [activeFilter="true"]
            button->setProperty("activeFilter", isActive ? QVariant(true) : QVariant()); // Set to true or invalid QVariant
            // Re-polish the widget to ensure the stylesheet is re-evaluated
            button->style()->unpolish(button);
            button->style()->polish(button);
        } else {
            qWarning() << "setFilterActiveProperty: Button is null!";
        }
    };

    // Determine which button corresponds to the current filter
    // Use new object names
    setFilterActiveProperty(ui->listFilterActiveButton, m_currentStatusFilter.compare("Active", Qt::CaseInsensitive) == 0 || m_currentStatusFilter.compare("En cours", Qt::CaseInsensitive) == 0);
    setFilterActiveProperty(ui->listFilterEndedButton, m_currentStatusFilter.compare("Terminee", Qt::CaseInsensitive) == 0);
    setFilterActiveProperty(ui->listFilterValidatedButton, m_currentStatusFilter.compare("Validee", Qt::CaseInsensitive) == 0);
    setFilterActiveProperty(ui->listFilterPendingButton, m_currentStatusFilter.compare("En Attente", Qt::CaseInsensitive) == 0);
    setFilterActiveProperty(ui->listFilterAllButton, m_currentStatusFilter.compare("All", Qt::CaseInsensitive) == 0);

    // Ensure the correct button in the group is also programmatically checked
    if (m_filterButtonGroup) {
        QAbstractButton* checkedButton = m_filterButtonGroup->checkedButton();
        QString checkedFilter = "";
        if (checkedButton == ui->listFilterActiveButton) checkedFilter = "Active";
        else if (checkedButton == ui->listFilterEndedButton) checkedFilter = "Terminee";
        else if (checkedButton == ui->listFilterValidatedButton) checkedFilter = "Validee";
        else if (checkedButton == ui->listFilterPendingButton) checkedFilter = "En Attente";
        else if (checkedButton == ui->listFilterAllButton) checkedFilter = "All";

        // If the button group's checked state doesn't match m_currentStatusFilter, update the button group
        if (checkedFilter.compare(m_currentStatusFilter, Qt::CaseInsensitive) != 0) {
            qDebug() << "Mismatch between m_currentStatusFilter and button group state. Updating button group.";
            if (m_currentStatusFilter.compare("Active", Qt::CaseInsensitive) == 0 && ui->listFilterActiveButton) ui->listFilterActiveButton->setChecked(true);
            else if (m_currentStatusFilter.compare("Terminee", Qt::CaseInsensitive) == 0 && ui->listFilterEndedButton) ui->listFilterEndedButton->setChecked(true);
            else if (m_currentStatusFilter.compare("Validee", Qt::CaseInsensitive) == 0 && ui->listFilterValidatedButton) ui->listFilterValidatedButton->setChecked(true);
            else if (m_currentStatusFilter.compare("En Attente", Qt::CaseInsensitive) == 0 && ui->listFilterPendingButton) ui->listFilterPendingButton->setChecked(true);
            else if (m_currentStatusFilter.compare("All", Qt::CaseInsensitive) == 0 && ui->listFilterAllButton) ui->listFilterAllButton->setChecked(true);
            else if (ui->listFilterActiveButton) ui->listFilterActiveButton->setChecked(true); // Default fallback
        }
    }
}


// --- Menu Action Slots (No changes needed) ---

void OffresFlashScreen::onLogoutActionTriggered() {
    qDebug() << "Menu Action Triggered: Logout";
    QMessageBox::information(this, "Deconnexion", "Fonctionnalite de deconnexion non implementee.");
    // Add actual logout logic here (e.g., emit a signal, close window, show login screen)
}
void OffresFlashScreen::onAboutActionTriggered() {
    qDebug() << "Menu Action Triggered: About";

    // Use the correct resource path for the PrimeEN logo
    // Ensure this path exists in your resources.qrc file
    QString logoResourcePath = ":/images/primeEN_logo.png";

    // Construct the text using correct string formatting
    QString aboutText = QString(
                            "<b>Mallmate - Gestion des Offres Flash</b><br>"
                            "Développé par: Jessem Abbedi<br>" // Updated developer name
                            "Version 1.0 <br><br>" // Updated version
                            "Application de gestion de centre commercial.<br><br>"
                            // Use %1 placeholder correctly for the image source
                            "<img src='%1' width='60' style='vertical-align: middle;' alt='PrimeEN Logo'> " // Updated Alt Text
                            " (C) 2025 PrimeEN" // Updated copyright year and owner
                            )
                            // Pass the logo path to .arg()
                            .arg(logoResourcePath);

    // Display the about box
    QMessageBox::about(this, "A propos de Mallmate", aboutText);
}

void OffresFlashScreen::onExportExcelTriggered() {
    qDebug() << "Export Action Triggered";

    // --- Get Export Settings ---
    // Default to including all columns if settings widget hasn't been created yet
    bool includeId = true;
    bool includeDesc = true;
    bool includePercent = true;
    bool includePrice = true;
    bool includeQty = true;
    bool includeStart = true;
    bool includeEnd = true;
    bool includeStoreId = true;
    bool includeStoreName = true;
    bool includeValidation = true;
    bool includeStatus = true;
    bool includeClaims = true;

    if (m_settingsWidgetInstance) {
        qDebug() << "Using export settings from SettingsWidget instance.";
        includeId = m_settingsWidgetInstance->exportIncludeId();
        includeDesc = m_settingsWidgetInstance->exportIncludeDescription();
        includePercent = m_settingsWidgetInstance->exportIncludePercent();
        includePrice = m_settingsWidgetInstance->exportIncludePrice();
        includeQty = m_settingsWidgetInstance->exportIncludeQuantity();
        includeStart = m_settingsWidgetInstance->exportIncludeStartDate();
        includeEnd = m_settingsWidgetInstance->exportIncludeEndDate();
        includeStoreId = m_settingsWidgetInstance->exportIncludeStoreId();
        includeStoreName = m_settingsWidgetInstance->exportIncludeStoreName();
        includeValidation = m_settingsWidgetInstance->exportIncludeValidationStatus();
        includeStatus = m_settingsWidgetInstance->exportIncludeCalculatedStatus();
        includeClaims = m_settingsWidgetInstance->exportIncludeClaimCount();
    } else {
        qWarning() << "Exporting CSV: SettingsWidget instance not found. Defaulting to exporting all columns.";
        // Optional: Show a message to the user?
        // QMessageBox::information(this, "Exportation CSV", "Les paramètres d'exportation n'ont pas été chargés (page Paramètres non visitée ?). Exportation de toutes les colonnes par défaut.");
    }

    // --- File Dialog ---
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultFileName = defaultPath + "/mallmate_offres_export_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".csv";
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter la liste des offres en CSV",
                                                    defaultFileName,
                                                    "Fichiers CSV (*.csv);;Tous les fichiers (*)");
    if (fileName.isEmpty()) {
        qDebug() << "CSV Export cancelled by user.";
        return;
    }
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv"; // Ensure .csv extension
    }

    // --- Open File ---
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open file for writing:" << fileName << "Error:" << file.errorString();
        QMessageBox::warning(this, "Erreur d'exportation",
                             QString("Impossible d'ouvrir le fichier pour l'ecriture :\n%1\n%2").arg(fileName).arg(file.errorString()));
        return;
    }

    // --- Setup Stream ---
    QTextStream stream(&file);
// Set UTF-8 encoding with BOM for better Excel compatibility
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    stream.setCodec("UTF-8");
#else
    stream.setEncoding(QStringConverter::Utf8);
#endif
    stream.setGenerateByteOrderMark(true);

    qDebug() << "Exporting" << m_offerList.size() << "offers to CSV file:" << fileName;

    // --- Write Data ---
    try {
        // --- Build Header Row Dynamically ---
        QStringList headerList;
        if (includeId) headerList << "\"OffreID\"";
        if (includeDesc) headerList << "\"Description\"";
        if (includePercent) headerList << "\"PourcentageReduction\"";
        if (includePrice) headerList << "\"PrixSpecial\"";
        if (includeQty) headerList << "\"QuantiteLimitee\"";
        if (includeStart) headerList << "\"DateDebut\"";
        if (includeEnd) headerList << "\"DateFin\"";
        if (includeStoreId) headerList << "\"MagasinID\"";
        if (includeStoreName) headerList << "\"NomMagasin\"";
        if (includeValidation) headerList << "\"StatutValidation\"";
        if (includeStatus) headerList << "\"StatutCalcule\"";
        if (includeClaims) headerList << "\"ClaimCount\"";

        if (headerList.isEmpty()) {
            qWarning() << "CSV Export Error: No columns selected for export!";
            QMessageBox::warning(this, "Erreur d'exportation", "Aucune colonne n'a été sélectionnée pour l'exportation dans les paramètres.");
            file.close();
            return;
        }
        stream << headerList.join(',') << Qt::endl; // Write header


        // --- Helper Lambdas (Keep these) ---
        auto quoteCsvString = [](const QString& input) -> QString {
            QString output = input;
            output.replace("\"", "\"\""); // Escape double quotes
            return "\"" + output + "\"";
        };
        auto formatCsvNumeric = [](const QVariant& input, int precision = -1) -> QString {
            if (input.isNull() || !input.isValid()) return "";
            bool ok; double dblVal = input.toDouble(&ok);
            if (ok) return QString::number(dblVal, 'f', precision >= 0 ? precision : 10);
            return input.toString(); // Fallback
        };
        // --- End Helper Lambdas ---


        // --- Write Data Rows Dynamically ---
        for (const OfferData &offer : std::as_const(m_offerList)) {
            QStringList dataList; // List to hold data for the current row

            // Get store name (needed if included)
            QString storeNameExport = "";
            if (includeStoreName) { // Only look up if needed
                storeNameExport = "ID: " + QString::number(offer.magasinId); // Fallback
                if(ui->formStoreComboBox) {
                    int storeIndex = ui->formStoreComboBox->findData(offer.magasinId);
                    if (storeIndex > -1) {
                        storeNameExport = ui->formStoreComboBox->itemText(storeIndex);
                    }
                }
            }

            // Conditionally add data to the list based on settings
            if (includeId) dataList << QString::number(offer.offreId);
            if (includeDesc) dataList << quoteCsvString(offer.description);
            if (includePercent) dataList << formatCsvNumeric(offer.percentageReduction > 0 ? QVariant(offer.percentageReduction) : QVariant(QMetaType(QMetaType::Double)), 1);
            if (includePrice) dataList << formatCsvNumeric(offer.prixSpecial > 0 ? QVariant(offer.prixSpecial) : QVariant(QMetaType(QMetaType::Double)), 2);
            if (includeQty) dataList << formatCsvNumeric(offer.quantiteLimitee > 0 ? QVariant(offer.quantiteLimitee) : QVariant(QMetaType(QMetaType::Int)), 0);
            if (includeStart) dataList << quoteCsvString(offer.startDateTime.toString(Qt::ISODate));
            if (includeEnd) dataList << quoteCsvString(offer.endDateTime.toString(Qt::ISODate));
            if (includeStoreId) dataList << QString::number(offer.magasinId);
            if (includeStoreName) dataList << quoteCsvString(storeNameExport);
            if (includeValidation) dataList << quoteCsvString(offer.validationStatus);
            if (includeStatus) dataList << quoteCsvString(offer.getCurrentStatus());
            if (includeClaims) dataList << QString::number(offer.claimCount);

            // Write the row by joining the list elements
            stream << dataList.join(',') << Qt::endl;
        }

        file.close(); // Ensure file is closed
        qDebug() << "CSV Export finished successfully.";
        QMessageBox::information(this, "Exportation Reussie",
                                 QString("La liste des offres a ete exportee avec succes vers :\n%1").arg(QDir::toNativeSeparators(fileName)));

    } catch (const std::exception& e) {
        qWarning() << "Standard exception during CSV export:" << e.what();
        QMessageBox::critical(this, "Erreur d'exportation",
                              QString("Une erreur standard est survenue lors de l'ecriture du fichier CSV:\n%1").arg(e.what()));
        file.close(); // Ensure file is closed on error too
    } catch (...) {
        qWarning() << "Unknown exception during CSV export.";
        QMessageBox::critical(this, "Erreur d'exportation",
                              "Une erreur inconnue est survenue lors de l'ecriture du fichier CSV.");
        file.close(); // Ensure file is closed on error too
    }
}

// --- Claims and Validation Slots (Updated for new object names) ---
void OffresFlashScreen::onViewClaimsClicked() {
    qDebug() << "View Claims button clicked!";
    if (!ui->offersListWidget) { /* ... null check ... */ return; }
    QListWidgetItem *selectedItem = ui->offersListWidget->currentItem();
    if (!selectedItem) { /* ... no selection message ... */ return; }
    bool conversionOk;
    int offerIdToView = selectedItem->data(Qt::UserRole).toInt(&conversionOk); // ID from UserRole
    if (!conversionOk || offerIdToView <= 0) { /* ... invalid ID message ... */ return; }

    // Find offer in list to get claim count and description
    const OfferData* selectedOfferData = nullptr;
    for (const auto& offer : std::as_const(m_offerList)) {
        if (offer.offreId == offerIdToView) {
            selectedOfferData = &offer;
            break;
        }
    }
    if (selectedOfferData) {
        qDebug() << "Offer found: ID" << selectedOfferData->offreId << "Claims:" << selectedOfferData->claimCount;
        QString message = QString("L'offre \"%1\" a été réclamée %2 fois.")
                              .arg(selectedOfferData->description) // Use description from OfferData
                              .arg(selectedOfferData->claimCount);
        QMessageBox::information(this, "Nombre de Réclamations", message);
    } else {
        qWarning() << "onViewClaimsClicked: Could not find offer ID" << offerIdToView << "in m_offerList.";
        QMessageBox::warning(this, "Erreur", "Impossible de trouver les données pour l'offre sélectionnée (ID: " + QString::number(offerIdToView) + ").");
    }
}

void OffresFlashScreen::onValidateOfferClicked() {
    qDebug() << "Validate button clicked.";
    if (!ui->offersListWidget) return;
    QListWidgetItem *selectedItem = ui->offersListWidget->currentItem();
    if (!selectedItem) { /* ... no selection message ... */ return; }
    bool conversionOk;
    int offerIdToValidate = selectedItem->data(Qt::UserRole).toInt(&conversionOk);
    if (!conversionOk || offerIdToValidate <= 0) { /* ... invalid ID message ... */ return; }

    // Optional: Check if the offer status is actually PENDING before proceeding
    bool isPending = false;
    for(const auto& offer : std::as_const(m_offerList)) {
        if (offer.offreId == offerIdToValidate) {
            isPending = (offer.validationStatus == "PENDING");
            break;
        }
    }
    if (!isPending) {
        QMessageBox::information(this, "Action Impossible", "Seules les offres 'En Attente' peuvent être validées.");
        return;
    }

    // Confirm Action
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation Validation",
                                  QString("Voulez-vous vraiment marquer l'offre ID %1 comme 'VALIDATED' ?").arg(offerIdToValidate),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::No) { /* ... cancel log ... */ return; }

    // Execute Database Update
    if (!m_database.isOpen() && !initializeDatabaseConnection()) { /* ... DB error message ... */ return; }
    QSqlQuery query(m_database);
    // ** IMPORTANT: Replace 'STATUT_VALIDATION' with your actual column name **
    QString updateSql = "UPDATE OFFERS SET STATUT_VALIDATION = :status WHERE OffreID = :id";
    if (!query.prepare(updateSql)) { /* ... prepare error message ... */ return; }
    query.bindValue(":status", "VALIDATED");
    query.bindValue(":id", offerIdToValidate);
    qDebug() << "Executing Query:" << updateSql << "with ID:" << offerIdToValidate << "Status: VALIDATED";
    if (!query.exec()) {
        /* ... exec error message ... */
        qWarning() << "Failed to validate offer in database:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur DB", QString("Impossible de valider l'offre.\nError: %1").arg(query.lastError().text()));
    } else {
        qDebug() << "Successfully validated offer" << offerIdToValidate << "in database.";
        QMessageBox::information(this, "Succès", QString("Offre ID %1 validée avec succès.").arg(offerIdToValidate));
        loadInitialData(); // Reload data to reflect status change
    }
}

void OffresFlashScreen::onRejectOfferClicked() {
    qDebug() << "Reject button clicked.";
    if (!ui->offersListWidget) return;
    QListWidgetItem *selectedItem = ui->offersListWidget->currentItem();
    if (!selectedItem) { /* ... no selection message ... */ return; }
    bool conversionOk;
    int offerIdToReject = selectedItem->data(Qt::UserRole).toInt(&conversionOk);
    if (!conversionOk || offerIdToReject <= 0) { /* ... invalid ID message ... */ return; }

    // Optional: Check if the offer status is actually PENDING before proceeding
    bool isPending = false;
    for(const auto& offer : std::as_const(m_offerList)) {
        if (offer.offreId == offerIdToReject) {
            isPending = (offer.validationStatus == "PENDING");
            break;
        }
    }
    if (!isPending) {
        QMessageBox::information(this, "Action Impossible", "Seules les offres 'En Attente' peuvent être rejetées.");
        return;
    }

    // Confirm Action
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation Rejet",
                                  QString("Voulez-vous vraiment marquer l'offre ID %1 comme 'REJECTED' ?").arg(offerIdToReject),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::No) { /* ... cancel log ... */ return; }

    // Execute Database Update
    if (!m_database.isOpen() && !initializeDatabaseConnection()) { /* ... DB error message ... */ return; }
    QSqlQuery query(m_database);
    // ** IMPORTANT: Replace 'STATUT_VALIDATION' with your actual column name **
    QString updateSql = "UPDATE OFFERS SET STATUT_VALIDATION = :status WHERE OffreID = :id";
    if (!query.prepare(updateSql)) { /* ... prepare error message ... */ return; }
    query.bindValue(":status", "REJECTED");
    query.bindValue(":id", offerIdToReject);
    qDebug() << "Executing Query:" << updateSql << "with ID:" << offerIdToReject << "Status: REJECTED";
    if (!query.exec()) {
        /* ... exec error message ... */
        qWarning() << "Failed to reject offer in database:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur DB", QString("Impossible de rejeter l'offre.\nError: %1").arg(query.lastError().text()));
    } else {
        qDebug() << "Successfully rejected offer" << offerIdToReject << "in database.";
        QMessageBox::information(this, "Succès", QString("Offre ID %1 rejetée avec succès.").arg(offerIdToReject));
        loadInitialData(); // Reload data to reflect status change
    }
}


// --- Slot Implementations for AI Features (Suggest & Fill) ---



void OffresFlashScreen::onSidebarSettingsClicked() {
    qDebug() << "Sidebar button clicked: Settings";

    // 1. Check if the main stacked widget and the target page exist
    if (!ui->contentStackedWidget || !ui->settingsPage) {
        qWarning() << "Cannot switch to Settings page: contentStackedWidget or settingsPage is null! Check UI file.";
        return;
    }

    // 2. Check if the SettingsWidget instance has already been created
    // Use the correct member variable name: m_settingsWidgetInstance
    if (!m_settingsWidgetInstance) {
        qDebug() << "Creating SettingsWidget instance for the first time.";

        // 3. Prepare the settingsPage layout
        QVBoxLayout *settingsLayout = qobject_cast<QVBoxLayout*>(ui->settingsPage->layout());
        if (!settingsLayout) {
            // If no layout exists on the settingsPage widget in the UI file, create one
            qDebug() << "Creating new QVBoxLayout for settingsPage.";
            settingsLayout = new QVBoxLayout(ui->settingsPage);
            ui->settingsPage->setLayout(settingsLayout);
            settingsLayout->setContentsMargins(0, 0, 0, 0); // Remove layout margins so widget fills page
        }

        // 4. Clear placeholder content (if any) from the settingsPage layout
        QLayoutItem* item;
        while ((item = settingsLayout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                qDebug() << "Deleting previous placeholder widget from settingsPage:" << widget->objectName();
                delete widget; // Delete the placeholder QLabel etc.
            }
            delete item;
        }

        // 5. Create the SettingsWidget instance using the correct class name
        // Store the pointer in the member variable m_settingsWidgetInstance
        m_settingsWidgetInstance = new SettingsWidget(ui->settingsPage); // Parent is the settingsPage

        // 6. Connect signals from SettingsWidget to handler slots in OffresFlashScreen
        // Connect ONLY the Database test signal
        // Ensure handleDatabaseTestRequest slot exists and is declared
        connect(m_settingsWidgetInstance, &SettingsWidget::testDatabaseConnectionRequested,
                this, &OffresFlashScreen::handleDatabaseTestRequest);
        // Removed connection for testApiKeyRequested as the API section was removed from SettingsWidget UI

        // 7. Add the SettingsWidget instance to the layout
        settingsLayout->addWidget(m_settingsWidgetInstance);
        qDebug() << "Created and added new SettingsWidget instance to settingsPage.";

    } else {
        qDebug() << "SettingsWidget instance already exists.";
        // Optional: If settings could change elsewhere or need refreshing,
        // you might want to tell the existing widget to reload its display from QSettings.
        // m_settingsWidgetInstance->loadSettings();
    }

    // 8. Switch the main stacked widget to show the settings page
    ui->contentStackedWidget->setCurrentWidget(ui->settingsPage);
    qDebug() << "Switched contentStackedWidget to settingsPage.";

    // 9. Note on Applying Settings:
    // You need a separate mechanism (e.g., an Apply button outside SettingsWidget)
    // to call m_settingsWidgetInstance->applySettings() to save changes.
}
void OffresFlashScreen::onSidebarExportClicked() {
    qDebug() << "Sidebar Button Clicked: Export";
    // This button is not checkable. Call the existing export function.
    onExportExcelTriggered();
    // No need to change check state of sidebar buttons for this action.
}

void OffresFlashScreen::onSidebarAboutClicked() {
    qDebug() << "Sidebar Button Clicked: About";
    // This button is not checkable. Call the existing about function.
    onAboutActionTriggered();
    // No need to change check state of sidebar buttons for this action.
}

void OffresFlashScreen::sortOfferList() {
    qDebug() << "Sorting m_offerList. Column:" << m_currentSortColumn << "Order:" << m_currentSortOrder;

    // Define a lambda function for comparison
    auto compareOffers = [this](const OfferData &a, const OfferData &b) {
        bool result = false; // Default comparison result

        // Compare based on the selected column
        if (m_currentSortColumn == SortColumn::ByDate) {
            // Compare by start date/time
            result = a.startDateTime < b.startDateTime;
        }
        else if (m_currentSortColumn == SortColumn::ByDescription) {
            // Use locale-aware comparison for strings (handles accents, etc.)
            result = QString::localeAwareCompare(a.description, b.description) < 0;
        }
        // Add more 'else if' blocks here for other sortable columns if needed

        // Apply the selected sort order (Ascending or Descending)
        return (m_currentSortOrder == Qt::AscendingOrder) ? result : !result;
    };

    // Use std::sort with the custom comparison lambda
    std::sort(m_offerList.begin(), m_offerList.end(), compareOffers);

    qDebug() << "m_offerList sorted.";
}
void OffresFlashScreen::onSidebarOffersClicked() {
    qDebug() << "Sidebar Button Clicked: Offers";
    // Switch the main content area's stacked widget to the page containing offer management
    // Ensure the object names 'contentStackedWidget' and 'offreManagementPage'
    // match exactly what's in your offresflashscreen.ui file.
    if (ui->contentStackedWidget && ui->offreManagementPage) {
        ui->contentStackedWidget->setCurrentWidget(ui->offreManagementPage);
    } else {
        qWarning() << "onSidebarOffersClicked: ui->contentStackedWidget or ui->offreManagementPage is null! Check UI file.";
    }
    // The button group handles the visual check state
}
void OffresFlashScreen::onQuitTriggered() {
    qDebug() << "Menu Action Triggered: Quit";
    // Ask for confirmation before quitting
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Quitter Mallmate",
                                  "Etes-vous sur de vouloir quitter l'application?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No); // Default to No
    if (reply == QMessageBox::Yes) {
        qDebug() << "User confirmed quit. Quitting application...";
        QApplication::quit(); // Use QApplication::quit() to exit the application
    } else {
        qDebug() << "User cancelled quit.";
    }
}
void OffresFlashScreen::onSidebarStatsClicked() {
    qDebug() << "Sidebar button clicked: Stats";

    // 1. Check if the main stacked widget and the target page exist
    if (!ui->contentStackedWidget || !ui->statsPage) {
        qWarning() << "Cannot switch to Stats page: contentStackedWidget or statsPage is null! Check UI file.";
        // Optionally switch back to offers page if stats page is broken
        // if (ui->offreManagementPage) ui->contentStackedWidget->setCurrentWidget(ui->offreManagementPage);
        // if (ui->sidebarButtonOffers) ui->sidebarButtonOffers->setChecked(true);
        return;
    }

    // 2. Calculate all necessary counts from the current m_offerList
    int activeCount = 0;
    int finishedCount = 0;
    int validatedCount = 0;
    int pendingCount = 0;
    int rejectedCount = 0;
    int totalClaims = 0;

    for(const OfferData& offer : std::as_const(m_offerList)) {
        QString currentStatus = offer.getCurrentStatus(); // Use the helper function

        // Count based on calculated current status
        if (currentStatus.compare("En cours", Qt::CaseInsensitive) == 0) {
            activeCount++;
        } else if (currentStatus.compare("Terminee", Qt::CaseInsensitive) == 0) {
            finishedCount++;
        }

        // Count based on explicit validation status stored in the OfferData object
        if (offer.validationStatus.compare("VALIDATED", Qt::CaseInsensitive) == 0) {
            validatedCount++;
        } else if (offer.validationStatus.compare("PENDING", Qt::CaseInsensitive) == 0) {
            pendingCount++;
        } else if (offer.validationStatus.compare("REJECTED", Qt::CaseInsensitive) == 0) {
            rejectedCount++;
        }

        // Sum total claims
        totalClaims += offer.claimCount;
    }

    qDebug() << "Calculated counts for StatsWidget - Active:" << activeCount
             << "Finished:" << finishedCount << "Validated:" << validatedCount
             << "Pending:" << pendingCount << "Rejected:" << rejectedCount
             << "Total Claims:" << totalClaims;


    // 3. Prepare the statsPage layout
    QVBoxLayout *statsLayout = qobject_cast<QVBoxLayout*>(ui->statsPage->layout());
    if (!statsLayout) {
        // If no layout exists on the statsPage widget in the UI file, create one
        qDebug() << "Creating new QVBoxLayout for statsPage.";
        statsLayout = new QVBoxLayout(ui->statsPage);
        ui->statsPage->setLayout(statsLayout);
        // Optional: Remove margins if the widget should fill the page
        statsLayout->setContentsMargins(0, 0, 0, 0);
    }

    // 4. Clear previous content from the statsPage layout
    // This removes any old StatsWidget instance or placeholder label
    QLayoutItem* item;
    while ((item = statsLayout->takeAt(0)) != nullptr) {
        // Delete the widget associated with the layout item
        if (QWidget* widget = item->widget()) {
            qDebug() << "Deleting previous widget from statsPage:" << widget->objectName();
            delete widget;
        }
        // Delete the layout item itself
        delete item;
    }
    qDebug() << "Cleared previous content from statsPage layout.";


    // 5. Create and add the new StatsWidget instance
    // Pass all the calculated counts to the enhanced constructor
    StatsWidget *statsWidgetInstance = new StatsWidget(
        activeCount, finishedCount, validatedCount,
        pendingCount, rejectedCount, totalClaims,
        ui->statsPage // Set statsPage as parent for proper memory management
        );
    statsLayout->addWidget(statsWidgetInstance); // Add the widget to the layout
    qDebug() << "Created and added new StatsWidget instance to statsPage.";


    // 6. Switch the main stacked widget to show the stats page
    ui->contentStackedWidget->setCurrentWidget(ui->statsPage);
    qDebug() << "Switched contentStackedWidget to statsPage.";
}
QList<OfferData> OffresFlashScreen::generateStockBasedSuggestions()
{
    qDebug() << "Generating stock-based offer suggestions (using store type)...";
    QList<OfferData> suggestionsList;

    // 1. Check Database Connection
    if (!m_database.isOpen()) {
        qWarning() << "generateStockBasedSuggestions: Database is not open.";
        if (!initializeDatabaseConnection()) {
            QMessageBox::critical(this, "Erreur DB", "Connexion impossible pour générer les suggestions.");
            return suggestionsList;
        }
    }

    // 2. Load Settings
    QSettings settings;
    int stockThreshold = settings.value("OfferSettings/suggestionStockThreshold", 100).toInt();
    int defaultDurationDays = settings.value("OfferSettings/defaultDuration", 7).toInt();
    double defaultDiscountPercent = settings.value("OfferSettings/defaultSuggestionDiscount", 15.0).toDouble();
    qDebug() << "Using settings - Threshold:" << stockThreshold << "Duration:" << defaultDurationDays << "Default Discount:" << defaultDiscountPercent;

    // 3. Prepare SQL Query (Join with MAGASINS to get TYPE)
    QSqlQuery query(m_database);
    // Query now joins with MAGASINS and selects m.TYPE
    QString sql = R"(
        SELECT ARTICLEID, NOMARTICLE, QUANTITE, MAGASINID, STORETYPE
        FROM (
            SELECT /*+ FIRST_ROWS(5) */
                   a.ARTICLEID, a.NOMARTICLE, a.QUANTITE, a.MAGASINID, m.TYPE as STORETYPE -- <<< Get Store Type
            FROM ARTICLESENSTOCK a
            JOIN MAGASINS m ON a.MAGASINID = m.MAGASINID -- <<< JOIN with MAGASINS
            WHERE a.QUANTITE > :threshold
            AND NOT EXISTS (
                SELECT 1
                FROM OFFERS o
                WHERE o.ARTICLEID = a.ARTICLEID
                  AND o.MAGASINID = a.MAGASINID
                  AND o.STATUT_VALIDATION IN ('PENDING', 'VALIDATED')
                  AND o.DATEFIN >= SYSTIMESTAMP
            )
            ORDER BY a.QUANTITE DESC
        ) articles_filtered
        WHERE ROWNUM <= 5
    )";

    if (!query.prepare(sql)) {
        qWarning() << "Failed to prepare suggestion query (Join version):" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Requête", "Impossible de préparer la requête pour les suggestions (join):\n" + query.lastError().text());
        return suggestionsList;
    }

    query.bindValue(":threshold", stockThreshold);

    // 4. Execute Query
    if (!query.exec()) {
        qWarning() << "Failed to execute suggestion query (Join version):" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Requête", "Impossible d'exécuter la requête pour les suggestions (join):\n" + query.lastError().text());
        return suggestionsList;
    }

    // 5. Process Results & Create Suggestions
    qDebug() << "Processing suggestion query results (with store type)...";
    QDateTime now = QDateTime::currentDateTime();

    while (query.next()) {
        OfferData suggestion;
        suggestion.offreId = -1;
        int articleId = query.value("ARTICLEID").toInt();
        QString articleName = query.value("NOMARTICLE").toString();
        suggestion.magasinId = query.value("MAGASINID").toInt();
        QString storeType = query.value("STORETYPE").toString(); // <<< Get store type from query
        suggestion.articleId = articleId;

        suggestion.description = QStringLiteral("Suggestion IA: Offre sur ") + articleName;

        // <<< --- SET DISCOUNT BASED ON STORE TYPE --- >>>
        double suggestedDiscount = defaultDiscountPercent; // Start with default
        // Use case-insensitive comparison for store types
        if (storeType.compare("Vêtements", Qt::CaseInsensitive) == 0) {
            suggestedDiscount = 20.0; // Higher discount for clothing
        } else if (storeType.compare("Supermarché", Qt::CaseInsensitive) == 0) {
            suggestedDiscount = 10.0; // Lower discount for supermarket
        } else if (storeType.compare("Café", Qt::CaseInsensitive) == 0) {
            suggestedDiscount = 5.0; // Even lower for cafe (or maybe suggest fixed price later)
        } // Add more else if blocks for other store types as needed

        suggestion.percentageReduction = suggestedDiscount;
        suggestion.prixSpecial = 0.0; // Still default to percentage
        // <<< --- END SET DISCOUNT --- >>>

        suggestion.quantiteLimitee = 0;
        suggestion.startDateTime = now;
        suggestion.endDateTime = now.addDays(defaultDurationDays);
        suggestion.validationStatus = "PENDING";

        suggestionsList.append(suggestion);

        qDebug() << "Generated suggestion - ArticleID:" << suggestion.articleId
                 << "Name:" << articleName
                 << "Store:" << suggestion.magasinId
                 << "Type:" << storeType // Log store type
                 << "Discount:" << suggestion.percentageReduction << "%" // Log suggested discount
                 << "Start:" << suggestion.startDateTime.toString(Qt::ISODate)
                 << "End:" << suggestion.endDateTime.toString(Qt::ISODate);
    }

    qDebug() << "Generated" << suggestionsList.count() << "suggestions.";
    return suggestionsList;
}

void OffresFlashScreen::onGenerateSuggestionsClicked()
{
    qDebug() << "Generate Suggestions button clicked!";

    // Optional: Provide user feedback that process is starting
    // QApplication::setOverrideCursor(Qt::WaitCursor); // Show busy cursor
    // if(ui->generateSuggestionsButton) ui->generateSuggestionsButton->setEnabled(false);

    // 1. Call the helper function to get suggestions
    QList<OfferData> newSuggestions = generateStockBasedSuggestions();

    // Optional: Restore cursor/button now that analysis is done
    // if(ui->generateSuggestionsButton) ui->generateSuggestionsButton->setEnabled(true);
    // QApplication::restoreOverrideCursor();

    // 2. Check if any suggestions were returned
    if (newSuggestions.isEmpty()) {
        QMessageBox::information(this, "Suggestions IA", "Aucune nouvelle suggestion d'offre basée sur le stock actuel n'a été générée.");
        return; // Nothing more to do
    }

    // 3. Add the new suggestions to the main list
    qDebug() << "Adding" << newSuggestions.count() << "new suggestions to m_offerList.";
    m_offerList.append(newSuggestions); // Append the list of new suggestions

    // 4. Re-sort the main list to include new suggestions correctly
    sortOfferList();

    // 5. Refresh the UI list widget to display the updated list
    refreshOfferList();

    // 6. Update the summary boxes (counts might change if suggestions are 'Pending')
    updateSummaryBoxes();

    // 7. Inform the user
    QMessageBox::information(this, "Suggestions Générées",
                             QString("%1 suggestions basées sur le stock ont été ajoutées à la liste (Statut: En Attente). "
                                     "Vous pouvez les consulter, les modifier et les valider.")
                                 .arg(newSuggestions.count()));
}
void OffresFlashScreen::loadArticlesIntoComboBox() {
    // Use the correct object name from the UI file
    if (!ui->formArticleComboBox) {
        qWarning() << "loadArticlesIntoComboBox: ui->formArticleComboBox is null! Check UI file.";
        return;
    }
    if (!m_database.isOpen()) {
        qWarning() << "loadArticlesIntoComboBox: Database is not open.";
        // Optional: Try to reconnect or show error
        // if (!initializeDatabaseConnection()) return;
        return;
    }

    QSqlQuery query(m_database);
    // Query the ARTICLESENSTOCK table
    QString sql = "SELECT ARTICLEID, NOMARTICLE FROM ARTICLESENSTOCK ORDER BY NOMARTICLE ASC";
    qDebug() << "Executing SQL query:" << sql;

    if (!query.exec(sql)) {
        qWarning() << "Failed to execute SELECT query on ARTICLESENSTOCK table:" << query.lastError().text();
        QMessageBox::warning(this, "Database Error", "Could not load articles: " + query.lastError().text());
        return;
    }

    // Clear previous items except the placeholder
    int currentCount = ui->formArticleComboBox->count();
    for (int i = currentCount - 1; i > 0; --i) { // Start from end, stop before placeholder at index 0
        ui->formArticleComboBox->removeItem(i);
    }
    // Ensure placeholder exists ("Choisir un article...")
    if(ui->formArticleComboBox->count() == 0 || ui->formArticleComboBox->itemData(0).toInt() != -1) {
        ui->formArticleComboBox->clear(); // Clear completely just in case
        ui->formArticleComboBox->addItem("Choisir un article...", -1); // Add placeholder with data -1
    }


    int count = 0;
    while (query.next()) {
        int articleId = query.value("ARTICLEID").toInt();
        QString nomArticle = query.value("NOMARTICLE").toString();
        if (articleId > 0 && !nomArticle.isEmpty()) {
            // Add item text (article name) and store Article ID as data
            ui->formArticleComboBox->addItem(nomArticle, articleId);
            count++;
        }
    }
    qDebug() << "Loaded" << count << "articles into ComboBox.";
}
void OffresFlashScreen::onSuggestDescriptionClicked()
{
    qDebug() << "Suggest Description button (✨) clicked!";
    if (!m_networkManager) {
        qWarning() << "Network manager not initialized!";
        QMessageBox::critical(this, "Erreur Interne", "Le gestionnaire réseau n'est pas initialisé.");
        return;
    }

    // --- 1. Get relevant context from form ---
    QString discount = ui->formDiscountEdit ? ui->formDiscountEdit->text().trimmed() : "";
    QString storeName = ui->formStoreComboBox ? ui->formStoreComboBox->currentText() : "";
    QString articleName = ui->formArticleComboBox ? ui->formArticleComboBox->currentText() : ""; // <<< GET ARTICLE NAME
    QString currentDesc = ui->formDescriptionEdit ? ui->formDescriptionEdit->text().trimmed() : "";

    // Improve context requirement - need at least article or discount
    if (discount.isEmpty() && (articleName.isEmpty() || articleName == "Choisir un article...")) {
        QMessageBox::information(this, "Suggestion Description (IA)", "Veuillez entrer une réduction/prix ou sélectionner un article pour obtenir des suggestions de description.");
        return;
    }

    // --- 2. Construct Prompt for Gemini ---
    QString prompt = "Generate 3 short, catchy flash offer descriptions (under 15 words each) for a mall promotion in Monastir, Tunisia. ";

    // Add Article context if available
    if (!articleName.isEmpty() && articleName != "Choisir un article...") {
        prompt += QString("The offer is for the product '%1'. ").arg(articleName);
    } else if (!currentDesc.isEmpty()) {
        // Use current description as fallback context if no article selected
        prompt += QString("The current description is '%1'. Improve it or suggest alternatives related to it. ").arg(currentDesc);
    }

    // Add Discount context
    if (!discount.isEmpty()){
        prompt += QString("The offer involves a discount/price of '%1'").arg(discount);
    }

    // Add Store context if available
    if (!storeName.isEmpty() && storeName != "Choisir un magasin...") {
        prompt += QString(" at the store '%1'.").arg(storeName);
    } else {
        prompt += "."; // End sentence if no store
    }

    prompt += " Keep descriptions concise and appealing to shoppers. Separate suggestions with newlines (use \\n between suggestions).";

    qDebug() << "Prompt for Gemini (Description Suggestion):" << prompt;

    // --- 3. Construct JSON payload for Gemini API ---
    QJsonObject textPart;
    textPart["text"] = prompt;
    QJsonObject contentPart;
    contentPart["parts"] = QJsonArray({textPart});
    QJsonObject requestBodyJson;
    requestBodyJson["contents"] = QJsonArray({contentPart});

    QJsonObject generationConfig;
    generationConfig["temperature"] = 0.8; // Higher temperature for creative descriptions
    generationConfig["maxOutputTokens"] = 150; // Enough for a few short descriptions
    requestBodyJson["generationConfig"] = generationConfig;

    QJsonDocument jsonDoc(requestBodyJson);
    QByteArray jsonData = jsonDoc.toJson();

    // --- 4. Create QNetworkRequest ---
    QUrl apiUrl("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent");
    // Use the API Key provided by the user
    QString apiKey = "AIzaSyANlGnxxmRXmbLlzz9LAeKoPWxNTC2RV48"; // <<< USER PROVIDED API KEY
    if (apiKey.startsWith("YOUR_") || apiKey.isEmpty() || apiKey.length() < 20) { // Basic check
        QMessageBox::critical(this, "Erreur API Key", "La clé API Gemini n'est pas configurée correctement ou est invalide.");
        // Consider loading from settings if implemented there later
        return;
    }
    QUrlQuery query(apiUrl);
    query.addQueryItem("key", apiKey);
    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // --- 5. Call API and Connect Reply ---
    qDebug() << "Sending request to Gemini API (Description Suggestion):" << apiUrl.toString();
    QNetworkReply *reply = m_networkManager->post(request, jsonData);

    // Connect the reply's finished signal to the specific handler for descriptions
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onGeminiDescriptionReplyFinished(reply);
    });

    // --- 6. Disable button (optional: change icon/text to indicate loading) ---
    if(ui->formSuggestDescButton) {
        ui->formSuggestDescButton->setEnabled(false);
        // Consider adding a visual cue like changing the icon slightly or tooltip
    }
}

/**
 * @brief Slot triggered when the Gemini API reply for description suggestions is finished.
 * Parses the response, extracts suggestions, shows the SuggestionDialog, and updates the form if accepted.
 * @param reply Pointer to the network reply object.
 */
void OffresFlashScreen::onGeminiDescriptionReplyFinished(QNetworkReply *reply)
{
    qDebug() << "Received reply from Gemini Description API.";
    // Re-enable the button
    if(ui->formSuggestDescButton) {
        ui->formSuggestDescButton->setEnabled(true);
    }

    if (!reply) { qWarning() << "Received null reply object in description slot."; return; }
    reply->deleteLater(); // Ensure reply is deleted

    // --- 1. Check for network errors ---
    QNetworkReply::NetworkError netError = reply->error();
    if (netError != QNetworkReply::NoError) {
        qWarning() << "Network Error (Description Suggestion):" << netError << reply->errorString();
        QByteArray errorData = reply->readAll();
        qWarning() << "Error Body:" << errorData;
        QString specificError = reply->errorString();
        QJsonDocument errorDoc = QJsonDocument::fromJson(errorData);
        if (!errorDoc.isNull() && errorDoc.isObject() && errorDoc.object().contains("error")) {
            QJsonObject errorObj = errorDoc.object()["error"].toObject();
            specificError = errorObj.value("message").toString(specificError);
        }
        QMessageBox::warning(this, "Erreur Réseau (Description IA)",
                             QString("Erreur API Gemini (Description):\n%1\n\nDétails: %2").arg(reply->errorString()).arg(specificError));
        return;
    }

    // --- 2. Read response data ---
    QByteArray responseData = reply->readAll();
    qDebug() << "Gemini Description Response Raw:" << responseData;

    // --- 3. Parse JSON response ---
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Failed to parse description JSON response:" << parseError.errorString();
        QMessageBox::warning(this, "Erreur API (Description IA)", "Réponse invalide (non-JSON) reçue pour les suggestions de description.");
        return;
    }
    QJsonObject jsonObject = jsonDoc.object();
    qDebug() << "Gemini Description Response JSON:" << jsonObject;

    // Check for API errors in the response body
    if (jsonObject.contains("error") && jsonObject["error"].isObject()) {
        QJsonObject errorObj = jsonObject["error"].toObject();
        QString errorMsg = errorObj.value("message").toString("Unknown API error");
        qWarning() << "Gemini API Error (Description):" << errorMsg;
        QMessageBox::warning(this, "Erreur API Gemini", "L'API Gemini a retourné une erreur:\n" + errorMsg);
        return;
    }

    // --- 4. Extract suggested description(s) ---
    QStringList suggestions;
    QString fullText; // Store the full extracted text

    // Navigate the response structure
    if (jsonObject.contains("candidates") && jsonObject["candidates"].isArray()) {
        QJsonArray candidates = jsonObject["candidates"].toArray();
        if (!candidates.isEmpty() && candidates[0].isObject()) {
            QJsonObject candidate = candidates[0].toObject();
            QString finishReason = candidate.value("finishReason").toString("");
            if (!finishReason.isEmpty() && finishReason != "STOP") {
                qWarning() << "Gemini description generation finished with reason:" << finishReason;
                QString userMessage = "La génération de suggestion de description a été arrêtée.";
                if (finishReason.contains("SAFETY")) userMessage = "Suggestion bloquée par les filtres de sécurité.";
                else if (finishReason.contains("MAX_TOKENS")) userMessage = "La réponse de la suggestion était trop longue.";
                QMessageBox::warning(this, "Suggestion Bloquée (Description IA)", userMessage);
                return;
            }
            if (candidate.contains("content") && candidate["content"].isObject()) {
                QJsonObject content = candidate["content"].toObject();
                if (content.contains("parts") && content["parts"].isArray()) {
                    QJsonArray parts = content["parts"].toArray();
                    if (!parts.isEmpty() && parts[0].isObject()) {
                        QJsonObject part = parts[0].toObject();
                        if (part.contains("text") && part["text"].isString()) {
                            fullText = part["text"].toString().trimmed();
                            qDebug() << "Extracted full text for descriptions:" << fullText;
                            // Split the text into lines, skipping empty ones
                            suggestions = fullText.split('\n', Qt::SkipEmptyParts);
                            // Trim whitespace and remove potential numbering/bullets
                            for(QString &s : suggestions) {
                                s = s.trimmed();
                                // Remove common list prefixes like "- ", "* ", "1. ", etc.
                                if (s.startsWith("- ") || s.startsWith("* ")) {
                                    s = s.mid(2);
                                } else if (s.length() > 2 && s[1] == '.' && s[0].isDigit()) {
                                    s = s.mid(3);
                                }
                                s = s.trimmed(); // Trim again after removing prefix
                            }
                            suggestions.removeAll(QString("")); // Remove any remaining empty strings
                        } else { qWarning() << "Could not find 'text' field in parts[0] for description"; }
                    } else { qWarning() << "'parts' array is empty or first element is not an object for description"; }
                } else { qWarning() << "'content' object does not contain a 'parts' array for description"; }
            } else { qWarning() << "'candidate' object does not contain a 'content' object for description"; }
        } else { qWarning() << "'candidates' array is empty or first element is not an object for description"; }
    } else { qWarning() << "Could not find 'candidates' array in the Gemini description response."; }


    // --- 5. Display suggestion(s) using the custom SuggestionDialog ---
    if (!suggestions.isEmpty()) {
        qDebug() << "Extracted Description Suggestions:" << suggestions;
        SuggestionDialog suggestionDialog(suggestions, this); // Pass the list to your dialog
        if (suggestionDialog.exec() == QDialog::Accepted) {
            QString selectedSuggestion = suggestionDialog.getSelectedSuggestion();
            if (!selectedSuggestion.isEmpty() && ui->formDescriptionEdit) {
                ui->formDescriptionEdit->setText(selectedSuggestion); // Update the description field
                qDebug() << "User selected description suggestion:" << selectedSuggestion;
            }
        } else {
            qDebug() << "User cancelled description suggestion selection.";
        }
    } else {
        qWarning() << "Could not extract any valid description suggestions from Gemini response.";
        QMessageBox::warning(this, "Suggestion Description (IA)",
                             "Aucune suggestion de description utilisable n'a été reçue de l'API.\n\nTexte reçu:\n" + fullText); // Show raw text if parsing failed
    }
}

// ADDED: SERIAL PORT SETUP
// =========================================================================
void OffresFlashScreen::setupSerialPort() {
    // --- Auto-detect Arduino Port (Simple Method) ---
    QString arduinoPortName = "";
    const auto ports = QSerialPortInfo::availablePorts();
    logMessage(QString("Ports series disponibles (%1):").arg(ports.count())); // Use new log function
    for (const QSerialPortInfo &portInfo : ports) {
        logMessage(QString(" - %1 (%2)").arg(portInfo.portName()).arg(portInfo.description()));
        // Simple check for Arduino identifiers (adjust as needed)
        if (portInfo.manufacturer().contains("Arduino", Qt::CaseInsensitive) ||
            portInfo.description().contains("CH340", Qt::CaseInsensitive) ||
            portInfo.description().contains("USB Serial", Qt::CaseInsensitive) ||
            (portInfo.hasVendorIdentifier() && portInfo.vendorIdentifier() == 0x2341) ||
            (portInfo.hasVendorIdentifier() && portInfo.vendorIdentifier() == 0x1A86) )
        {
            if (arduinoPortName.isEmpty()) {
                arduinoPortName = portInfo.portName();
                logMessage(QString("Port Arduino potentiel detecte: %1").arg(arduinoPortName));
            }
        }
    }

    if (arduinoPortName.isEmpty()) {
        logMessage("AVERTISSEMENT: Aucun port Arduino potentiel detecte automatiquement.");
        emit serialStatusChanged("Port Non Trouve", false); // Emit signal
        return;
    }

    // --- Configure and Open Serial Port ---
    // Use the member variable m_arduinoSerialPort
    m_arduinoSerialPort = new QSerialPort(this); // Parent to 'this'
    m_arduinoSerialPort->setPortName(arduinoPortName);

    if (m_arduinoSerialPort->setBaudRate(QSerialPort::Baud9600) &&
        m_arduinoSerialPort->setDataBits(QSerialPort::Data8) &&
        m_arduinoSerialPort->setParity(QSerialPort::NoParity) &&
        m_arduinoSerialPort->setStopBits(QSerialPort::OneStop) &&
        m_arduinoSerialPort->setFlowControl(QSerialPort::NoFlowControl) &&
        m_arduinoSerialPort->open(QIODevice::ReadWrite))
    {
        logMessage("Connexion serie etablie sur " + arduinoPortName);
        // Connect the readyRead signal to our slot
        connect(m_arduinoSerialPort, &QSerialPort::readyRead, this, &OffresFlashScreen::readSerialData); // Ensure readSerialData slot exists
        emit serialStatusChanged("Connecte: " + arduinoPortName, true); // Emit signal
    } else {
        logMessage("ERREUR: Impossible d'ouvrir/configurer le port serie " + arduinoPortName);
        logMessage("Erreur systeme: " + m_arduinoSerialPort->errorString());
        // Inside OffersFlashScreen::setupSerialPort(), within the 'else' block for port opening failure:
        QMessageBox::critical(this, "Erreur Port Serie",
                              "Impossible d'ouvrir ou configurer le port serie.\nVerifier que l'Arduino est connecte et non utilise par une autre application.\n\n" +
                                  m_arduinoSerialPort->errorString()); // <<< Add the actual error message here
        // delete m_arduinoSerialPort; // Let parent handle deletion
        m_arduinoSerialPort = nullptr; // Indicate failure
        emit serialStatusChanged("Erreur Connexion", false); // Emit signal
    }
}

// =========================================================================
// ADDED: READ SERIAL DATA (Slot)
// =========================================================================
void OffresFlashScreen::readSerialData() { // Ensure this is declared as a slot in .h
    if (!m_arduinoSerialPort || !m_arduinoSerialPort->isOpen()) {
        logMessage("readSerialData appele mais le port n'est pas ouvert.");
        return;
    }

    QByteArray data = m_arduinoSerialPort->readAll();
    m_partialDataBuffer.append(QString::fromUtf8(data)); // Use member buffer

    while (m_partialDataBuffer.contains('\n')) {
        int newlinePos = m_partialDataBuffer.indexOf('\n');
        QString line = m_partialDataBuffer.left(newlinePos).trimmed();
        m_partialDataBuffer = m_partialDataBuffer.mid(newlinePos + 1);

        if (!line.isEmpty()) {
            logMessage("Recu Arduino: " + line); // Use log function
            if (line.startsWith("RFID_UID:")) {
                QString uid = line.mid(9);
                logMessage("UID RFID extrait: " + uid);
                emit newUidReceived(uid); // Emit signal for dialog
                // Clear previous results in dialog before processing
                if (m_kioskMonitorDialog) m_kioskMonitorDialog->clearPatientInfo();
                processUid(uid); // Call the function to handle the UID
            } else {
                logMessage("(Info Arduino): " + line); // Log other messages
            }
        }
    }
}

// =========================================================================
// ADDED: PROCESS UID (Query Database and Send Response)
// =========================================================================
void OffresFlashScreen::processUid(const QString &uid) {
    // Use the member database connection 'm_database'
    if (!m_database.isOpen()) {
        logMessage("ERREUR: Base de donnees non connectee. Impossible de traiter l'UID.");
        if (!initializeDatabaseConnection()) { // Try reconnecting
            logMessage("Reponse (non envoyee): ERR;Erreur BD;Reconnexion echouee");
            emit newUidProcessed("Erreur BD", "Reconnexion impossible", ""); // Emit signal for dialog
            sendToArduino("ERR;Erreur BD;Reconnexion echouee\n"); // Send error back
            return;
        }
    }

    QString statusCode = "ERR";
    QString line1 = "Badge inconnu";
    QString line2 = "Contactez accueil";
    QString patientName = "N/A"; // Use CLIENT table now

    // Prepare query for KIOSK_PLAYS logging later
    QSqlQuery logQuery(m_database);
    QString kioskId = "K01"; // Example Kiosk ID - Make this configurable later if needed

    // Prepare query to find client based on RFID UID
    // **ASSUMPTION:** You need a way to link RFID UID to CLIENT_ID.
    // Let's assume for now the UID *is* the client identifier or stored in a client column.
    // If not, you need another table or column (e.g., CLIENT.RFID_TAG)
    // For this example, let's query the CLIENT table directly using the UID as CLIENT_ID or CIN?
    // Let's assume UID maps to CLIENT_ID for now. Adjust query if needed.
    QSqlQuery query(m_database);
    query.prepare("SELECT CLIENT_ID, NOM, PRENOM FROM CLIENT WHERE CLIENT_ID = :id_or_cin OR CIN = :id_or_cin"); // Check both ID and CIN? Adjust as needed.
    // Convert UID string to number if it represents CLIENT_ID
    bool okNum;
    long long clientIdNum = uid.toLongLong(&okNum);
    if(okNum) {
        query.bindValue(":id_or_cin", clientIdNum);
    } else {
        query.bindValue(":id_or_cin", uid.toUpper()); // Assume it might be CIN if not numeric ID
    }


    logMessage(QString("Execution requete: SELECT CLIENT_ID, NOM, PRENOM FROM CLIENT WHERE ... = '%1'").arg(uid.toUpper()));

    int foundClientId = -1; // Store the found client ID for logging

    if (query.exec() && query.next()) {
        // --- Client Found - Proceed with Draw Logic ---
        foundClientId = query.value("CLIENT_ID").toInt();
        patientName = query.value("PRENOM").toString() + " " + query.value("NOM").toString();
        logMessage("Client trouve: " + patientName + " (ID: " + QString::number(foundClientId) + ")");

        // TODO: Add eligibility checks here (query KIOSK_PLAYS for recent plays by foundClientId or kioskId?)

        // --- Perform Draw ---
        // Simple random draw for now
        long randomValue = QRandomGenerator::global()->bounded(1, 101); // Use Qt's random generator (1-100)
        const long WIN_PROBABILITY_PERCENT = 20; // Example probability
        QString prizeDesc = "";
        QString outcome = "LOSE";

        if (randomValue <= WIN_PROBABILITY_PERCENT) {
            // WIN - Select prize (query OFFERS table for active, validated offers?)
            outcome = "WIN";
            // Example: Fetch a random valid offer description
            QSqlQuery prizeQuery(m_database);
            // Query for an active, validated offer (adjust criteria as needed)
            prizeQuery.prepare("SELECT DESCRIPTION FROM OFFERS "
                               "WHERE STATUT_VALIDATION = 'VALIDATED' AND DATEFIN >= SYSTIMESTAMP "
                               "ORDER BY DBMS_RANDOM.VALUE FETCH FIRST 1 ROWS ONLY"); // Oracle specific random row
            if(prizeQuery.exec() && prizeQuery.next()) {
                prizeDesc = prizeQuery.value(0).toString();
                statusCode = "WIN";
                line1 = "Felicitations!";
                line2 = "Vous gagnez: " + prizeDesc.left(10); // Truncate for LCD line 2
                // Line 3 could show more prize details or a code if available
            } else {
                logMessage("AVERTISSEMENT: Tirage GAGNANT mais impossible de recuperer un lot depuis la BD.");
                statusCode = "WIN"; // Still a win, but with generic message
                line1 = "Felicitations!";
                line2 = "Prix Special!";
                prizeDesc = "Prix Special (Erreur BD)"; // Log this prize description
            }

        } else {
            // LOSE
            outcome = "LOSE";
            statusCode = "LOSE";
            line1 = "   Desole...    ";
            line2 = "Pas de chance ";
            // line3 = "cette fois-ci."; // Fits on line 2
            prizeDesc = ""; // No prize description
        }

        // --- Log the play result ---
        logQuery.prepare("INSERT INTO KIOSK_PLAYS (USERID, KIOSK_ID, OUTCOME, PRIZE_DESCRIPTION) "
                         "VALUES (:userid, :kioskid, :outcome, :prize)");
        logQuery.bindValue(":userid", foundClientId);
        logQuery.bindValue(":kioskid", kioskId);
        logQuery.bindValue(":outcome", outcome);
        logQuery.bindValue(":prize", prizeDesc.isEmpty() ? QVariant() : prizeDesc); // Use QVariant() for NULL

        if (!logQuery.exec()) {
            logMessage("ERREUR SQL: Impossible d'enregistrer le resultat du tirage dans KIOSK_PLAYS.");
            logMessage("Erreur DB: " + logQuery.lastError().databaseText());
        } else {
            logMessage("Resultat du tirage enregistre pour UserID: " + QString::number(foundClientId));
        }


    } else { // Client UID/CIN not found in CLIENT table
        logMessage("UID/CIN non trouve dans la table CLIENT.");
        // Use default "ERR" status and messages
        statusCode = "ERR";
        line1 = "Carte inconnue";
        line2 = "Contactez accueil";
        patientName = "N/A"; // Ensure patient name is cleared
    }

    // Update the UI in the dialog via signals
    emit newUidProcessed(patientName, line1, line2); // Adapt signal parameters as needed

    // --- Construct and Send Response back to Arduino ---
    // Use 3 semicolons for consistency with Arduino parsing logic
    QString response = QString("%1;%2;%3;\n").arg(statusCode).arg(line1).arg(line2); // Added 3rd semicolon and newline
    sendToArduino(response);
}


// =========================================================================
// ADDED: SEND TO ARDUINO
// =========================================================================
void OffresFlashScreen::sendToArduino(const QString &message) {
    // Use member variable m_arduinoSerialPort
    if (m_arduinoSerialPort && m_arduinoSerialPort->isOpen() && m_arduinoSerialPort->isWritable()) {
        QByteArray dataToSend = message.toUtf8();
        qint64 bytesWritten = m_arduinoSerialPort->write(dataToSend);

        if (bytesWritten == -1) {
            logMessage("ERREUR: Echec de l'ecriture sur le port serie.");
            logMessage("Erreur systeme: " + m_arduinoSerialPort->errorString());
        } else if (bytesWritten < dataToSend.size()) {
            logMessage("AVERTISSEMENT: Ecriture incomplete sur le port serie.");
        } else {
            logMessage("Envoye Arduino: " + message.trimmed()); // Log without newline
        }
    } else {
        logMessage("ERREUR: Impossible d'envoyer '" + message.trimmed() + "' a l'Arduino (Port non ouvert/ecriture impossible).");
    }
}


// =========================================================================
// ADDED: LOG MESSAGE (Helper Function)
// =========================================================================
void OffresFlashScreen::logMessage(const QString &message) {
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString logEntry = timestamp + " - " + message;

    qDebug() << logEntry; // Print to Qt Creator's Application Output

    // Emit signal so the dialog can display the log
    emit newLogMessage(logEntry);
}

// MODIFY THIS FUNCTION COMPLETELY:
void OffresFlashScreen::onQrCodeGeneratorClicked() // Rename this function if you renamed the button object
{
    qDebug() << "Sidebar button clicked: Kiosk Monitor"; // Updated log message

    // Lazy instantiation: Create dialog only if it doesn't exist yet
    if (!m_kioskMonitorDialog) {
        logMessage("Creating Kiosk Monitor dialog instance..."); // Use new log function
        m_kioskMonitorDialog = new KioskMonitorDialog(this); // Create it, 'this' is the parent

        // *** Connect Signals from OffersFlashScreen to Slots in KioskMonitorDialog ***
        // Ensure you declare these signals in OffersFlashScreen.h!
        connect(this, &OffresFlashScreen::serialStatusChanged, m_kioskMonitorDialog, &KioskMonitorDialog::updateSerialStatus);
        connect(this, &OffresFlashScreen::dbStatusChanged, m_kioskMonitorDialog, &KioskMonitorDialog::updateDbStatus);
        connect(this, &OffresFlashScreen::newLogMessage, m_kioskMonitorDialog, &KioskMonitorDialog::addLogMessage);
        connect(this, &OffresFlashScreen::newUidProcessed, m_kioskMonitorDialog, &KioskMonitorDialog::updatePatientInfo); // Adapt signal parameters if needed
        connect(this, &OffresFlashScreen::newUidReceived, m_kioskMonitorDialog, &KioskMonitorDialog::updateLastUid);

        // Send initial status when dialog is first created
        emit dbStatusChanged(m_database.isOpen() ? "Connectee (ODBC)" : "Deconnectee", m_database.isOpen());
        if (m_arduinoSerialPort) {
            emit serialStatusChanged(m_arduinoSerialPort->isOpen() ? "Connecte: " + m_arduinoSerialPort->portName() : "Deconnecte", m_arduinoSerialPort->isOpen());
        } else {
            emit serialStatusChanged("Indéterminé", false);
        }
        m_kioskMonitorDialog->clearPatientInfo(); // Ensure fields are clear initially
    }

    // Show the dialog
    m_kioskMonitorDialog->show();
    m_kioskMonitorDialog->raise(); // Bring it to the front
    m_kioskMonitorDialog->activateWindow();
}
