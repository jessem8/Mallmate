#include "founditemsadminwidget.h"
#include "additemdialog.h"
#include "addlostitemdialog.h" // Include new dialog
#include "databaseconnection.h" // Make sure this is included if needed elsewhere, though connect is static

// Standard Qt Widget Includes
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QListWidgetItem>
#include <QSize>
#include <QFrame>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QDateTime> // Needed for currentDateTime
#include <QLocale>   // Needed for QLocale date formatting (Qt6)
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QScreen>
#include <QPageSize>
#include <QMarginsF>
#include <QInputDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QRect>
#include <QEasingCurve>
#include <QStackedWidget>
#include <QFormLayout>
#include <QPointer>
#include <QCoreApplication>
#include <QVariant>
#include <QRegularExpression> // Needed for filterRegularExpression()
#include <QtSql> // Include main header for QSqlDatabase::database()


// --- Qt Charts Includes ---
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>

// Helper function to escape single quotes for direct SQL (basic)
QString escapeSql(const QString& input) {
    QString output = input;
    output.replace("'", "''");
    return output;
}


// Constructor - Initializer list reordered to match header declaration order (fix -Wreorder)
FoundItemsAdminWidget::FoundItemsAdminWidget(QWidget *parent) :
    QWidget(parent),
    // --- Widgets --- (Order roughly matches header)
    sidebarWidget(nullptr), logoLabel(nullptr), sidebarListWidget(nullptr), supportButton(nullptr), signOutButton(nullptr),
    mainStackedWidget(nullptr), itemsManagementWidget(nullptr), lostItemsWidget(nullptr), statisticsWidget(nullptr),
    titleLabel(nullptr), searchLineEdit(nullptr), addFoundItemButton(nullptr), editItemButton(nullptr), deleteItemButton(nullptr), exportPdfButton(nullptr), filterButton(nullptr), sortButton(nullptr), editViewButton(nullptr),
    statsTotalLabel(nullptr), statsTotalValue(nullptr), statsUnclaimedLabel(nullptr), statsUnclaimedValue(nullptr), statsClaimedLabel(nullptr), statsClaimedValue(nullptr), statsReturnedLabel(nullptr), statsReturnedValue(nullptr),
    tableAreaTitleLabel(nullptr), tableResultsCountLabel(nullptr), prevDateButton(nullptr), dateLabel(nullptr), nextDateButton(nullptr),
    itemsTableView(nullptr), emptyTableLabel(nullptr),
    lostItemsTableView(nullptr), addLostReportButton(nullptr), deleteLostItemButton(nullptr),
    statsChartView(nullptr),
    // --- Layouts ---
    mainLayout(nullptr), sidebarLayout(nullptr), itemsManagementLayout(nullptr), topControlsLayout(nullptr), statsLayout(nullptr), tableControlsLayout(nullptr), tableSectionLayout(nullptr), lostItemsLayout(nullptr), statsPageMainLayout(nullptr),
    // --- Models ---
    itemModel(nullptr), proxyModel(nullptr), lostItemModel(nullptr),
    // --- Effects & Animations ---
    tableOpacityEffect(nullptr), emptyLabelOpacityEffect(nullptr), visibilityAnimationGroup(nullptr),
    addButtonAnim(nullptr), originalAddButtonGeometry(), originalAddGeoStored(false), // originalAddButtonGeometry initialized after addButtonAnim
    slideTransitionGroup(nullptr),
    currentStackIndex(0),
    // --- Statistics Counts ---
    m_totalCount(0), m_unclaimedCount(0), m_claimedCount(0), m_returnedCount(0), m_disposedCount(0) // Initialized after animation members
{
    qDebug() << "FoundItemsAdminWidget constructor started...";
    // Connection is established in main.cpp now
    setupUi();
    qDebug() << "setupUi() finished.";
    setupModelsAndData();
    qDebug() << "setupModelsAndData() finished.";
    setupAnimations();
    qDebug() << "setupAnimations() finished.";
    qDebug() << "Connecting signals and slots...";
    // Connect signals AFTER setupUi has created the widgets
    Q_ASSERT(addFoundItemButton != nullptr); if(addFoundItemButton) { connect(addFoundItemButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onAddFoundItemClicked); connect(addFoundItemButton, &QPushButton::pressed, this, &FoundItemsAdminWidget::onAddButtonPressed); connect(addFoundItemButton, &QPushButton::released, this, &FoundItemsAdminWidget::onAddButtonReleased); } else { qWarning("addFoundItemButton is null in constructor"); }
    Q_ASSERT(editItemButton != nullptr); if(editItemButton) connect(editItemButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onEditItemClicked); else { qWarning("editItemButton is null in constructor"); }
    Q_ASSERT(deleteItemButton != nullptr); if(deleteItemButton) connect(deleteItemButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onDeleteItemClicked); else { qWarning("deleteItemButton is null in constructor"); }
    Q_ASSERT(exportPdfButton != nullptr); if(exportPdfButton) connect(exportPdfButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onExportPdfClicked); else { qWarning("exportPdfButton is null in constructor"); }
    Q_ASSERT(filterButton != nullptr); if(filterButton) connect(filterButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onFilterButtonClicked); else { qWarning("filterButton is null in constructor"); }
    Q_ASSERT(sortButton != nullptr); if(sortButton) connect(sortButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onSortButtonClicked); else { qWarning("sortButton is null in constructor"); }
    Q_ASSERT(searchLineEdit != nullptr); if(searchLineEdit) connect(searchLineEdit, &QLineEdit::textChanged, this, &FoundItemsAdminWidget::onSearchTextChanged); else { qWarning("searchLineEdit is null in constructor"); }
    Q_ASSERT(addLostReportButton != nullptr); if(addLostReportButton) connect(addLostReportButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onAddLostItemReportClicked); else { qWarning("addLostReportButton is null"); }
    Q_ASSERT(deleteLostItemButton != nullptr); if(deleteLostItemButton) connect(deleteLostItemButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onDeleteLostItemClicked); else { qWarning("deleteLostItemButton is null"); }
    // Connect selection model signals AFTER models are set on views in setupModelsAndData
    if (itemsTableView && itemsTableView->selectionModel()) { connect(itemsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FoundItemsAdminWidget::updateButtonStates); } else { qWarning() << "Constructor: Found items table/selection model null or not ready."; }
    if (lostItemsTableView && lostItemsTableView->selectionModel()) { connect(lostItemsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FoundItemsAdminWidget::updateButtonStates); } else { qWarning() << "Constructor: Lost items table/selection model null or not ready."; }
    Q_ASSERT(sidebarListWidget != nullptr); if(sidebarListWidget) connect(sidebarListWidget, &QListWidget::currentItemChanged, this, &FoundItemsAdminWidget::onSidebarSelectionChanged); else { qWarning("sidebarListWidget is null in constructor"); }
    Q_ASSERT(prevDateButton != nullptr); if(prevDateButton) connect(prevDateButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onPrevDateClicked); else { qWarning("prevDateButton is null in constructor"); }
    Q_ASSERT(nextDateButton != nullptr); if(nextDateButton) connect(nextDateButton, &QPushButton::clicked, this, &FoundItemsAdminWidget::onNextDateClicked); else { qWarning("nextDateButton is null in constructor"); }
    qDebug() << "Signal connection finished.";
    updateButtonStates();
    updateStats(); // updateStats now needs to be called AFTER setupModelsAndData
    updateTableVisibility(); // Initial call
    if (mainStackedWidget && itemsManagementWidget) { mainStackedWidget->setCurrentWidget(itemsManagementWidget); currentStackIndex = mainStackedWidget->currentIndex(); } else { qWarning() << "Constructor: Cannot set initial widget."; }
    qDebug() << "FoundItemsAdminWidget constructor finished.";
}

FoundItemsAdminWidget::~FoundItemsAdminWidget() {
    qDebug() << "FoundItemsAdminWidget destructor called.";
    // No need to delete child widgets explicitly if they have `this` as parent
}

// --- Status Enum Helpers (Unchanged) ---
QString FoundItemsAdminWidget::statusToString(ItemStatus status) const { switch (status) { case ItemStatus::Unclaimed: return "Unclaimed"; case ItemStatus::Claimed: return "Claimed"; case ItemStatus::Returned: return "Returned"; case ItemStatus::Disposed: return "Disposed"; case ItemStatus::Reported: return "Reported"; case ItemStatus::Found: return "Found"; default: return "Unknown"; } }
ItemStatus FoundItemsAdminWidget::stringToStatus(const QString& statusStr) const { if (statusStr == "Unclaimed") return ItemStatus::Unclaimed; if (statusStr == "Claimed") return ItemStatus::Claimed; if (statusStr == "Returned") return ItemStatus::Returned; if (statusStr == "Disposed") return ItemStatus::Disposed; if (statusStr == "Reported") return ItemStatus::Reported; if (statusStr == "Found") return ItemStatus::Found; return ItemStatus::Unknown; }

// --- Model Setup and Data Loading (Unchanged) ---
void FoundItemsAdminWidget::setupModelsAndData() {
    qDebug() << "setupModelsAndData started...";

    // --- Found Items Model Setup ---
    // 1. Create the source model
    itemModel = new QStandardItemModel(0, FOUND_MODEL_COLUMN_COUNT, this);
    itemModel->setHorizontalHeaderLabels({"", "Item Description", "Date Found", "Location Found", "Status"});

    // 2. Load data into the source model
    loadFoundItemsFromDb(); // Populates itemModel

    // 3. Create the proxy model AFTER source model is populated
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(itemModel); // Set source AFTER itemModel has data
    proxyModel->setFilterKeyColumn(COL_FOUND_DESC); // Filter by description
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // 4. Configure the Table View and set the PROXY model
    if (itemsTableView) {
        itemsTableView->setModel(proxyModel); // Use the proxy model for the view
        itemsTableView->setSortingEnabled(true);
        itemsTableView->setColumnHidden(COL_FOUND_ID, true); // Hide the ID column
        itemsTableView->horizontalHeader()->setSectionResizeMode(COL_FOUND_DESC, QHeaderView::Stretch);
        itemsTableView->setColumnWidth(COL_FOUND_DATE, 120);
        itemsTableView->setColumnWidth(COL_FOUND_LOC, 180);
        itemsTableView->setColumnWidth(COL_FOUND_STATUS, 100);
        itemsTableView->sortByColumn(COL_FOUND_DATE, Qt::AscendingOrder); // Initial sort
        // Connect selection model signal here if not done in constructor
        if (itemsTableView->selectionModel() && !connect(itemsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FoundItemsAdminWidget::updateButtonStates)) {
            qWarning() << "Failed to connect itemsTableView selectionChanged signal in setupModelsAndData";
        }

    } else {
        qWarning() << "setupModelsAndData: itemsTableView is null!";
    }

    // 5. Recalculate stats based on the source model
    recalculateStatsFromModel(); // Calculates stats based on itemModel

    // Log counts AFTER setup
    qDebug() << "Found items setup: itemModel rows:" << (itemModel ? itemModel->rowCount() : -1)
             << " proxyModel rows:" << (proxyModel ? proxyModel->rowCount() : -1);


    // --- Lost Items Model Setup ---
    // 1. Create the source model
    lostItemModel = new QStandardItemModel(0, LOST_MODEL_COLUMN_COUNT, this);
    lostItemModel->setHorizontalHeaderLabels({"", "Item Description", "Date Lost", "Contact Info", "Status"});

    // 2. Load data into the source model
    loadLostItemsFromDb(); // Populates lostItemModel

    // 3. Configure the Table View and set the SOURCE model (no proxy needed currently)
    if (lostItemsTableView) {
        lostItemsTableView->setModel(lostItemModel); // Use the source model directly
        lostItemsTableView->setSortingEnabled(true);
        lostItemsTableView->setColumnHidden(COL_LOST_ID, true); // Hide ID column
        lostItemsTableView->horizontalHeader()->setSectionResizeMode(COL_LOST_DESC, QHeaderView::Stretch);
        lostItemsTableView->setColumnWidth(COL_LOST_DATE, 120);
        lostItemsTableView->setColumnWidth(COL_LOST_CONTACT, 200);
        lostItemsTableView->setColumnWidth(COL_LOST_STATUS, 100);
        lostItemsTableView->sortByColumn(COL_LOST_DATE, Qt::DescendingOrder); // Initial sort
            // Connect selection model signal here if not done in constructor
        if (lostItemsTableView->selectionModel() && !connect(lostItemsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FoundItemsAdminWidget::updateButtonStates)) {
            qWarning() << "Failed to connect lostItemsTableView selectionChanged signal in setupModelsAndData";
        }
    } else {
        qWarning() << "setupModelsAndData: lostItemsTableView is null!";
    }

    // Log count AFTER setup
    qDebug() << "Lost items setup: lostItemModel rows:" << (lostItemModel ? lostItemModel->rowCount() : -1);


    qDebug() << "setupModelsAndData finished.";
}

