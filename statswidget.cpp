#include "statswidget.h" // <<< ENSURE THIS IS THE VERY FIRST INCLUDE

// --- Standard Qt Headers ---
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPainter>
#include <QColor>
#include <QFont>

// --- Qt Charts Headers ---
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>

// --- Constructor (Enhanced) ---
// Use correct class name qualification: StatsWidget::
StatsWidget::StatsWidget(int activeCount, int finishedCount, int validatedCount,
                         int pendingCount, int rejectedCount, int totalClaims,
                         QWidget *parent) :
    QWidget(parent), // Base class initializer
    m_totalOffersLabel(nullptr), // Initialize members to nullptr
    m_totalClaimsLabel(nullptr),
    m_chartView(nullptr),
    m_metricsLayout(nullptr)
{
    qDebug() << "StatsWidget Constructor: Initializing with counts - Active:" << activeCount
             << "Finished:" << finishedCount << "Validated:" << validatedCount
             << "Pending:" << pendingCount << "Rejected:" << rejectedCount
             << "Total Claims:" << totalClaims;

    // Create the UI elements and layout using the new counts
    setupUi(activeCount, finishedCount, validatedCount,
            pendingCount, rejectedCount, totalClaims);

    qDebug() << "StatsWidget Constructor: Finished.";
}

// --- Destructor ---
// Use correct class name qualification: StatsWidget::
StatsWidget::~StatsWidget()
{
    qDebug() << "StatsWidget Destructor called.";
    // Qt's parent-child mechanism handles deletion of widgets/layouts
}

// --- setupUi Method (Enhanced) ---
// Use correct class name qualification: StatsWidget::
void StatsWidget::setupUi(int activeCount, int finishedCount, int validatedCount,
                          int pendingCount, int rejectedCount, int totalClaims)
{
    // --- 1. Main Layout ---
    // 'this' is valid here because setupUi is a member function
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // --- 2. Calculations ---
    int totalCount = activeCount + finishedCount + validatedCount + pendingCount + rejectedCount;
    qDebug() << "StatsWidget setupUi: Total offers =" << totalCount;

    // --- 3. Title Label ---
    // 'this' is valid here
    QLabel *titleLabel = new QLabel("<b>Statistiques des Offres</b>", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // --- 4. Create Pie Chart Series ---
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setLabelsPosition(QPieSlice::LabelInsideHorizontal);
    pieSeries->setPieSize(0.7);

    QColor activeColor(144, 238, 144, 220);
    QColor finishedColor(211, 211, 211, 220);
    QColor validatedColor(173, 216, 230, 220);
    QColor pendingColor(255, 253, 208, 220);
    QColor rejectedColor(240, 128, 128, 220);

    // Add slices (using local variable 'slice', not member)
    if (activeCount > 0) { QPieSlice *slice = pieSeries->append(QString("Actives (%1)").arg(activeCount), activeCount); slice->setColor(activeColor); slice->setLabelVisible(true); }
    if (pendingCount > 0) { QPieSlice *slice = pieSeries->append(QString("En Attente (%1)").arg(pendingCount), pendingCount); slice->setColor(pendingColor); slice->setLabelVisible(true); }
    if (validatedCount > 0) { QPieSlice *slice = pieSeries->append(QString("Validées (%1)").arg(validatedCount), validatedCount); slice->setColor(validatedColor); slice->setLabelVisible(true); }
    if (rejectedCount > 0) { QPieSlice *slice = pieSeries->append(QString("Rejetées (%1)").arg(rejectedCount), rejectedCount); slice->setColor(rejectedColor); slice->setLabelVisible(true); }
    if (finishedCount > 0) { QPieSlice *slice = pieSeries->append(QString("Terminées (%1)").arg(finishedCount), finishedCount); slice->setColor(finishedColor); slice->setLabelVisible(true); }

    if (totalCount == 0) {
        QPieSlice *slice = pieSeries->append("Aucune offre", 1);
        slice->setColor(Qt::lightGray); slice->setLabelVisible(true); slice->setExploded(false);
    } else {
        QPieSlice *largestSlice = nullptr; qreal maxVal = 0;
        for(QPieSlice *s : pieSeries->slices()) { if(s->value() > maxVal) { maxVal = s->value(); largestSlice = s; } }
        if(largestSlice) { largestSlice->setExploded(true); largestSlice->setLabelVisible(true); }
    }

    // --- 5. Create Chart Object ---
    QChart *chart = new QChart();
    chart->addSeries(pieSeries); // The chart takes ownership of the series
    chart->setTitle("Répartition par Statut");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);

    // --- 6. Create Chart View Widget ---
    // Use member variable m_chartView correctly
    // The chart view takes ownership of the chart
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(m_chartView, 1); // Add chart view to stretch

    // --- 7. Metrics Section ---
    // Use member variable m_metricsLayout correctly
    m_metricsLayout = new QGridLayout();
    m_metricsLayout->setContentsMargins(20, 10, 20, 10);
    m_metricsLayout->setHorizontalSpacing(20);
    m_metricsLayout->setVerticalSpacing(5);

    // 'this' is valid here
    QLabel* totalOffersTitleLabel = new QLabel("Offres Totales:", this);
    // Use member variable m_totalOffersLabel correctly
    // 'this' is valid here
    m_totalOffersLabel = new QLabel(QString::number(totalCount), this);
    QFont metricFont = m_totalOffersLabel->font();
    metricFont.setPointSize(metricFont.pointSize() + 1);
    metricFont.setBold(true);
    m_totalOffersLabel->setFont(metricFont);
    m_totalOffersLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    totalOffersTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 'this' is valid here
    QLabel* totalClaimsTitleLabel = new QLabel("Réclamations Totales:", this);
    // Use member variable m_totalClaimsLabel correctly
    // 'this' is valid here
    m_totalClaimsLabel = new QLabel(QString::number(totalClaims), this);
    m_totalClaimsLabel->setFont(metricFont);
    m_totalClaimsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    totalClaimsTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_metricsLayout->addWidget(totalOffersTitleLabel, 0, 0);
    m_metricsLayout->addWidget(m_totalOffersLabel, 0, 1);
    m_metricsLayout->addWidget(totalClaimsTitleLabel, 1, 0);
    m_metricsLayout->addWidget(m_totalClaimsLabel, 1, 1);
    m_metricsLayout->setColumnStretch(0, 0);
    m_metricsLayout->setColumnStretch(1, 1);

    mainLayout->addLayout(m_metricsLayout);

    qDebug() << "StatsWidget setupUi: UI setup complete.";
}

