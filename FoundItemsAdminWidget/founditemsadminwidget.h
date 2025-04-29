#ifndef FOUNDITEMSADMINWIDGET_H
#define FOUNDITEMSADMINWIDGET_H

#include <QWidget>
#include <QRect>
#include <QString>
// <<< Removed SQL includes - Now handled in databaseconnection.h >>>

// Forward declarations
class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QTableView;
class QVBoxLayout;
class QHBoxLayout;
class QListWidgetItem;
class QFrame;
class QStandardItemModel;
class QItemSelectionModel;
class QSortFilterProxyModel;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QStackedWidget;
class QFormLayout;
class QChartView; // Renamed from QtCharts::QChartView due to QT_CHARTS_USE_NAMESPACE

// Define an enum for item statuses (Can be shared or defined separately)
enum class ItemStatus {
    Unclaimed, // Found item status
    Claimed,   // Found item status
    Returned,  // Found item status
    Disposed,  // Found item status
    Reported,  // Lost item status
    Found,     // Lost item status (marked as found)
    Unknown
};

// Column indices constants (for Found Items Model) - Adding ID
const int COL_FOUND_ID = 0; // Hidden ID column
const int COL_FOUND_DESC = 1;
const int COL_FOUND_DATE = 2;
const int COL_FOUND_LOC = 3;
const int COL_FOUND_STATUS = 4;
const int FOUND_MODEL_COLUMN_COUNT = 5; // Total columns including hidden ID

// Column indices constants (for Lost Items Model) - Adding ID
const int COL_LOST_ID = 0; // Hidden ID column
const int COL_LOST_DESC = 1;
const int COL_LOST_DATE = 2;
const int COL_LOST_CONTACT = 3; // Shifted
const int COL_LOST_STATUS = 4; // Shifted
const int LOST_MODEL_COLUMN_COUNT = 5; // Total columns including hidden ID


class FoundItemsAdminWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FoundItemsAdminWidget(QWidget *parent = nullptr);
    ~FoundItemsAdminWidget();