// --- Load Found Items from Database (Unchanged) ---
void FoundItemsAdminWidget::loadFoundItemsFromDb() {
    // Ensure model exists before clearing/loading
    if (!itemModel) {
        qWarning() << "loadFoundItemsFromDb: itemModel is null! Cannot load.";
        return;
    }
    QSqlDatabase db = QSqlDatabase::database(); // Get default connection
    if (!db.isValid()) {
        qWarning() << "loadFoundItemsFromDb: Invalid database connection handle.";
        return;
    }
    if (!db.isOpen()) {
        // Attempt to open it if closed? Or rely on main.cpp check?
        // For now, just warn, as main should ensure it's open initially.
        qWarning() << "loadFoundItemsFromDb: Database is not open!";
        return;
    }

    qDebug() << "Loading found items from database...";
    itemModel->removeRows(0, itemModel->rowCount()); // Clear existing rows

    QSqlQuery query(db);
    QString sql = "SELECT ITEM_ID, DESCRIPTION, DATE_FOUND, LOCATION_FOUND, STATUS FROM FOUND_ITEMS ORDER BY DATE_FOUND ASC";

    if (!query.exec(sql)) {
        qWarning() << "Failed to execute query to load found items:" << query.lastError().text();
        qWarning() << "DB Error:" << query.lastError().databaseText();
        qWarning() << "Driver Error:" << query.lastError().driverText();
        QMessageBox::warning(this, "Database Error", "Could not load found items from the database.\n" + query.lastError().databaseText());
        return;
    }

    itemModel->blockSignals(true); // Block signals during bulk update
    int loadedCount = 0;
    while (query.next()) {
        QList<QStandardItem *> newRowItems;

        // Column 0: ID (Hidden) - Store as data
        QStandardItem* idItem = new QStandardItem();
        idItem->setData(query.value(COL_FOUND_ID), Qt::DisplayRole); // Store ID
        idItem->setEditable(false);
        newRowItems << idItem;

        // Column 1: Description
        newRowItems << new QStandardItem(query.value(COL_FOUND_DESC).toString());

        // Column 2: Date Found
        QDate dateFound = query.value(COL_FOUND_DATE).toDate();
        newRowItems << new QStandardItem(dateFound.toString(Qt::ISODate)); // Use ISO format for consistency

        // Column 3: Location Found
        newRowItems << new QStandardItem(query.value(COL_FOUND_LOC).toString());

        // Column 4: Status
        newRowItems << new QStandardItem(query.value(COL_FOUND_STATUS).toString());

        // Set items (except ID) as non-editable
        for(int i = 1; i < newRowItems.count(); ++i) {
            if(newRowItems[i]) newRowItems[i]->setEditable(false);
        }

        itemModel->appendRow(newRowItems);
        loadedCount++;
    }
    itemModel->blockSignals(false); // Unblock signals

    qDebug() << "Loaded" << loadedCount << "found items from DB query. itemModel row count:" << itemModel->rowCount();
}

// --- Load Lost Items from Database (Unchanged) ---
void FoundItemsAdminWidget::loadLostItemsFromDb() {
    // Ensure model exists
    if (!lostItemModel) {
        qWarning() << "loadLostItemsFromDb: lostItemModel is null! Cannot load.";
        return;
    }
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qWarning() << "loadLostItemsFromDb: Invalid database connection handle.";
        return;
    }
    if (!db.isOpen()) {
        qWarning() << "loadLostItemsFromDb: Database is not open!";
        return;
    }

    qDebug() << "Loading lost items reports from database...";
    lostItemModel->removeRows(0, lostItemModel->rowCount()); // Clear existing rows

    QSqlQuery query(db);
    // Ensure column names match your LOST_ITEMS table schema
    QString sql = "SELECT REPORT_ID, DESCRIPTION, DATE_LOST, CONTACT_INFO, STATUS FROM LOST_ITEMS ORDER BY DATE_LOST DESC";

    if (!query.exec(sql)) {
        qWarning() << "Failed to execute query to load lost items:" << query.lastError().text();
        qWarning() << "DB Error:" << query.lastError().databaseText();
        qWarning() << "Driver Error:" << query.lastError().driverText();
        QMessageBox::warning(this, "Database Error", "Could not load lost item reports from the database.\n" + query.lastError().databaseText());
        return;
    }

    lostItemModel->blockSignals(true); // Block signals
    int loadedCount = 0;
    while (query.next()) {
        QList<QStandardItem *> newRowItems;

        // Column 0: ID (Hidden)
        QStandardItem* idItem = new QStandardItem();
        idItem->setData(query.value(COL_LOST_ID), Qt::DisplayRole); // Store ID
        idItem->setEditable(false);
        newRowItems << idItem;

        // Column 1: Description
        newRowItems << new QStandardItem(query.value(COL_LOST_DESC).toString());

        // Column 2: Date Lost
        QDate dateLost = query.value(COL_LOST_DATE).toDate();
        newRowItems << new QStandardItem(dateLost.toString(Qt::ISODate));

        // Column 3: Contact Info
        newRowItems << new QStandardItem(query.value(COL_LOST_CONTACT).toString()); // Make sure index matches query

        // Column 4: Status
        newRowItems << new QStandardItem(query.value(COL_LOST_STATUS).toString()); // Make sure index matches query

        // Set items (except ID) as non-editable
        for(int i = 1; i < newRowItems.count(); ++i) {
            if(newRowItems[i]) newRowItems[i]->setEditable(false);
        }

        lostItemModel->appendRow(newRowItems);
        loadedCount++;
    }
    lostItemModel->blockSignals(false); // Unblock signals

    qDebug() << "Loaded" << loadedCount << "lost items reports from DB query. lostItemModel row count:" << lostItemModel->rowCount();
}


// --- Stats Handling (Found Items Only) (Unchanged) ---
void FoundItemsAdminWidget::recalculateStatsFromModel() {
    m_totalCount = 0;
    m_unclaimedCount = 0;
    m_claimedCount = 0;
    m_returnedCount = 0;
    m_disposedCount = 0;

    if (!itemModel) {
        qWarning() << "recalculateStatsFromModel: itemModel is null!";
        updateStats(); // Update UI with zero counts
        return;
    }

    int rowCount = itemModel->rowCount();
    m_totalCount = rowCount;

    for (int row = 0; row < rowCount; ++row) {
        QStandardItem *statusItem = itemModel->item(row, COL_FOUND_STATUS);
        if (statusItem) {
            ItemStatus status = stringToStatus(statusItem->text());
            switch (status) {
            case ItemStatus::Unclaimed: m_unclaimedCount++; break;
            case ItemStatus::Claimed:   m_claimedCount++;   break;
            case ItemStatus::Returned:  m_returnedCount++;  break;
            case ItemStatus::Disposed:  m_disposedCount++;  break;
            default: break; // Ignore Reported, Found, Unknown for these stats
            }
        }
    }
    qDebug() << "Found Item Stats Recalculated: Total=" << m_totalCount
             << " Unclaimed=" << m_unclaimedCount << " Claimed=" << m_claimedCount
             << " Returned=" << m_returnedCount << " Disposed=" << m_disposedCount;

    updateStats(); // Update the UI labels and chart
}

// --- Update Stats Incrementally (Unchanged) ---
void FoundItemsAdminWidget::updateStatsCount(ItemStatus oldStatus, ItemStatus newStatus) {
    // Only adjust counts if status actually changed relevantly
    if (oldStatus == newStatus && oldStatus != ItemStatus::Unknown) return;

    // Decrement count for the old status (if it was a tracked status)
    switch (oldStatus) {
    case ItemStatus::Unclaimed: m_unclaimedCount--; break;
    case ItemStatus::Claimed:   m_claimedCount--;   break;
    case ItemStatus::Returned:  m_returnedCount--;  break;
    case ItemStatus::Disposed:  m_disposedCount--;  break;
    default: break; // No change if old status wasn't tracked or Unknown
    }

    // Increment count for the new status (if it's a tracked status)
    switch (newStatus) {
    case ItemStatus::Unclaimed: m_unclaimedCount++; break;
    case ItemStatus::Claimed:   m_claimedCount++;   break;
    case ItemStatus::Returned:  m_returnedCount++;  break;
    case ItemStatus::Disposed:  m_disposedCount++;  break;
    default: break; // No change if new status isn't tracked or Unknown
    }

    // Adjust total count only when adding/removing a valid item
    if (oldStatus == ItemStatus::Unknown && newStatus != ItemStatus::Unknown && newStatus != ItemStatus::Reported && newStatus != ItemStatus::Found) {
        m_totalCount++; // Added a new found item
    } else if (oldStatus != ItemStatus::Unknown && oldStatus != ItemStatus::Reported && oldStatus != ItemStatus::Found && newStatus == ItemStatus::Unknown) {
        m_totalCount--; // Removed a found item
    }

    // Ensure counts don't go below zero
    m_totalCount = qMax(0, m_totalCount);
    m_unclaimedCount = qMax(0, m_unclaimedCount);
    m_claimedCount = qMax(0, m_claimedCount);
    m_returnedCount = qMax(0, m_returnedCount);
    m_disposedCount = qMax(0, m_disposedCount);

    qDebug() << "Found Item Stats Incremented: Total=" << m_totalCount
             << " Unclaimed=" << m_unclaimedCount << " Claimed=" << m_claimedCount
             << " Returned=" << m_returnedCount << " Disposed=" << m_disposedCount;

    updateStats(); // Update the UI labels and chart
}


// --- Update Stats UI (Labels and Chart) (Unchanged) ---
void FoundItemsAdminWidget::updateStats() {
    // Update Labels
    if(statsTotalValue) statsTotalValue->setText(QString::number(m_totalCount));
    if(statsUnclaimedValue) statsUnclaimedValue->setText(QString::number(m_unclaimedCount));
    if(statsClaimedValue) statsClaimedValue->setText(QString::number(m_claimedCount));
    if(statsReturnedValue) statsReturnedValue->setText(QString::number(m_returnedCount));
    // Note: No label for Disposed shown in UI setup

    // Update Chart View
    if (!statsChartView) {
        qWarning() << "updateStats called with null chart view!";
        return;
    }

    QChart *chart = statsChartView->chart();
    QPieSeries *series = nullptr;

    // Find existing series or create a new one
    if (chart) {
        auto currentSeriesList = chart->series();
        if (!currentSeriesList.isEmpty() && (series = dynamic_cast<QPieSeries*>(currentSeriesList.first()))) {
            // Existing series found, clear it
            series->clear();
        } else {
            // No valid series found, remove any old ones and create new
            if(chart) chart->removeAllSeries();
            series = new QPieSeries();
            series->setHoleSize(0.35); // Optional: for donut chart style
            if(chart) {
                chart->addSeries(series);
                chart->setTitle("Found Items Breakdown by Status");
                chart->setAnimationOptions(QChart::SeriesAnimations); // Enable animations
                if(chart->legend()) {
                    chart->legend()->setVisible(true);
                    chart->legend()->setAlignment(Qt::AlignBottom);
                }
            } else {
                qWarning() << "updateStats: Chart became null unexpectedly!";
                delete series; // Clean up allocated series
                return;
            }
        }
    } else {
        // Chart doesn't exist, create it
        chart = new QChart();
        chart->setTitle("Found Items Breakdown by Status");
        chart->setAnimationOptions(QChart::SeriesAnimations);
        statsChartView->setChart(chart); // Set the new chart on the view
        if(chart->legend()) {
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);
        }
        series = new QPieSeries();
        series->setHoleSize(0.35);
        chart->addSeries(series);
    }

    // Add data to the series
    if (m_unclaimedCount > 0) series->append(QString("Unclaimed (%1)").arg(m_unclaimedCount), m_unclaimedCount);
    if (m_claimedCount > 0) series->append(QString("Claimed (%1)").arg(m_claimedCount), m_claimedCount);
    if (m_returnedCount > 0) series->append(QString("Returned (%1)").arg(m_returnedCount), m_returnedCount);
    if (m_disposedCount > 0) series->append(QString("Disposed (%1)").arg(m_disposedCount), m_disposedCount);

    // Configure slice labels if there's data
    if (!series->isEmpty()) {
        series->setLabelsVisible(true);
        series->setLabelsPosition(QPieSlice::LabelOutside); // Position labels outside slices
        for(QPieSlice *slice : series->slices()) {
            // Customize label format (e.g., "Status\nValue%")
            slice->setLabel(QString("%1\n%2%")
                                .arg(slice->label().split(" ").first()) // Get status name
                                .arg(100 * slice->percentage(), 0, 'f', 1)); // Format percentage
            slice->setLabelVisible(true);
            // Optional: Explode or color slices based on status
            // if (slice->label().startsWith("Unclaimed")) slice->setExploded(true);
        }
        chart->setTitle("Found Items Breakdown by Status");
    } else {
        // Handle case with no data
        chart->setTitle("Found Items Breakdown by Status (No data)");
    }
}


// --- Slot Implementations ---

