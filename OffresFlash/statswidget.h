#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget> // Base class include

// Include necessary Qt Charts header
#include <QtCharts/QChartView>

// Forward declarations for standard Qt Widgets
class QLabel;
class QGridLayout; // Added for metrics layout

// Renamed class from StatsDialog to StatsWidget
class StatsWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the statistics widget.
     * @param activeCount Number of active offers.
     * @param finishedCount Number of finished offers.
     * @param validatedCount Number of validated offers.
     * @param pendingCount Number of pending offers.   // <<< NEW
     * @param rejectedCount Number of rejected offers. // <<< NEW
     * @param totalClaims Sum of claims for all offers. // <<< NEW
     * @param parent The parent widget (optional).
     */
    // Added new counts to constructor parameters
    explicit StatsWidget(int activeCount, int finishedCount, int validatedCount,
                         int pendingCount, int rejectedCount, int totalClaims, // <<< NEW PARAMS
                         QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~StatsWidget() override;

    // Optional: Add a function to update the chart and metrics if needed later
    // void updateStats(int activeCount, int finishedCount, int validatedCount,
    //                  int pendingCount, int rejectedCount, int totalClaims);

private:
    // --- UI Element Pointers ---
    QLabel* m_totalOffersLabel = nullptr;   // Label for total offers metric <<< RENAMED
    QLabel* m_totalClaimsLabel = nullptr;   // Label for total claims metric <<< NEW
    QChartView* m_chartView = nullptr;      // Chart view pointer
    QGridLayout* m_metricsLayout = nullptr; // Layout for key metrics <<< NEW

    // --- Helper Functions ---
    /**
     * @brief Sets up the UI layout, creating and displaying the chart and metrics.
     * Called by the constructor.
     * @param activeCount Number of active offers.
     * @param finishedCount Number of finished offers.
     * @param validatedCount Number of validated offers.
     * @param pendingCount Number of pending offers.   // <<< NEW
     * @param rejectedCount Number of rejected offers. // <<< NEW
     * @param totalClaims Sum of claims for all offers. // <<< NEW
     */
    // Added new counts to setupUi parameters
    void setupUi(int activeCount, int finishedCount, int validatedCount,
                 int pendingCount, int rejectedCount, int totalClaims); // <<< NEW PARAMS
};

#endif // STATSWIDGET_H