private:
    // --- Database ---
    // <<< REMOVED QSqlDatabase m_db; >>>

    // --- Widgets ---
    // Sidebar (Unchanged)
    QWidget *sidebarWidget = nullptr; QLabel *logoLabel = nullptr; QListWidget *sidebarListWidget = nullptr; QPushButton *supportButton = nullptr; QPushButton *signOutButton = nullptr;

    // Main Area Containers (Unchanged)
    QStackedWidget *mainStackedWidget = nullptr;
    QWidget *itemsManagementWidget = nullptr; // Found Items page
    QWidget *lostItemsWidget = nullptr;       // Lost Items page
    QWidget *statisticsWidget = nullptr;      // Statistics page

    // Widgets within itemsManagementWidget (Found Items - Unchanged)
    QLabel *titleLabel = nullptr; QLineEdit *searchLineEdit = nullptr; QPushButton *addFoundItemButton = nullptr; QPushButton *editItemButton = nullptr; QPushButton *deleteItemButton = nullptr; QPushButton *exportPdfButton = nullptr; QPushButton *filterButton = nullptr; QPushButton *sortButton = nullptr; QPushButton *editViewButton = nullptr;
    QLabel *statsTotalLabel = nullptr; QLabel *statsTotalValue = nullptr; QLabel *statsUnclaimedLabel = nullptr; QLabel *statsUnclaimedValue = nullptr; QLabel *statsClaimedLabel = nullptr; QLabel *statsClaimedValue = nullptr; QLabel *statsReturnedLabel = nullptr; QLabel *statsReturnedValue = nullptr;
    QLabel *tableAreaTitleLabel = nullptr; QLabel *tableResultsCountLabel = nullptr; QPushButton *prevDateButton = nullptr; QLabel *dateLabel = nullptr; QPushButton *nextDateButton = nullptr;
    QTableView *itemsTableView = nullptr; QLabel *emptyTableLabel = nullptr;

    // Widgets within lostItemsWidget (Lost Items)
    QTableView *lostItemsTableView = nullptr; // Declared first in .h
    QPushButton *addLostReportButton = nullptr; // <<< NEW
    QPushButton *deleteLostItemButton = nullptr; // <<< NEW

    // Widgets within statisticsWidget (Unchanged)
    QChartView *statsChartView = nullptr; // Declared after lostItemsTableView

    // --- Layouts --- (Unchanged)
    QHBoxLayout *mainLayout = nullptr; QVBoxLayout *sidebarLayout = nullptr;
    QVBoxLayout *itemsManagementLayout = nullptr; QHBoxLayout *topControlsLayout = nullptr; QHBoxLayout *statsLayout = nullptr; QHBoxLayout *tableControlsLayout = nullptr; QVBoxLayout *tableSectionLayout = nullptr;
    QVBoxLayout *lostItemsLayout = nullptr;
    QVBoxLayout *statsPageMainLayout = nullptr;

    // --- Models ---
    QStandardItemModel *itemModel = nullptr;      // For Found Items
    QSortFilterProxyModel *proxyModel = nullptr;  // For Found Items Filtering/Sorting
    QStandardItemModel *lostItemModel = nullptr;  // For Lost Items

    // --- Effects & Animations --- (Unchanged)
    QGraphicsOpacityEffect *tableOpacityEffect = nullptr; QGraphicsOpacityEffect *emptyLabelOpacityEffect = nullptr; QParallelAnimationGroup *visibilityAnimationGroup = nullptr;
    QPropertyAnimation *addButtonAnim = nullptr; QRect originalAddButtonGeometry; bool originalAddGeoStored = false;
    QParallelAnimationGroup *slideTransitionGroup = nullptr;
    int currentStackIndex = 0;

    // --- Statistics Counts (Found Items Only) ---
    int m_totalCount = 0; int m_unclaimedCount = 0; int m_claimedCount = 0; int m_returnedCount = 0; int m_disposedCount = 0;

    // --- Helper Methods ---
    // <<< REMOVED bool initializeDatabase(); >>>
    void setupUi();
    void setupModelsAndData(); // Will be modified to load from DB
    void updateTableVisibility(); // Updates found items table visibility
    void updateStats(); // Updates found items stats
    void setupAnimations();
    void createStatisticsPage();
    void slideToWidget(QWidget *targetWidget);

    // Stats and Status Helpers
    void loadFoundItemsFromDb(); // <<< Specific load function
    void loadLostItemsFromDb();  // <<< Specific load function
    void recalculateStatsFromModel(); // Calculates found items stats
    void updateStatsCount(ItemStatus oldStatus, ItemStatus newStatus); // Updates found items stats counts
    QString statusToString(ItemStatus status) const;
    ItemStatus stringToStatus(const QString& statusStr) const;

private slots:
    // Found Item Slots
    void onAddFoundItemClicked();  // Modified for DB
    void onEditItemClicked();    // Modified for DB
    void onDeleteItemClicked();  // Modified for DB
    void onExportPdfClicked();
    void onFilterButtonClicked();
    void onSortButtonClicked();
    void onSearchTextChanged(const QString &text);
    void onAddButtonPressed(); // Found item add button animation
    void onAddButtonReleased(); // Found item add button animation

    // Lost Item Slots <<< NEW
    void onAddLostItemReportClicked(); // Modified for DB
    void onDeleteLostItemClicked();    // Modified for DB
    // void onMarkLostItemFoundClicked(); // Future enhancement
    // void onEditLostItemClicked(); // Future enhancement

    // Common Slots
    void updateButtonStates(); // Needs update for lost items page
    void onSidebarSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onPrevDateClicked(); // Placeholder
    void onNextDateClicked(); // Placeholder
};

#endif // FOUNDITEMSADMINWIDGET_H