// --- Add Found Item ---
// <<< WORKAROUND: Using direct SQL execution instead of prepared statements >>>
void FoundItemsAdminWidget::onAddFoundItemClicked() {
    qDebug() << "Add Found Item Clicked!";
    QSqlDatabase db = QSqlDatabase::database(); // Get default connection
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not valid or not open.");
        return;
    }

    AddItemDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString description = dialog.description();
        QDate date = dialog.dateFound();
        QString location = dialog.location();
        QString statusStr = dialog.status();
        ItemStatus status = stringToStatus(statusStr);

        if (description.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Description cannot be empty.");
            return;
        }

        // --- Get New ID using Sequence (Oracle specific) ---
        qlonglong newId = -1;
        QSqlQuery seqQuery(db);
        if (!seqQuery.exec("SELECT found_items_seq.NEXTVAL FROM DUAL")) {
            qWarning() << "Failed to get next value from sequence 'found_items_seq':" << seqQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not generate a new item ID from the database sequence.\n" + seqQuery.lastError().databaseText());
            return;
        }
        if (seqQuery.next()) { newId = seqQuery.value(0).toLongLong(); }
        else { /* Error handling */ return; }
        if (newId <= 0) { /* Error handling */ return; }
        qDebug() << "Generated new Found Item ID:" << newId;
        // --- End Get New ID ---

        // --- Start Transaction ---
        if (!db.transaction()) {
            qWarning() << "Failed to start database transaction:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not start database transaction.\n" + db.lastError().text());
            return;
        }
        qDebug() << "Database transaction started for INSERT.";
        // --- End Start Transaction ---

        // --- Execute Direct Insert ---
        // !!! WARNING: This is for DIAGNOSTIC PURPOSES ONLY due to SQL injection risks !!!
        // It bypasses prepare() and bindValue() to test if the issue lies there.
        QSqlQuery query(db);
        QString sqlStatement = QString(
                                   "INSERT INTO FOUND_ITEMS (ITEM_ID, DESCRIPTION, DATE_FOUND, LOCATION_FOUND, STATUS) "
                                   "VALUES (%1, '%2', TO_DATE('%3', 'YYYY-MM-DD'), '%4', '%5')")
                                   .arg(newId)
                                   .arg(escapeSql(description)) // Use helper function for basic escaping
                                   .arg(date.toString(Qt::ISODate))
                                   .arg(escapeSql(location))    // Use helper function for basic escaping
                                   .arg(escapeSql(statusStr));  // Use helper function for basic escaping

        qDebug() << "Executing direct SQL (DIAGNOSTIC):" << sqlStatement;
        bool execOk = query.exec(sqlStatement);
        // !!! END WARNING !!!

        if (!execOk) {
            qWarning() << "Failed to execute direct insert for found item."; // Modified message
            qWarning() << "Qt Error:" << query.lastError().text();
            qWarning() << "DB Error:" << query.lastError().databaseText();
            qWarning() << "Driver Error:" << query.lastError().driverText();
            QString errorToShow = query.lastError().databaseText().isEmpty() ? query.lastError().text() : query.lastError().databaseText();
            QMessageBox::critical(this, "Database Error", "Failed to add the item to the database (Direct Exec).\n" + errorToShow); // Modified message
            // --- Rollback Transaction ---
            if (!db.rollback()) {
                qWarning() << "Failed to rollback transaction after INSERT error:" << db.lastError().text();
            } else {
                qDebug() << "Transaction rolled back after INSERT error.";
            }
            // --- End Rollback Transaction ---
            return;
        } else {
            qDebug() << "Direct INSERT query executed successfully."; // Modified message
            // --- Commit Transaction ---
            if (!db.commit()) {
                qWarning() << "Failed to commit transaction after INSERT:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to commit the new item to the database.\n" + db.lastError().databaseText());
                if (!db.rollback()) { qWarning() << "Failed to rollback transaction after COMMIT error:" << db.lastError().text(); }
                else { qDebug() << "Transaction rolled back after COMMIT error."; }
                return;
            } else {
                qDebug() << "Transaction committed successfully after INSERT.";
                // --- Add to Model (Only after successful commit) ---
                if(itemModel) {
                    QList<QStandardItem *> newRowItems;
                    QStandardItem* idItem = new QStandardItem();
                    idItem->setData(QVariant(newId), Qt::DisplayRole);
                    idItem->setEditable(false);
                    newRowItems << idItem;
                    newRowItems << new QStandardItem(description);
                    newRowItems << new QStandardItem(date.toString(Qt::ISODate));
                    newRowItems << new QStandardItem(location);
                    newRowItems << new QStandardItem(statusStr);
                    for(int i = 1; i < newRowItems.count(); ++i) { if(newRowItems[i]) newRowItems[i]->setEditable(false); }
                    itemModel->appendRow(newRowItems);
                    updateStatsCount(ItemStatus::Unknown, status);
                    qDebug() << "New found item added to model:" << description;
                    updateTableVisibility();
                    updateButtonStates();
                    if(itemsTableView) itemsTableView->scrollToBottom();
                    QMessageBox::information(this, "Success", QString("Item '%1' added successfully.").arg(description));
                } else {
                    qWarning("onAddFoundItemClicked: itemModel is null! Cannot update view.");
                    QMessageBox::warning(this, "UI Warning", "Item added to database, but UI model is missing.");
                }
                // --- End Add to Model ---
            }
            // --- End Commit Transaction ---
        }
        // --- End Execute Direct Insert ---
    } else {
        qDebug() << "Add Dialog Rejected / Cancelled";
    }
}


// --- Edit Found Item ---
// <<< WORKAROUND: Using direct SQL execution instead of prepared statements >>>
void FoundItemsAdminWidget::onEditItemClicked() {
    qDebug() << "Edit Found Item Clicked!";
    if (!itemsTableView || !itemsTableView->selectionModel() || !proxyModel || !itemModel) { /* Error handling */ return; }
    QItemSelectionModel *selectionModel = itemsTableView->selectionModel();
    if (!selectionModel->hasSelection()) { /* Info message */ return; }
    QModelIndex selectedProxyIndex = selectionModel->currentIndex();
    if (!selectedProxyIndex.isValid()) return;
    QModelIndex selectedSourceIndex = proxyModel->mapToSource(selectedProxyIndex);
    int sourceRow = selectedSourceIndex.row();
    if (!selectedSourceIndex.isValid() || sourceRow < 0 || sourceRow >= itemModel->rowCount()) { /* Error handling */ return; }
    QStandardItem* idItem = itemModel->item(sourceRow, COL_FOUND_ID);
    if (!idItem) { /* Error handling */ return; }
    bool idOk; qlonglong currentId = idItem->data(Qt::DisplayRole).toLongLong(&idOk);
    if (!idOk || currentId <= 0) { /* Error handling */ return; }
    QString currentDesc = itemModel->item(sourceRow, COL_FOUND_DESC) ? itemModel->item(sourceRow, COL_FOUND_DESC)->text() : "";
    QDate currentDate = QDate::fromString(itemModel->item(sourceRow, COL_FOUND_DATE) ? itemModel->item(sourceRow, COL_FOUND_DATE)->text() : "", Qt::ISODate);
    QString currentLoc = itemModel->item(sourceRow, COL_FOUND_LOC) ? itemModel->item(sourceRow, COL_FOUND_LOC)->text() : "";
    QString currentStatusStr = itemModel->item(sourceRow, COL_FOUND_STATUS) ? itemModel->item(sourceRow, COL_FOUND_STATUS)->text() : "";
    ItemStatus currentStatus = stringToStatus(currentStatusStr);

    AddItemDialog dialog(this);
    dialog.setData(currentDesc, currentDate, currentLoc, currentStatusStr);

    if (dialog.exec() == QDialog::Accepted) {
        QString newDesc = dialog.description();
        QDate newDate = dialog.dateFound();
        QString newLoc = dialog.location();
        QString newStatusStr = dialog.status();
        ItemStatus newStatus = stringToStatus(newStatusStr);
        if (newDesc.isEmpty()) { /* Warning */ return; }

        // --- Update Database ---
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isValid() || !db.isOpen()) { /* Error handling */ return; }

        // --- Start Transaction ---
        if (!db.transaction()) {
            qWarning() << "Failed to start database transaction for UPDATE:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not start database transaction.\n" + db.lastError().text());
            return;
        }
        qDebug() << "Database transaction started for UPDATE.";
        // --- End Start Transaction ---

        // --- Execute Direct Update ---
        // !!! WARNING: This is for DIAGNOSTIC PURPOSES ONLY due to SQL injection risks !!!
        QSqlQuery query(db);
        QString sqlStatement = QString(
                                   "UPDATE FOUND_ITEMS SET DESCRIPTION = '%1', DATE_FOUND = TO_DATE('%2', 'YYYY-MM-DD'), "
                                   "LOCATION_FOUND = '%3', STATUS = '%4' WHERE ITEM_ID = %5")
                                   .arg(escapeSql(newDesc))
                                   .arg(newDate.toString(Qt::ISODate))
                                   .arg(escapeSql(newLoc))
                                   .arg(escapeSql(newStatusStr))
                                   .arg(currentId);

        qDebug() << "Executing direct SQL (DIAGNOSTIC):" << sqlStatement;
        bool execOk = query.exec(sqlStatement);
        // !!! END WARNING !!!

        if (!execOk) {
            qWarning() << "Failed to execute direct update for found item (ID: " << currentId << ")"; // Modified message
            qWarning() << "Qt Error:" << query.lastError().text();
            qWarning() << "DB Error:" << query.lastError().databaseText();
            qWarning() << "Driver Error:" << query.lastError().driverText();
            QString errorToShow = query.lastError().databaseText().isEmpty() ? query.lastError().text() : query.lastError().databaseText();
            QMessageBox::critical(this, "Database Error", "Failed to update the item in the database (Direct Exec).\n" + errorToShow); // Modified message
            // --- Rollback Transaction ---
            if (!db.rollback()) {
                qWarning() << "Failed to rollback transaction after UPDATE error:" << db.lastError().text();
            } else {
                qDebug() << "Transaction rolled back after UPDATE error.";
            }
            // --- End Rollback Transaction ---
            return;
        } else {
            qDebug() << "Direct UPDATE query executed successfully. Rows affected:" << query.numRowsAffected(); // Modified message
            // --- Commit Transaction ---
            if (!db.commit()) {
                qWarning() << "Failed to commit transaction after UPDATE:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to commit the updated item to the database.\n" + db.lastError().databaseText());
                if (!db.rollback()) { qWarning() << "Failed to rollback transaction after UPDATE COMMIT error:" << db.lastError().text(); }
                else { qDebug() << "Transaction rolled back after UPDATE COMMIT error."; }
                return; // Don't update model if commit failed
            } else {
                qDebug() << "Transaction committed successfully after UPDATE.";
                // --- Update Model (Only after successful commit) ---
                if (query.numRowsAffected() == 0) {
                    qWarning() << "Update query affected 0 rows for ID:" << currentId << "(Item might have been deleted externally).";
                    QMessageBox::warning(this, "Update Warning", "The selected item might have been deleted by another process. Refreshing data.");
                    loadFoundItemsFromDb(); // Reload data
                    recalculateStatsFromModel();
                    updateTableVisibility();
                    updateButtonStates();
                } else {
                    if(itemModel->item(sourceRow, COL_FOUND_DESC)) itemModel->item(sourceRow, COL_FOUND_DESC)->setText(newDesc);
                    if(itemModel->item(sourceRow, COL_FOUND_DATE)) itemModel->item(sourceRow, COL_FOUND_DATE)->setText(newDate.toString(Qt::ISODate));
                    if(itemModel->item(sourceRow, COL_FOUND_LOC)) itemModel->item(sourceRow, COL_FOUND_LOC)->setText(newLoc);
                    if(itemModel->item(sourceRow, COL_FOUND_STATUS)) itemModel->item(sourceRow, COL_FOUND_STATUS)->setText(newStatusStr);
                    if (currentStatus != newStatus) { updateStatsCount(currentStatus, newStatus); }
                    qDebug() << "Found item updated in model and database:" << newDesc;
                    QMessageBox::information(this, "Success", QString("Item '%1' updated successfully.").arg(newDesc));
                }
                // --- End Update Model ---
            }
            // --- End Commit Transaction ---
        }
        // --- End Execute Direct Update ---
        // --- End Update Database ---
    } else {
        qDebug() << "Edit Dialog Rejected / Cancelled";
    }
}


// --- Delete Found Item ---
// <<< WORKAROUND: Using direct SQL execution instead of prepared statements >>>
void FoundItemsAdminWidget::onDeleteItemClicked() {
    qDebug() << "Delete Found Item Clicked!";
    if (!itemsTableView || !itemsTableView->selectionModel() || !proxyModel || !itemModel) { /* Error handling */ return; }
    QItemSelectionModel *selectionModel = itemsTableView->selectionModel();
    if (!selectionModel->hasSelection()) { /* Info message */ return; }
    QModelIndex selectedProxyIndex = selectionModel->currentIndex();
    if (!selectedProxyIndex.isValid()) return;
    QModelIndex selectedSourceIndex = proxyModel->mapToSource(selectedProxyIndex);
    int sourceRow = selectedSourceIndex.row();
    if (!selectedSourceIndex.isValid() || sourceRow < 0 || sourceRow >= itemModel->rowCount()) { /* Error handling */ return; }
    QStandardItem* idItem = itemModel->item(sourceRow, COL_FOUND_ID);
    if (!idItem) { /* Error handling */ return; }
    bool idOk; qlonglong currentId = idItem->data(Qt::DisplayRole).toLongLong(&idOk);
    if (!idOk || currentId <= 0) { /* Error handling */ return; }
    QString itemDesc = itemModel->item(sourceRow, COL_FOUND_DESC) ? itemModel->item(sourceRow, COL_FOUND_DESC)->text() : "Unknown Item";
    QString itemStatusStr = itemModel->item(sourceRow, COL_FOUND_STATUS) ? itemModel->item(sourceRow, COL_FOUND_STATUS)->text() : "";
    ItemStatus itemStatus = stringToStatus(itemStatusStr);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete", QString("Are you sure you want to delete the found item '%1'?").arg(itemDesc), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // --- Delete from Database ---
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isValid() || !db.isOpen()) { /* Error handling */ return; }

        // --- Start Transaction ---
        if (!db.transaction()) {
            qWarning() << "Failed to start database transaction for DELETE:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not start database transaction.\n" + db.lastError().text());
            return;
        }
        qDebug() << "Database transaction started for DELETE.";
        // --- End Start Transaction ---

        // --- Execute Direct Delete ---
        // !!! WARNING: This is for DIAGNOSTIC PURPOSES ONLY !!!
        QSqlQuery query(db);
        QString sqlStatement = QString("DELETE FROM FOUND_ITEMS WHERE ITEM_ID = %1").arg(currentId);

        qDebug() << "Executing direct SQL (DIAGNOSTIC):" << sqlStatement;
        bool execOk = query.exec(sqlStatement);
        // !!! END WARNING !!!

        if (!execOk) {
            qWarning() << "Failed to execute direct delete for found item (ID: " << currentId << ")"; // Modified message
            qWarning() << "Qt Error:" << query.lastError().text();
            qWarning() << "DB Error:" << query.lastError().databaseText();
            qWarning() << "Driver Error:" << query.lastError().driverText();
            QString errorToShow = query.lastError().databaseText().isEmpty() ? query.lastError().text() : query.lastError().databaseText();
            QMessageBox::critical(this, "Database Error", "Failed to delete the item from the database (Direct Exec).\n" + errorToShow); // Modified message
            // --- Rollback Transaction ---
            if (!db.rollback()) {
                qWarning() << "Failed to rollback transaction after DELETE error:" << db.lastError().text();
            } else {
                qDebug() << "Transaction rolled back after DELETE error.";
            }
            // --- End Rollback Transaction ---
            return;
        } else {
            qDebug() << "Direct DELETE query executed successfully. Rows affected:" << query.numRowsAffected(); // Modified message
            // --- Commit Transaction ---
            if (!db.commit()) {
                qWarning() << "Failed to commit transaction after DELETE:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to commit the deletion to the database.\n" + db.lastError().databaseText());
                if (!db.rollback()) { qWarning() << "Failed to rollback transaction after DELETE COMMIT error:" << db.lastError().text(); }
                else { qDebug() << "Transaction rolled back after DELETE COMMIT error."; }
                return; // Don't remove from model if commit failed
            } else {
                qDebug() << "Transaction committed successfully after DELETE.";
                // --- Remove from Model (Only after successful commit) ---
                if (query.numRowsAffected() == 0) {
                    qWarning() << "Delete query affected 0 rows for ID:" << currentId << "(Item might have been deleted externally).";
                    QMessageBox::warning(this, "Delete Warning", "The selected item might have already been deleted. Refreshing data.");
                    // Consider reloading data here if needed
                }
                // Remove row regardless of numRowsAffected, as the goal is deletion
                itemModel->removeRow(sourceRow);
                qDebug() << "Found item removed from model.";
                updateStatsCount(itemStatus, ItemStatus::Unknown);
                updateTableVisibility();
                updateButtonStates();
                QMessageBox::information(this, "Success", QString("Item '%1' deleted successfully.").arg(itemDesc));
                // --- End Remove from Model ---
            }
            // --- End Commit Transaction ---
        }
        // --- End Execute Direct Delete ---
        // --- End Delete from Database ---
    } else {
        qDebug() << "Deletion cancelled by user.";
    }
}


// --- Search Found Items (Unchanged) ---
void FoundItemsAdminWidget::onSearchTextChanged(const QString &text) {
    qDebug() << "Search text changed (Found Items):" << text;
    if(proxyModel) {
        // Ensure filter is applied to the correct column (Description)
        proxyModel->setFilterKeyColumn(COL_FOUND_DESC);
        // Use setFilterFixedString for simple substring matching (case-insensitive set earlier)
        proxyModel->setFilterFixedString(text);
        // Or use setFilterRegularExpression for more complex patterns:
        // proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
    }
    updateTableVisibility(); // Update visibility based on filtered results
    updateButtonStates(); // Selection might change or become invalid
}

// --- Filter Found Items (Unchanged) ---
void FoundItemsAdminWidget::onFilterButtonClicked() {
    qDebug() << "Filter Button Clicked (Found Items)!";
    if (!proxyModel) {
        qWarning("Filter button clicked but proxyModel is null!");
        return;
    }

    QStringList statusItems;
    statusItems << "All"; // Option to show all items
    statusItems << statusToString(ItemStatus::Unclaimed);
    statusItems << statusToString(ItemStatus::Claimed);
    statusItems << statusToString(ItemStatus::Returned);
    statusItems << statusToString(ItemStatus::Disposed);

    bool ok;
    QString currentFilterText = "";
    int currentSelectionIndex = 0; // Default to "All"

    // Check if currently filtering by status column
    if (proxyModel->filterKeyColumn() == COL_FOUND_STATUS) {
        // *** Use filterRegularExpression().pattern() to get the string ***
        currentFilterText = proxyModel->filterRegularExpression().pattern();
        // Remove potential regex characters if setFilterFixedString was used internally
        // (setFilterFixedString often escapes regex characters)
        // A safer approach might be to store the last selected status string in a member variable.
        // For now, we just try to find the exact match in the list.
        int idx = statusItems.indexOf(currentFilterText);
        if (idx >= 0) {
            currentSelectionIndex = idx;
        }
    }

    QString selectedStatus = QInputDialog::getItem(this, "Filter Found Items by Status",
                                                   "Select status to show:", statusItems,
                                                   currentSelectionIndex, false, &ok);

    if (ok && !selectedStatus.isEmpty()) {
        qDebug() << "Status selected for filtering:" << selectedStatus;
        if(searchLineEdit) searchLineEdit->clear(); // Clear search field when applying status filter

        if (selectedStatus == "All") {
            // Clear the status filter by setting an invalid key column or empty string
            proxyModel->setFilterKeyColumn(-1); // No column filter
            proxyModel->setFilterFixedString(""); // No string filter
        } else {
            // Apply filter to the status column
            proxyModel->setFilterKeyColumn(COL_FOUND_STATUS);
            proxyModel->setFilterFixedString(selectedStatus);
        }
        updateTableVisibility(); // Update view based on new filter
        updateButtonStates();
    } else {
        qDebug() << "Filter dialog cancelled or selection was empty.";
    }
}

// --- Export Found Items to PDF (Unchanged) ---
void FoundItemsAdminWidget::onExportPdfClicked() {
    qDebug() << "Export PDF Clicked (Found Items)!";
    // Use the PROXY model for export, so it reflects current filtering/sorting
    if (!itemsTableView || !proxyModel || !proxyModel->sourceModel()) {
        QMessageBox::warning(this, "Export Error", "Table or data model not available for export.");
        return;
    }

    QString defaultFilename = QString("Found_Items_Report_%1.pdf").arg(QDate::currentDate().toString("yyyyMMdd"));
    QString fileName = QFileDialog::getSaveFileName(this, "Save Found Items PDF Report", defaultFilename, "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        qDebug() << "PDF Export cancelled.";
        return;
    }
    // Ensure .pdf extension
    if (!fileName.toLower().endsWith(".pdf")) {
        fileName += ".pdf";
    }

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4)); // Standard A4 size
    pdfWriter.setPageOrientation(QPageLayout::Landscape); // Landscape orientation
    QMarginsF pageMargins(15, 15, 15, 15); // Margins in mm
    pdfWriter.setPageMargins(pageMargins);
    pdfWriter.setTitle("Found Items Report");
    pdfWriter.setCreator("Mallmate Application");

    QPainter painter;
    if (!painter.begin(&pdfWriter)) {
        QMessageBox::critical(this, "Export Error", "Could not start painting on the PDF device.\nCheck file permissions or if the file is already open.");
        return;
    }

    // --- PDF Content ---
    painter.setRenderHint(QPainter::Antialiasing);
    int currentY = 0; // Keep track of vertical position

    // Title
    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    QRectF titleRect(0, currentY, pdfWriter.width(), 40);
    painter.drawText(titleRect, Qt::AlignCenter, "Found Items Report");
    currentY += titleRect.height() + 10; // Add some space after title

    // Optional: Add date/filter information
    painter.setFont(QFont("Segoe UI", 9));
    QString filterInfo = "Filter: ";
    // *** Use filterRegularExpression().pattern() to get filter string ***
    QString currentFilterPattern = proxyModel->filterRegularExpression().pattern();
    if (proxyModel->filterKeyColumn() == COL_FOUND_STATUS && !currentFilterPattern.isEmpty()) {
        filterInfo += "Status = " + currentFilterPattern; // Might include regex chars if not set via fixed string
    } else if (proxyModel->filterKeyColumn() == COL_FOUND_DESC && !currentFilterPattern.isEmpty()) {
        filterInfo += "Description contains '" + currentFilterPattern + "'";
    } else {
        filterInfo += "All";
    }
    QRectF dateRect(0, currentY, pdfWriter.width(), 20);
    // *** Use QLocale for date formatting ***
    QString generatedDate = QLocale::system().toString(QDateTime::currentDateTime(), QLocale::ShortFormat);
    painter.drawText(dateRect, Qt::AlignLeft, QString("Generated: %1 | %2").arg(generatedDate).arg(filterInfo));
    currentY += dateRect.height() + 15;


    // Table Header
    painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
    QHeaderView *header = itemsTableView->horizontalHeader();
    if (!header) {
        qWarning("Export PDF: Horizontal header missing!");
        painter.end();
        QMessageBox::critical(this, "Export Error", "Table header is not available for export.");
        return;
    }

    // Calculate column widths based on view (adjust as needed)
    QList<int> columnWidths;
    QList<QString> headers;
    int totalWidth = 0;
    for (int col = 0; col < proxyModel->columnCount(); ++col) {
        if (!itemsTableView->isColumnHidden(col)) {
            headers << proxyModel->headerData(col, Qt::Horizontal).toString();
            int w = header->sectionSize(col);
            columnWidths << w;
            totalWidth += w;
        }
    }

    // Scale factor to fit table width to page width (minus margins)
    double availableWidth = pdfWriter.width(); // Width within margins
    double scale = (totalWidth > 0 && availableWidth > 0) ? qMin(1.0, availableWidth / totalWidth) : 1.0;


    // Draw Header Row
    int currentX = 0;
    painter.save();
    painter.setPen(Qt::black);
    painter.setBrush(QColor("#E0E0E0")); // Light gray background for header
    QRectF headerBgRect(0, currentY, availableWidth, 25); // Background for the whole header row
    painter.drawRect(headerBgRect);

    for (int i = 0; i < headers.size(); ++i) {
        int colWidth = columnWidths[i] * scale;
        QRectF headerRect(currentX, currentY, colWidth, 25); // Header cell rectangle
        painter.drawText(headerRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
        painter.drawLine(currentX + colWidth, currentY, currentX + colWidth, currentY + 25); // Vertical line
        currentX += colWidth;
    }
    painter.restore();
    currentY += 25; // Move down past header


    // Draw Table Rows
    painter.setFont(QFont("Segoe UI", 8)); // Smaller font for data
    int rowCount = proxyModel->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        currentX = 0;
        int rowHeight = 20; // Base row height

        // Alternate row colors (optional)
        if (row % 2 != 0) {
            painter.save();
            painter.setBrush(QColor("#F9F9F9"));
            painter.setPen(Qt::NoPen);
            painter.drawRect(QRectF(0, currentY, availableWidth, rowHeight));
            painter.restore();
        }

        int currentVisibleCol = 0;
        for (int col = 0; col < proxyModel->columnCount(); ++col) {
            if (!itemsTableView->isColumnHidden(col)) {
                int colWidth = columnWidths[currentVisibleCol] * scale;
                QModelIndex index = proxyModel->index(row, col);
                QString text = proxyModel->data(index).toString();
                QRectF cellRect(currentX, currentY, colWidth, rowHeight);
                painter.drawText(cellRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, text); // Allow word wrap
                currentX += colWidth;
                currentVisibleCol++;
            }
        }
        painter.drawLine(0, currentY + rowHeight, availableWidth, currentY + rowHeight); // Horizontal line below row
        currentY += rowHeight;

        // Check for page break (simple check, might need refinement)
        if (currentY > pdfWriter.height() - 30) { // Check if near bottom margin
            pdfWriter.newPage();
            currentY = 0; // Reset Y position for new page
            // Optionally re-draw header on new page
            painter.setFont(QFont("Segoe UI", 9, QFont::Bold));
            currentX = 0;
            painter.save();
            painter.setPen(Qt::black);
            painter.setBrush(QColor("#E0E0E0"));
            QRectF headerBgRect(0, currentY, availableWidth, 25);
            painter.drawRect(headerBgRect);
            for (int i = 0; i < headers.size(); ++i) {
                int colWidth = columnWidths[i] * scale;
                QRectF headerRect(currentX, currentY, colWidth, 25);
                painter.drawText(headerRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
                painter.drawLine(currentX + colWidth, currentY, currentX + colWidth, currentY + 25);
                currentX += colWidth;
            }
            painter.restore();
            currentY += 25;
            painter.setFont(QFont("Segoe UI", 8));
        }
    }
    // --- End PDF Content ---


    if (!painter.end()) {
        qWarning() << "Painter failed to end gracefully. PDF generation likely failed.";
        QMessageBox::critical(this, "Export Error", "Failed to finalize the PDF file.\nCheck disk space or file permissions.");
    } else {
        qDebug() << "PDF Export finished successfully.";
        QMessageBox::information(this, "Export Successful", QString("Found items report exported successfully to:\n%1").arg(fileName));
    }
}


// --- Add Button Press Animation (Unchanged) ---
void FoundItemsAdminWidget::onAddButtonPressed() {
    if (!addFoundItemButton) return;
    // Store original geometry only once
    if (!originalAddGeoStored) {
        originalAddButtonGeometry = addFoundItemButton->geometry();
        originalAddGeoStored = true;
    }
    if (!addButtonAnim || !originalAddButtonGeometry.isValid()) {
        return; // Animation not set up or geometry invalid
    }

    // Calculate pressed state geometry (slightly offset)
    QRect pressedGeometry = originalAddButtonGeometry;
    pressedGeometry.moveTopLeft(originalAddButtonGeometry.topLeft() + QPoint(1, 1)); // Move down and right 1px

    // Animate to pressed state
    addButtonAnim->stop(); // Stop any ongoing animation
    addButtonAnim->setStartValue(addFoundItemButton->geometry()); // Start from current position
    addButtonAnim->setEndValue(pressedGeometry);
    addButtonAnim->start();
}

// --- Add Button Release Animation (Unchanged) ---
void FoundItemsAdminWidget::onAddButtonReleased() {
    if (!addFoundItemButton) return;
    // Ensure animation is set up and original geometry is stored
    if (!addButtonAnim || !originalAddGeoStored || !originalAddButtonGeometry.isValid()) {
        // If original wasn't stored, maybe just snap back? Or do nothing.
        // For now, do nothing if state is inconsistent.
        return;
    }

    // Animate back to original state
    addButtonAnim->stop(); // Stop any ongoing animation
    addButtonAnim->setStartValue(addFoundItemButton->geometry()); // Start from current position
    addButtonAnim->setEndValue(originalAddButtonGeometry);
    addButtonAnim->start();
}


// --- Add Lost Item Report ---
// <<< WORKAROUND: Using direct SQL execution instead of prepared statements >>>
void FoundItemsAdminWidget::onAddLostItemReportClicked() {
    qDebug() << "Add Lost Item Report Clicked!";
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not valid or not open.");
        return;
    }

    AddLostItemDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString description = dialog.description();
        QString contact = dialog.contactInfo();
        QDate dateLost = dialog.dateLost(); // Can be invalid if user didn't select one
        QString location = dialog.location();
        QString statusStr = statusToString(ItemStatus::Reported); // Default status for new reports

        // Validation (already done in dialog, but double-check)
        if (description.isEmpty() || contact.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Description and Contact Info cannot be empty.");
            return;
        }

        // --- Get New ID using Sequence (Oracle specific) ---
        qlonglong newId = -1;
        QSqlQuery seqQuery(db);
        if (!seqQuery.exec("SELECT lost_items_seq.NEXTVAL FROM DUAL")) {
            qWarning() << "Failed to get next value from sequence 'lost_items_seq':" << seqQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not generate a new report ID from the database sequence.\n" + seqQuery.lastError().databaseText());
            return;
        }
        if (seqQuery.next()) { newId = seqQuery.value(0).toLongLong(); }
        else { /* Error handling */ return; }
        if (newId <= 0) { /* Error handling */ return; }
        qDebug() << "Generated new Lost Item Report ID:" << newId;
        // --- End Get New ID ---


        // --- Start Transaction ---
        if (!db.transaction()) {
            qWarning() << "Failed to start database transaction for Lost Item INSERT:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not start database transaction.\n" + db.lastError().text());
            return;
        }
        qDebug() << "Database transaction started for Lost Item INSERT.";
        // --- End Start Transaction ---

        // --- Execute Direct Insert ---
        // !!! WARNING: This is for DIAGNOSTIC PURPOSES ONLY due to SQL injection risks !!!
        QSqlQuery query(db);
        QString dateLostSql;
        if (dateLost.isValid()) {
            dateLostSql = QString("TO_DATE('%1', 'YYYY-MM-DD')").arg(dateLost.toString(Qt::ISODate));
        } else {
            dateLostSql = "NULL"; // Use NULL keyword if date is invalid
        }

        QString sqlStatement = QString(
                                   "INSERT INTO LOST_ITEMS (REPORT_ID, DESCRIPTION, DATE_LOST, LOCATION_LOST, CONTACT_INFO, STATUS) "
                                   "VALUES (%1, '%2', %3, '%4', '%5', '%6')")
                                   .arg(newId)
                                   .arg(escapeSql(description))
                                   .arg(dateLostSql) // Use NULL or TO_DATE(...)
                                   .arg(escapeSql(location))
                                   .arg(escapeSql(contact))
                                   .arg(escapeSql(statusStr));

        qDebug() << "Executing direct SQL (DIAGNOSTIC):" << sqlStatement;
        bool execOk = query.exec(sqlStatement);
        // !!! END WARNING !!!

        if (!execOk) {
            qWarning() << "Failed to execute direct insert for lost item report."; // Modified message
            qWarning() << "Qt Error:" << query.lastError().text();
            qWarning() << "DB Error:" << query.lastError().databaseText();
            qWarning() << "Driver Error:" << query.lastError().driverText();
            QString errorToShow = query.lastError().databaseText().isEmpty() ? query.lastError().text() : query.lastError().databaseText();
            QMessageBox::critical(this, "Database Error", "Failed to add the lost item report to the database (Direct Exec).\n" + errorToShow); // Modified message
            // --- Rollback Transaction ---
            if (!db.rollback()) {
                qWarning() << "Failed to rollback transaction after Lost Item INSERT error:" << db.lastError().text();
            } else {
                qDebug() << "Transaction rolled back after Lost Item INSERT error.";
            }
            // --- End Rollback Transaction ---
            return;
        } else {
            qDebug() << "Direct INSERT Lost Item query executed successfully."; // Modified message
            // --- Commit Transaction ---
            if (!db.commit()) {
                qWarning() << "Failed to commit transaction after Lost Item INSERT:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to commit the new lost item report to the database.\n" + db.lastError().databaseText());
                if (!db.rollback()) { qWarning() << "Failed to rollback transaction after Lost Item INSERT COMMIT error:" << db.lastError().text(); }
                else { qDebug() << "Transaction rolled back after Lost Item INSERT COMMIT error."; }
                return; // Don't add to model if commit failed
            } else {
                qDebug() << "Transaction committed successfully after Lost Item INSERT.";
                // --- Add to Model (Only after successful commit) ---
                if(lostItemModel) {
                    QList<QStandardItem *> newRowItems;
                    QStandardItem* idItem = new QStandardItem();
                    idItem->setData(QVariant(newId), Qt::DisplayRole);
                    idItem->setEditable(false);
                    newRowItems << idItem;
                    newRowItems << new QStandardItem(description);
                    newRowItems << new QStandardItem(dateLost.isValid() ? dateLost.toString(Qt::ISODate) : ""); // Show empty if date was null
                    newRowItems << new QStandardItem(contact);
                    newRowItems << new QStandardItem(statusStr);
                    for(int i = 1; i < newRowItems.count(); ++i) { if(newRowItems[i]) newRowItems[i]->setEditable(false); }
                    lostItemModel->appendRow(newRowItems);
                    qDebug() << "New lost item report added to model:" << description;
                    updateButtonStates(); // Update buttons
                    if(lostItemsTableView) lostItemsTableView->scrollToBottom(); // Scroll to new item
                    QMessageBox::information(this, "Success", QString("Lost item report for '%1...' added successfully.").arg(description.left(30)));
                } else {
                    qWarning("onAddLostItemReportClicked: lostItemModel is null! Cannot update view.");
                    QMessageBox::warning(this, "UI Warning", "Report added to database, but UI model is missing.");
                }
                // --- End Add to Model ---
            }
            // --- End Commit Transaction ---
        }
        // --- End Insert ---
    } else {
        qDebug() << "Add Lost Item Dialog Rejected / Cancelled";
    }
}


// --- Delete Lost Item Report ---
// <<< WORKAROUND: Using direct SQL execution instead of prepared statements >>>
void FoundItemsAdminWidget::onDeleteLostItemClicked() {
    qDebug() << "Delete Lost Item Report Clicked!";
    if (!lostItemsTableView || !lostItemsTableView->selectionModel() || !lostItemModel) { /* Error handling */ return; }
    QItemSelectionModel *selectionModel = lostItemsTableView->selectionModel();
    if (!selectionModel->hasSelection()) { /* Info message */ return; }
    QModelIndex selectedIndex = selectionModel->currentIndex();
    if (!selectedIndex.isValid()) return;
    int sourceRow = selectedIndex.row();
    if (sourceRow < 0 || sourceRow >= lostItemModel->rowCount()) { /* Error handling */ return; }
    QStandardItem* idItem = lostItemModel->item(sourceRow, COL_LOST_ID);
    if (!idItem) { /* Error handling */ return; }
    bool idOk; qlonglong currentId = idItem->data(Qt::DisplayRole).toLongLong(&idOk);
    if (!idOk || currentId <= 0) { /* Error handling */ return; }
    QString itemDesc = lostItemModel->item(sourceRow, COL_LOST_DESC) ? lostItemModel->item(sourceRow, COL_LOST_DESC)->text() : "Unknown Item";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete", QString("Are you sure you want to delete the lost item report for '%1'?").arg(itemDesc), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // --- Delete from Database ---
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isValid() || !db.isOpen()) { /* Error handling */ return; }

        // --- Start Transaction ---
        if (!db.transaction()) {
            qWarning() << "Failed to start database transaction for Lost Item DELETE:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Could not start database transaction.\n" + db.lastError().text());
            return;
        }
        qDebug() << "Database transaction started for Lost Item DELETE.";
        // --- End Start Transaction ---

        // --- Execute Direct Delete ---
        // !!! WARNING: This is for DIAGNOSTIC PURPOSES ONLY !!!
        QSqlQuery query(db);
        QString sqlStatement = QString("DELETE FROM LOST_ITEMS WHERE REPORT_ID = %1").arg(currentId);

        qDebug() << "Executing direct SQL (DIAGNOSTIC):" << sqlStatement;
        bool execOk = query.exec(sqlStatement);
        // !!! END WARNING !!!

        if (!execOk) {
            qWarning() << "Failed to execute direct delete for lost item report (ID: " << currentId << ")"; // Modified message
            qWarning() << "Qt Error:" << query.lastError().text();
            qWarning() << "DB Error:" << query.lastError().databaseText();
            qWarning() << "Driver Error:" << query.lastError().driverText();
            QString errorToShow = query.lastError().databaseText().isEmpty() ? query.lastError().text() : query.lastError().databaseText();
            QMessageBox::critical(this, "Database Error", "Failed to delete the lost item report from the database (Direct Exec).\n" + errorToShow); // Modified message
            // --- Rollback Transaction ---
            if (!db.rollback()) {
                qWarning() << "Failed to rollback transaction after Lost Item DELETE error:" << db.lastError().text();
            } else {
                qDebug() << "Transaction rolled back after Lost Item DELETE error.";
            }
            // --- End Rollback Transaction ---
            return;
        } else {
            qDebug() << "Direct DELETE Lost Item Report query executed successfully. Rows affected:" << query.numRowsAffected(); // Modified message
            // --- Commit Transaction ---
            if (!db.commit()) {
                qWarning() << "Failed to commit transaction after Lost Item DELETE:" << db.lastError().text();
                QMessageBox::critical(this, "Database Error", "Failed to commit the lost item report deletion to the database.\n" + db.lastError().databaseText());
                if (!db.rollback()) { qWarning() << "Failed to rollback transaction after Lost Item DELETE COMMIT error:" << db.lastError().text(); }
                else { qDebug() << "Transaction rolled back after Lost Item DELETE COMMIT error."; }
                return; // Don't remove from model if commit failed
            } else {
                qDebug() << "Transaction committed successfully after Lost Item DELETE.";
                // --- Remove from Model (Only after successful commit) ---
                if (query.numRowsAffected() == 0) {
                    qWarning() << "Delete query affected 0 rows for lost item report ID:" << currentId << "(Report might have been deleted externally).";
                    QMessageBox::warning(this, "Delete Warning", "The selected report might have already been deleted. Refreshing data.");
                    // Consider reloading data if needed
                }
                // Remove row regardless of numRowsAffected
                lostItemModel->removeRow(sourceRow);
                qDebug() << "Lost item report removed from model.";
                updateButtonStates();
                QMessageBox::information(this, "Success", QString("Lost item report '%1' deleted successfully.").arg(itemDesc));
                // --- End Remove from Model ---
            }
            // --- End Commit Transaction ---
        }
        // --- End Execute Direct Delete ---
        // --- End Delete from Database ---
    } else {
        qDebug() << "Deletion of lost item report cancelled by user.";
    }
}



// --- Update Button Enabled States (Unchanged) ---
void FoundItemsAdminWidget::updateButtonStates() {
    bool foundItemsPageActive = (mainStackedWidget && mainStackedWidget->currentWidget() == itemsManagementWidget);
    bool lostItemsPageActive = (mainStackedWidget && mainStackedWidget->currentWidget() == lostItemsWidget);

    // Found Items Page Buttons
    bool foundItemSelected = foundItemsPageActive &&
                             itemsTableView &&
                             itemsTableView->selectionModel() &&
                             itemsTableView->selectionModel()->hasSelection() &&
                             itemsTableView->selectionModel()->currentIndex().isValid();

    if(editItemButton) editItemButton->setEnabled(foundItemSelected);
    if(deleteItemButton) deleteItemButton->setEnabled(foundItemSelected);
    if(addFoundItemButton) addFoundItemButton->setEnabled(foundItemsPageActive); // Always enabled if page is active
    if(exportPdfButton) exportPdfButton->setEnabled(foundItemsPageActive); // Always enabled if page is active
    if(filterButton) filterButton->setEnabled(foundItemsPageActive); // Always enabled if page is active
    if(sortButton) sortButton->setEnabled(foundItemsPageActive); // Always enabled if page is active
    if(searchLineEdit) searchLineEdit->setEnabled(foundItemsPageActive); // Always enabled if page is active

    // Lost Items Page Buttons
    bool lostItemSelected = lostItemsPageActive &&
                            lostItemsTableView &&
                            lostItemsTableView->selectionModel() &&
                            lostItemsTableView->selectionModel()->hasSelection() &&
                            lostItemsTableView->selectionModel()->currentIndex().isValid();

    if(deleteLostItemButton) deleteLostItemButton->setEnabled(lostItemSelected); // Only enabled if item selected
    if(addLostReportButton) addLostReportButton->setEnabled(lostItemsPageActive); // Always enabled if page is active

    qDebug() << "Button states updated. FoundActive:" << foundItemsPageActive
             << " LostActive:" << lostItemsPageActive
             << " FoundSelected:" << foundItemSelected
             << " LostSelected:" << lostItemSelected;
}

// --- Sort Found Items (Unchanged) ---
void FoundItemsAdminWidget::onSortButtonClicked() {
    if (!proxyModel || !itemModel || !itemsTableView || !itemsTableView->horizontalHeader() || !sortButton) {
        qWarning("onSortButtonClicked: Missing required objects!");
        return;
    }

    Qt::SortOrder currentOrder = itemsTableView->horizontalHeader()->sortIndicatorOrder();
    int currentColumn = itemsTableView->horizontalHeader()->sortIndicatorSection();

    // Determine next sort column and order (Cycle: Date Asc -> Date Desc -> Desc Asc -> Desc Desc -> Date Asc ...)
    int nextColumn = COL_FOUND_DATE; // Default start
    Qt::SortOrder nextOrder = Qt::AscendingOrder;

    // Map view column index (which might change if columns are moved) to logical index
    // For simplicity, assume COL_FOUND_DATE and COL_FOUND_DESC are the only sortable columns for now
    // and their visual position matches their logical index if not hidden.

    if (currentColumn == COL_FOUND_DATE) {
        if (currentOrder == Qt::AscendingOrder) {
            // Currently Date Asc, switch to Date Desc
            nextColumn = COL_FOUND_DATE;
            nextOrder = Qt::DescendingOrder;
        } else {
            // Currently Date Desc, switch to Desc Asc
            nextColumn = COL_FOUND_DESC;
            nextOrder = Qt::AscendingOrder;
        }
    } else if (currentColumn == COL_FOUND_DESC) {
        if (currentOrder == Qt::AscendingOrder) {
            // Currently Desc Asc, switch to Desc Desc
            nextColumn = COL_FOUND_DESC;
            nextOrder = Qt::DescendingOrder;
        } else {
            // Currently Desc Desc, switch back to Date Asc
            nextColumn = COL_FOUND_DATE;
            nextOrder = Qt::AscendingOrder;
        }
    } else {
        // Currently unsorted or sorted by other column, start with Date Asc
        nextColumn = COL_FOUND_DATE;
        nextOrder = Qt::AscendingOrder;
    }

    // Apply the sort to the proxy model
    proxyModel->sort(nextColumn, nextOrder);

    // Update the button text/tooltip to reflect the new sort order
    QStandardItem* headerItem = itemModel->horizontalHeaderItem(nextColumn);
    QString sortColName = headerItem ? headerItem->text() : QString("Column %1").arg(nextColumn);
    QString sortOrderName = (nextOrder == Qt::AscendingOrder) ? "Asc" : "Desc";
    sortButton->setText(QString(" Sort: %1").arg(sortColName)); // Show current sort column
    sortButton->setToolTip(QString("Sorting by %1 (%2). Click to change.").arg(sortColName, sortOrderName));

    qDebug() << "Sort Button Clicked! Sorting Found Items by" << sortColName << sortOrderName;
}

// --- Sidebar Navigation (Unchanged) ---
void FoundItemsAdminWidget::onSidebarSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    Q_UNUSED(previous); // Not used currently
    if (!current || !mainStackedWidget) return;

    QWidget *targetWidget = nullptr;
    QString currentText = current->text();
    qDebug() << "Sidebar selection changed to:" << currentText;

    // Determine target widget based on sidebar item text
    if (currentText == "Found Items List") {
        targetWidget = itemsManagementWidget;
    } else if (currentText == "Reported Lost Items") {
        targetWidget = lostItemsWidget;
    } else if (currentText == "Statistics") {
        // Ensure stats are up-to-date before showing page
        // recalculateStatsFromModel(); // Might be redundant if data hasn't changed
        updateStats(); // Ensure chart is current
        targetWidget = statisticsWidget;
    } else {
        qWarning() << "Unknown sidebar item selected:" << currentText;
        targetWidget = itemsManagementWidget; // Default to first page on error
    }

    int targetIndex = mainStackedWidget->indexOf(targetWidget);
    int currentWidgetIndex = currentStackIndex; // Use stored index

    qDebug() << "Current Index:" << currentWidgetIndex << "Target Index:" << targetIndex
             << "Target Widget ObjName:" << (targetWidget ? targetWidget->objectName() : "null");

    // Perform slide transition only if target is valid and different from current
    if (targetWidget && targetIndex != currentWidgetIndex && targetIndex >= 0) {
        slideToWidget(targetWidget);
    } else if (targetWidget && targetIndex >=0 && targetIndex == currentWidgetIndex) {
        // Already on the target widget, do nothing or maybe refresh?
        qDebug() << "Target widget is same as current, no transition.";
        // Ensure correct index is set just in case
        mainStackedWidget->setCurrentIndex(targetIndex);
        currentStackIndex = targetIndex;
        updateButtonStates(); // Still update buttons
    } else {
        qWarning() << "Target widget invalid or index out of bounds, cannot transition.";
        // Optionally default to a known good state
        // mainStackedWidget->setCurrentWidget(itemsManagementWidget);
        // currentStackIndex = mainStackedWidget->currentIndex();
        // updateButtonStates();
    }
}


// --- Slide Transition Animation (Unchanged) ---
void FoundItemsAdminWidget::slideToWidget(QWidget *targetWidget) {
    if (!targetWidget || !mainStackedWidget || !slideTransitionGroup) {
        qWarning() << "slideToWidget called with invalid pointers.";
        return;
    }

    int targetIndex = mainStackedWidget->indexOf(targetWidget);
    int currentWidgetIndex = currentStackIndex; // Use the stored index

    if (targetIndex == currentWidgetIndex || targetIndex < 0) {
        qDebug() << "Slide target is same or invalid.";
        return; // No transition needed or possible
    }

    QWidget *currentWidget = mainStackedWidget->widget(currentWidgetIndex);
    if (!currentWidget) {
        qWarning() << "Slide outgoing widget is null at index" << currentWidgetIndex;
        // Cannot animate out, just switch directly
        mainStackedWidget->setCurrentIndex(targetIndex);
        currentStackIndex = targetIndex;
        updateButtonStates();
        return;
    }

    qDebug() << "Initiating slide from" << currentWidget->objectName() << "(index" << currentWidgetIndex
             << ") to" << targetWidget->objectName() << "(index" << targetIndex << ")";

    int width = mainStackedWidget->width(); // Use stack widget width for slide distance
    bool slideLeft = (targetIndex > currentWidgetIndex); // Sliding left if target index is higher

    // Stop previous animation and clear animations from group
    slideTransitionGroup->stop();
    slideTransitionGroup->clear(); // Remove previous animations

    // Animation for the outgoing widget
    QPropertyAnimation *animOut = new QPropertyAnimation(currentWidget, "pos", slideTransitionGroup);
    animOut->setDuration(350); // Animation duration in ms
    animOut->setStartValue(QPoint(0, 0)); // Start at current position
    animOut->setEndValue(slideLeft ? QPoint(-width, 0) : QPoint(width, 0)); // Slide off screen
    animOut->setEasingCurve(QEasingCurve::InOutQuad); // Smooth easing
    slideTransitionGroup->addAnimation(animOut);

    // Prepare the incoming widget
    targetWidget->move(slideLeft ? QPoint(width, 0) : QPoint(-width, 0)); // Position off-screen initially
    targetWidget->setVisible(true); // Make it visible before animation
    targetWidget->raise(); // Ensure it's on top

    // Animation for the incoming widget
    QPropertyAnimation *animIn = new QPropertyAnimation(targetWidget, "pos", slideTransitionGroup);
    animIn->setDuration(350);
    animIn->setStartValue(targetWidget->pos()); // Start from off-screen position
    animIn->setEndValue(QPoint(0, 0)); // Slide to final position (0, 0)
    animIn->setEasingCurve(QEasingCurve::InOutQuad);
    slideTransitionGroup->addAnimation(animIn);

    // Use QPointer for safety in lambda, in case widgets are deleted during animation
    QPointer<FoundItemsAdminWidget> safeThis = this;
    QPointer<QWidget> safeCurrentWidget = currentWidget;
    QPointer<QWidget> safeTargetWidget = targetWidget;
    QPointer<QStackedWidget> safeStack = mainStackedWidget;

    // Disconnect previous connection to avoid multiple calls if animation restarted quickly
    disconnect(slideTransitionGroup, &QParallelAnimationGroup::finished, nullptr, nullptr);
    // Connect the finished signal to clean up and update state
    connect(slideTransitionGroup, &QParallelAnimationGroup::finished, this,
            [safeThis, safeCurrentWidget, safeTargetWidget, safeStack, targetIndex]() {
                if (!safeThis) return; // Check if main widget still exists

                qDebug() << "Slide Animation finished.";

                // Hide the outgoing widget AFTER animation completes
                if (safeCurrentWidget) {
                    qDebug() << "Hiding outgoing widget:" << safeCurrentWidget->objectName();
                    safeCurrentWidget->setVisible(false);
                    safeCurrentWidget->move(0, 0); // Reset position for next time
                } else {
                    qDebug() << "Outgoing widget was deleted during animation.";
                }

                // Ensure target widget is correctly positioned and visible
                if (safeTargetWidget) {
                    qDebug() << "Ensuring target widget is visible:" << safeTargetWidget->objectName() << safeTargetWidget->isVisible();
                    safeTargetWidget->setVisible(true); // Should already be visible
                    safeTargetWidget->move(0,0); // Ensure final position is correct

                    // Update the stack's current index officially AFTER animation
                    if (safeStack && safeStack->currentIndex() != targetIndex) {
                        qWarning() << "Target index mismatch after animation! Forcing current index.";
                        safeStack->setCurrentIndex(targetIndex);
                    }
                    safeThis->currentStackIndex = targetIndex; // Update stored index
                    safeThis->updateButtonStates(); // Update buttons for the new active page
                } else {
                    qDebug() << "Target widget was deleted during animation.";
                    // Maybe revert to a default state if target is gone?
                    if(safeStack) safeStack->setCurrentIndex(0);
                    safeThis->currentStackIndex = 0;
                    safeThis->updateButtonStates();
                }
            });

    // IMPORTANT: Set the stack's current index BEFORE starting animation
    // This makes the target widget the 'active' one logically, even while animating.
    mainStackedWidget->setCurrentIndex(targetIndex);

    qDebug() << "Starting slideTransitionGroup...";
    slideTransitionGroup->start();
}


// --- Placeholder Slots (Unchanged) ---
void FoundItemsAdminWidget::onPrevDateClicked() {
    qDebug() << "Previous Date button clicked - Not implemented.";
    QMessageBox::information(this, "Not Implemented", "Date range navigation is not yet implemented.");
}
void FoundItemsAdminWidget::onNextDateClicked() {
    qDebug() << "Next Date button clicked - Not implemented.";
    QMessageBox::information(this, "Not Implemented", "Date range navigation is not yet implemented.");
}


// --- Update Table/Empty Label Visibility Animation (Unchanged) ---
void FoundItemsAdminWidget::updateTableVisibility() {
    // Check required components exist
    if (!proxyModel || !itemsTableView || !emptyTableLabel || !tableOpacityEffect || !emptyLabelOpacityEffect || !visibilityAnimationGroup || !tableResultsCountLabel) {
        qWarning() << "updateTableVisibility called with null members! Cannot perform visibility update.";
        // Fallback visibility setting without animation
        bool hasItemsFallback = (proxyModel && proxyModel->rowCount() > 0);
        if(itemsTableView) itemsTableView->setVisible(hasItemsFallback);
        if(emptyTableLabel) emptyTableLabel->setVisible(!hasItemsFallback);
        if(tableResultsCountLabel) tableResultsCountLabel->setText(QString("%1 results found").arg(proxyModel ? proxyModel->rowCount() : 0));
        return;
    }

    bool hasItems = proxyModel->rowCount() > 0;
    qDebug() << "updateTableVisibility called. Proxy model has items:" << hasItems << "(" << proxyModel->rowCount() << "rows)";


    // Stop any ongoing visibility animation
    visibilityAnimationGroup->stop();
    visibilityAnimationGroup->clear(); // Remove old animations

    // Get current opacities
    double currentTableOpacity = tableOpacityEffect->opacity();
    double currentLabelOpacity = emptyLabelOpacityEffect->opacity();

    // Determine target opacities
    double targetTableOpacity = hasItems ? 1.0 : 0.0;
    double targetLabelOpacity = hasItems ? 0.0 : 1.0;

    // Update results count label immediately
    tableResultsCountLabel->setText(QString("%1 results found").arg(proxyModel->rowCount()));

    // Animate only if target state is different from current state
    if (!qFuzzyCompare(currentTableOpacity, targetTableOpacity) || !qFuzzyCompare(currentLabelOpacity, targetLabelOpacity)) {
        qDebug() << "Animating table visibility. Table target:" << targetTableOpacity << "Label target:" << targetLabelOpacity;
        // Ensure both widgets are technically visible during animation (opacity handles visual)
        itemsTableView->setVisible(true);
        emptyTableLabel->setVisible(true);

        // Table fade animation
        QPropertyAnimation *tableAnim = new QPropertyAnimation(tableOpacityEffect, "opacity", visibilityAnimationGroup);
        tableAnim->setDuration(250); // Animation duration
        tableAnim->setStartValue(currentTableOpacity);
        tableAnim->setEndValue(targetTableOpacity);
        tableAnim->setEasingCurve(QEasingCurve::InOutQuad);
        visibilityAnimationGroup->addAnimation(tableAnim);

        // Label fade animation
        QPropertyAnimation *labelAnim = new QPropertyAnimation(emptyLabelOpacityEffect, "opacity", visibilityAnimationGroup);
        labelAnim->setDuration(250);
        labelAnim->setStartValue(currentLabelOpacity);
        labelAnim->setEndValue(targetLabelOpacity);
        labelAnim->setEasingCurve(QEasingCurve::InOutQuad);
        visibilityAnimationGroup->addAnimation(labelAnim);

        // Use QPointer for safety in lambda connection
        QPointer<FoundItemsAdminWidget> safeThis = this;
        // Disconnect previous connection first
        disconnect(visibilityAnimationGroup, &QParallelAnimationGroup::finished, nullptr, nullptr);
        // Connect finished signal to set final visibility state AFTER animation
        connect(visibilityAnimationGroup, &QParallelAnimationGroup::finished, this,
                [safeThis, targetTableOpacity, targetLabelOpacity, hasItems]() {
                    if (!safeThis) return; // Check if widget still exists
                    qDebug() << "Table visibility animation finished. Setting final states.";
                    // Set final opacities explicitly
                    if (safeThis->tableOpacityEffect) safeThis->tableOpacityEffect->setOpacity(targetTableOpacity);
                    if (safeThis->emptyLabelOpacityEffect) safeThis->emptyLabelOpacityEffect->setOpacity(targetLabelOpacity);
                    // Set final visibility based on whether items exist
                    if(safeThis->itemsTableView) safeThis->itemsTableView->setVisible(hasItems);
                    if(safeThis->emptyTableLabel) safeThis->emptyTableLabel->setVisible(!hasItems);
                }, Qt::QueuedConnection); // Use QueuedConnection for safety across threads if needed (though unlikely here)

        visibilityAnimationGroup->start(); // Start the animation group
    } else {
        qDebug() << "Table visibility already at target state. Setting directly.";
        // Already at target state, just ensure visibility is correct
        if(itemsTableView) itemsTableView->setVisible(hasItems);
        if(emptyTableLabel) emptyTableLabel->setVisible(!hasItems);
        // Ensure opacities are correct too
        if (tableOpacityEffect) tableOpacityEffect->setOpacity(targetTableOpacity);
        if (emptyLabelOpacityEffect) emptyLabelOpacityEffect->setOpacity(targetLabelOpacity);
    }
}


// --- UI Setup (Unchanged) ---
void FoundItemsAdminWidget::setupUi() {
    qDebug() << "setupUi started...";

    // --- Create Main Widgets ---
    sidebarWidget = new QWidget(this); // Parent to main widget
    sidebarWidget->setObjectName("sidebarWidget");

    itemsManagementWidget = new QWidget(this);
    itemsManagementWidget->setObjectName("itemsManagementPage");

    lostItemsWidget = new QWidget(this);
    lostItemsWidget->setObjectName("lostItemsPage");

    createStatisticsPage(); // Creates statisticsWidget and its contents
    Q_ASSERT(statisticsWidget != nullptr);

    mainStackedWidget = new QStackedWidget(this);
    mainStackedWidget->setObjectName("mainStackedWidget");

    // --- Sidebar Setup ---
    logoLabel = new QLabel(sidebarWidget); // Parent to sidebar
    logoLabel->setObjectName("logoLabel");
    QPixmap logoPixmap(":/icons/logo.png");
    if(logoPixmap.isNull()) {
        qWarning() << "Logo file ':/icons/logo.png' not found or invalid! Check resources.qrc";
        logoLabel->setText("Mallmate"); // Fallback text
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setStyleSheet("font-weight: bold; font-size: 14pt; padding: 10px; color: #333;");
    } else {
        logoLabel->setPixmap(logoPixmap.scaled(180, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
    }

    sidebarListWidget = new QListWidget(sidebarWidget);
    sidebarListWidget->setObjectName("sidebarListWidget");
    // Add items with icons
    QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/icons/list.png"), "Found Items List");
    sidebarListWidget->addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/icons/list.png"), "Reported Lost Items"); // Consider different icon?
    sidebarListWidget->addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem(QIcon(":/icons/stats.png"), "Statistics");
    sidebarListWidget->addItem(item3);
    sidebarListWidget->setIconSize(QSize(18, 18));
    sidebarListWidget->setCurrentRow(0); // Select first item initially

    supportButton = new QPushButton(QIcon(":/icons/support.png"), " Support", sidebarWidget);
    supportButton->setObjectName("supportButton");
    signOutButton = new QPushButton(QIcon(":/icons/signout.png"), " Sign Out", sidebarWidget);
    signOutButton->setObjectName("signOutButton");

    // Sidebar Layout
    sidebarLayout = new QVBoxLayout(); // No parent needed, will be set on sidebarWidget
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addSpacing(20);
    sidebarLayout->addWidget(sidebarListWidget);
    sidebarLayout->addStretch(1); // Pushes buttons to bottom
    sidebarLayout->addWidget(supportButton);
    sidebarLayout->addWidget(signOutButton);
    sidebarLayout->setContentsMargins(0, 10, 0, 10); // Margins for sidebar content
    sidebarWidget->setLayout(sidebarLayout); // Apply layout to sidebar

    // --- Found Items Page Setup ---
    itemsManagementLayout = new QVBoxLayout(); // Layout for this page
    itemsManagementLayout->setContentsMargins(25, 20, 25, 20);
    itemsManagementLayout->setSpacing(20);

    // Top Title/Controls Row
    titleLabel = new QLabel("Found Items Management", itemsManagementWidget); // Parent to page
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16pt;");

    searchLineEdit = new QLineEdit(itemsManagementWidget);
    searchLineEdit->setObjectName("searchLineEdit");
    searchLineEdit->setPlaceholderText("Search Found Item Descriptions...");
    searchLineEdit->setClearButtonEnabled(true);

    addFoundItemButton = new QPushButton(QIcon(":/icons/add.png"), " Add Found Item", itemsManagementWidget);
    addFoundItemButton->setObjectName("addFoundItemButton");
    addFoundItemButton->setIconSize(QSize(16, 16));
    addFoundItemButton->setToolTip("Add a new found item to the list");

    editItemButton = new QPushButton(QIcon(":/icons/edit.png"), " Edit Selected", itemsManagementWidget);
    editItemButton->setObjectName("editItemButton");
    editItemButton->setIconSize(QSize(16, 16));
    editItemButton->setToolTip("Edit the selected item in the table");

    deleteItemButton = new QPushButton(QIcon(":/icons/delete.png"), " Delete Selected", itemsManagementWidget);
    deleteItemButton->setObjectName("deleteItemButton");
    deleteItemButton->setIconSize(QSize(16, 16));
    deleteItemButton->setToolTip("Delete the selected item from the table");

    exportPdfButton = new QPushButton(QIcon(":/icons/export.png"), " Export PDF", itemsManagementWidget);
    exportPdfButton->setObjectName("exportPdfButton");
    exportPdfButton->setIconSize(QSize(16,16));
    exportPdfButton->setToolTip("Export the current table view to a PDF file");

    filterButton = new QPushButton(QIcon(":/icons/filter.png"), " Filter by Status", itemsManagementWidget);
    filterButton->setObjectName("filterButton");
    filterButton->setToolTip("Filter found items list by status");

    sortButton = new QPushButton(QIcon(":/icons/sort.png"), " Sort: Date Found", itemsManagementWidget); // Initial sort state text
    sortButton->setObjectName("sortButton");
    sortButton->setToolTip("Change sorting options (Date / Description)");

    editViewButton = new QPushButton(itemsManagementWidget); // Icon only button
    editViewButton->setObjectName("editViewButton");
    editViewButton->setIcon(QIcon(":/icons/pencil.png")); // Use pencil icon
    editViewButton->setFixedSize(32, 32); // Make it square
    editViewButton->setIconSize(QSize(16, 16));
    editViewButton->setToolTip("Edit view options (Not implemented)");
    editViewButton->setEnabled(false); // Disabled for now

    // Layout for Top Controls
    topControlsLayout = new QHBoxLayout();
    topControlsLayout->addWidget(titleLabel);
    topControlsLayout->addStretch(1);
    topControlsLayout->addWidget(searchLineEdit);
    topControlsLayout->addSpacing(10);
    topControlsLayout->addWidget(addFoundItemButton);
    topControlsLayout->addSpacing(5);
    topControlsLayout->addWidget(editItemButton);
    topControlsLayout->addSpacing(5);
    topControlsLayout->addWidget(deleteItemButton);
    topControlsLayout->addSpacing(5);
    topControlsLayout->addWidget(exportPdfButton);
    //topControlsLayout->addSpacing(5); // Remove spacing before icon button if desired
    //topControlsLayout->addWidget(editViewButton); // Add if needed

    // Stats Cards Row
    statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20);
    // Card 1: Total
    QFrame *card1 = new QFrame(itemsManagementWidget); card1->setObjectName("statsCard"); QVBoxLayout* card1Layout = new QVBoxLayout(card1);
    statsTotalLabel = new QLabel("Total Items", card1); statsTotalLabel->setObjectName("statsTitleLabel");
    statsTotalValue = new QLabel("0", card1); statsTotalValue->setObjectName("statsValueLabel");
    card1Layout->addWidget(statsTotalLabel); card1Layout->addWidget(statsTotalValue); card1Layout->setContentsMargins(15, 15, 15, 15);
    statsLayout->addWidget(card1);
    // Card 2: Unclaimed
    QFrame *card2 = new QFrame(itemsManagementWidget); card2->setObjectName("statsCard"); QVBoxLayout* card2Layout = new QVBoxLayout(card2);
    statsUnclaimedLabel = new QLabel("Unclaimed", card2); statsUnclaimedLabel->setObjectName("statsTitleLabel");
    statsUnclaimedValue = new QLabel("0", card2); statsUnclaimedValue->setObjectName("statsValueLabel");
    card2Layout->addWidget(statsUnclaimedLabel); card2Layout->addWidget(statsUnclaimedValue); card2Layout->setContentsMargins(15, 15, 15, 15);
    statsLayout->addWidget(card2);
    // Card 3: Claimed
    QFrame *card3 = new QFrame(itemsManagementWidget); card3->setObjectName("statsCard"); QVBoxLayout* card3Layout = new QVBoxLayout(card3);
    statsClaimedLabel = new QLabel("Claimed", card3); statsClaimedLabel->setObjectName("statsTitleLabel");
    statsClaimedValue = new QLabel("0", card3); statsClaimedValue->setObjectName("statsValueLabel");
    card3Layout->addWidget(statsClaimedLabel); card3Layout->addWidget(statsClaimedValue); card3Layout->setContentsMargins(15, 15, 15, 15);
    statsLayout->addWidget(card3);
    // Card 4: Returned
    QFrame *card4 = new QFrame(itemsManagementWidget); card4->setObjectName("statsCard"); QVBoxLayout* card4Layout = new QVBoxLayout(card4);
    statsReturnedLabel = new QLabel("Returned", card4); statsReturnedLabel->setObjectName("statsTitleLabel");
    statsReturnedValue = new QLabel("0", card4); statsReturnedValue->setObjectName("statsValueLabel");
    card4Layout->addWidget(statsReturnedLabel); card4Layout->addWidget(statsReturnedValue); card4Layout->setContentsMargins(15, 15, 15, 15);
    statsLayout->addWidget(card4);
    statsLayout->addStretch(1); // Push cards to the left

    // Table Area Title / Controls
    tableAreaTitleLabel = new QLabel("Found Items List", itemsManagementWidget);
    tableResultsCountLabel = new QLabel("0 results found", itemsManagementWidget); // Will be updated
    prevDateButton = new QPushButton("<", itemsManagementWidget);
    prevDateButton->setToolTip("Previous month/period (Not implemented)"); prevDateButton->setFixedWidth(30); prevDateButton->setObjectName("prevDateButton"); prevDateButton->setEnabled(false);
    dateLabel = new QLabel("All Time", itemsManagementWidget); dateLabel->setAlignment(Qt::AlignCenter); dateLabel->setObjectName("dateLabel");
    nextDateButton = new QPushButton(">", itemsManagementWidget);
    nextDateButton->setToolTip("Next month/period (Not implemented)"); nextDateButton->setFixedWidth(30); nextDateButton->setObjectName("nextDateButton"); nextDateButton->setEnabled(false);

    // Layout for Table Controls
    tableControlsLayout = new QHBoxLayout();
    tableControlsLayout->addWidget(tableAreaTitleLabel);
    tableControlsLayout->addSpacing(5);
    tableControlsLayout->addWidget(tableResultsCountLabel);
    tableControlsLayout->addStretch(1);
    tableControlsLayout->addWidget(filterButton); // Add filter button here
    tableControlsLayout->addSpacing(5);
    tableControlsLayout->addWidget(sortButton); // Add sort button here
    tableControlsLayout->addSpacing(20);
    //tableControlsLayout->addWidget(prevDateButton); // Add date controls if needed
    //tableControlsLayout->addWidget(dateLabel);
    //tableControlsLayout->addWidget(nextDateButton);
    tableControlsLayout->setContentsMargins(0, 5, 0, 5);

    // Table View and Empty Label
    itemsTableView = new QTableView(itemsManagementWidget);
    itemsTableView->setObjectName("itemsTableView");
    itemsTableView->horizontalHeader()->setStretchLastSection(false); // Don't stretch last column
    itemsTableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Select whole rows
    itemsTableView->setAlternatingRowColors(true); // Zebra striping
    itemsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Don't allow editing in table
    itemsTableView->setSelectionMode(QAbstractItemView::SingleSelection); // Only allow one row selected
    itemsTableView->setShowGrid(true); // Show grid lines
    itemsTableView->verticalHeader()->setVisible(false); // Hide row numbers
    itemsTableView->setWordWrap(false); // Prevent text wrapping in cells (can be slow)

    emptyTableLabel = new QLabel("No found items match your current filter.", itemsManagementWidget);
    emptyTableLabel->setObjectName("emptyTableLabel");
    emptyTableLabel->setAlignment(Qt::AlignCenter);
    emptyTableLabel->setVisible(false); // Initially hidden

    // Layout for Table Section (Controls + Table/Empty Label)
    tableSectionLayout = new QVBoxLayout();
    tableSectionLayout->addLayout(tableControlsLayout);
    tableSectionLayout->addSpacing(10);
    // Use a QStackedWidget to easily switch between table and empty label?
    // For now, just add both and toggle visibility.
    tableSectionLayout->addWidget(itemsTableView);
    tableSectionLayout->addWidget(emptyTableLabel);
    // Ensure table stretches, label doesn't (or vice versa depending on visibility)
    tableSectionLayout->setStretchFactor(itemsTableView, 1); // Table takes available space
    tableSectionLayout->setStretchFactor(emptyTableLabel, 1); // Label also takes space when visible


    // Add all sections to the itemsManagementLayout
    itemsManagementLayout->addLayout(topControlsLayout);
    itemsManagementLayout->addLayout(statsLayout);
    itemsManagementLayout->addLayout(tableSectionLayout);
    itemsManagementWidget->setLayout(itemsManagementLayout); // Apply layout to page


    // --- Lost Items Page Setup ---
    lostItemsLayout = new QVBoxLayout(); // Layout for this page
    lostItemsLayout->setContentsMargins(25, 20, 25, 20);
    lostItemsLayout->setSpacing(20);

    QLabel *lostItemsTitle = new QLabel("Reported Lost Items", lostItemsWidget); // Parent to page
    lostItemsTitle->setObjectName("lostItemsTitle");
    lostItemsTitle->setStyleSheet("font-weight: bold; font-size: 16pt;");

    addLostReportButton = new QPushButton(QIcon(":/icons/add.png"), " Add Lost Item Report", lostItemsWidget);
    addLostReportButton->setObjectName("addLostReportButton");
    addLostReportButton->setToolTip("Report a new lost item");

    deleteLostItemButton = new QPushButton(QIcon(":/icons/delete.png"), " Delete Report", lostItemsWidget);
    deleteLostItemButton->setObjectName("deleteLostItemButton");
    deleteLostItemButton->setToolTip("Delete the selected lost item report");
    deleteLostItemButton->setEnabled(false); // Initially disabled

    // Layout for Lost Items Controls (e.g., Add/Delete buttons)
    QHBoxLayout *lostItemsControlsLayout = new QHBoxLayout();
    lostItemsControlsLayout->addStretch(1); // Push buttons to the right
    lostItemsControlsLayout->addWidget(deleteLostItemButton); // Delete first maybe? Or Add first?
    lostItemsControlsLayout->addWidget(addLostReportButton);

    lostItemsTableView = new QTableView(lostItemsWidget); // Parent to page
    lostItemsTableView->setObjectName("lostItemsTableView");
    // Configure lost items table view
    lostItemsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    lostItemsTableView->setAlternatingRowColors(true);
    lostItemsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lostItemsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    lostItemsTableView->setShowGrid(true);
    lostItemsTableView->verticalHeader()->setVisible(false);
    lostItemsTableView->setWordWrap(false);

    // Add widgets to lost items layout
    lostItemsLayout->addWidget(lostItemsTitle);
    lostItemsLayout->addLayout(lostItemsControlsLayout);
    lostItemsLayout->addWidget(lostItemsTableView, 1); // Table takes stretch factor 1
    lostItemsWidget->setLayout(lostItemsLayout); // Apply layout to page


    // --- Stacked Widget Setup ---
    Q_ASSERT(itemsManagementWidget != nullptr);
    mainStackedWidget->addWidget(itemsManagementWidget);
    Q_ASSERT(lostItemsWidget != nullptr);
    mainStackedWidget->addWidget(lostItemsWidget);
    Q_ASSERT(statisticsWidget != nullptr);
    mainStackedWidget->addWidget(statisticsWidget);

    // --- Main Layout Setup ---
    mainLayout = new QHBoxLayout(); // Main layout for the whole widget
    Q_ASSERT(sidebarWidget != nullptr);
    mainLayout->addWidget(sidebarWidget); // Add sidebar (fixed width set by QSS)
    Q_ASSERT(mainStackedWidget != nullptr);
    mainLayout->addWidget(mainStackedWidget, 1); // Add stacked widget (takes stretch factor 1)
    mainLayout->setContentsMargins(0, 0, 0, 0); // No margins around main layout
    mainLayout->setSpacing(0); // No spacing between sidebar and content

    // Apply main layout to this widget (FoundItemsAdminWidget)
    // Check if layout already exists (shouldn't in constructor)
    if (this->layout()) {
        qWarning() << "setupUi Warning: Widget already has a layout! Deleting previous layout.";
        delete this->layout();
    }
    this->setLayout(mainLayout);
    Q_ASSERT(this->layout() != nullptr); // Ensure layout was set

    qDebug() << "setupUi finished.";
}

// --- Create Statistics Page (Unchanged) ---
void FoundItemsAdminWidget::createStatisticsPage() {
    qDebug() << "createStatisticsPage started...";
    statisticsWidget = new QWidget(this); // Parent to main widget
    statisticsWidget->setObjectName("statisticsPage");

    statsPageMainLayout = new QVBoxLayout(); // Layout for this page
    statsPageMainLayout->setContentsMargins(25, 20, 25, 20);
    statsPageMainLayout->setSpacing(20);

    QLabel *statsPageTitle = new QLabel("Found Item Statistics", statisticsWidget); // Parent to page
    statsPageTitle->setObjectName("statsPageTitle");
    statsPageTitle->setStyleSheet("font-weight: bold; font-size: 16pt;");

    // Create Chart View - Chart itself is created/updated in updateStats()
    statsChartView = new QChartView(statisticsWidget); // Parent to page
    statsChartView->setRenderHint(QPainter::Antialiasing); // Enable anti-aliasing for smoother chart
    statsChartView->setMinimumSize(400, 300); // Set a minimum size

    // Add widgets to layout
    statsPageMainLayout->addWidget(statsPageTitle);
    statsPageMainLayout->addWidget(statsChartView, 1); // Chart view takes stretch factor 1

    statisticsWidget->setLayout(statsPageMainLayout); // Apply layout to page
    qDebug() << "createStatisticsPage finished.";
}

// --- Setup Animations ---
void FoundItemsAdminWidget::setupAnimations() {
    qDebug() << "setupAnimations started...";

    // Opacity effect for Found Items Table
    if (itemsTableView) {
        tableOpacityEffect = new QGraphicsOpacityEffect(itemsTableView);
        itemsTableView->setGraphicsEffect(tableOpacityEffect); // Apply effect TO the table view
        tableOpacityEffect->setOpacity(1.0); // Start fully opaque (visibility controlled by updateTableVisibility)
    } else {
        tableOpacityEffect = nullptr;
        qWarning("setupAnimations: itemsTableView is null!");
    }

    // Opacity effect for Empty Table Label
    if (emptyTableLabel) {
        emptyLabelOpacityEffect = new QGraphicsOpacityEffect(emptyTableLabel);
        // *** CORRECTED LINE BELOW ***
        emptyTableLabel->setGraphicsEffect(emptyLabelOpacityEffect); // Apply effect TO the label
        emptyLabelOpacityEffect->setOpacity(0.0); // Start fully transparent
    } else {
        emptyLabelOpacityEffect = nullptr;
        qWarning("setupAnimations: emptyTableLabel is null!");
    }

    // Animation group for table/label visibility fade
    visibilityAnimationGroup = new QParallelAnimationGroup(this); // Parented to main widget

    // Animation for Add button press/release
    if (addFoundItemButton) {
        addButtonAnim = new QPropertyAnimation(addFoundItemButton, "geometry", this); // Parented
        addButtonAnim->setDuration(100); // Short duration
        addButtonAnim->setEasingCurve(QEasingCurve::OutQuad);
    } else {
        addButtonAnim = nullptr;
        qWarning("setupAnimations: addFoundItemButton is null!");
    }
    originalAddGeoStored = false; // Reset flag

    // Animation group for page slide transitions
    slideTransitionGroup = new QParallelAnimationGroup(this); // Parented to main widget

    qDebug() << "setupAnimations finished.";
}
