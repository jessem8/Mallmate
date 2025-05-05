#include "offeritemwidget.h" // Include the header for this class
#include "offerdata.h"         // Include the CORRECTED OfferData definition

// Include necessary Qt headers
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>     // Needed for the toggle switch
#include <QString>
#include <QFontMetrics> // Needed for potential text eliding (optional improvement)
#include <QDateTime>    // Needed for date formatting
#include <QtDebug>      // For qWarning and qDebug

// Constructor: Calls setupUi, populates data, initializes members, connects signal
OfferItemWidget::OfferItemWidget(const OfferData &data, QWidget *parent)
    : QWidget(parent), // Base class constructor
    discountLabel(nullptr), // Initialize pointers to nullptr
    titleLabel(nullptr),
    dateLabel(nullptr),
    storeLabel(nullptr),
    statusLabel(nullptr),
    claimCountLabel(nullptr), // <<< Initialize new label pointer
    offerToggleButton(nullptr),
    m_offerId(data.offreId) // Initialize m_offerId using data.offreId
{
    setupUi();        // Create the labels, toggle, and layout
    populateData(data); // Fill the widgets with the provided offer data

    // Connect the toggle button's toggled signal to our internal slot
    if (offerToggleButton) {
        connect(offerToggleButton, &QCheckBox::toggled, this, &OfferItemWidget::onToggleClicked);
    } else {
        qWarning() << "OfferItemWidget::OfferItemWidget - offerToggleButton is null, cannot connect signal.";
    }
}

// Destructor: Default is usually fine
OfferItemWidget::~OfferItemWidget()
{
    // Child widgets and layouts are deleted automatically by Qt's parent-child hierarchy
}

// setupUi: Creates the labels and arranges them in layouts
void OfferItemWidget::setupUi()
{
    // --- Create Labels (Member variables declared in offeritemwidget.h) ---
    discountLabel = new QLabel(this);
    titleLabel = new QLabel(this);
    dateLabel = new QLabel(this);
    storeLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    claimCountLabel = new QLabel(this); // <<< Create the new label instance
    offerToggleButton = new QCheckBox(this);

    // --- Styling & Object Names ---
    // (Assuming most styling comes from QSS or .ui properties now)
    discountLabel->setObjectName("discountLabel");
    discountLabel->setAlignment(Qt::AlignCenter);
    discountLabel->setFixedSize(QSize(45, 45));
    // Set style via QSS

    titleLabel->setObjectName("titleLabel");
    // Set style via QSS

    dateLabel->setObjectName("dateLabel");
    // Set style via QSS

    storeLabel->setObjectName("storeLabel");
    // Set style via QSS

    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    // Set style via QSS (background color set in populateData)

    // <<< ADDED: Set up the new claim count label >>>
    claimCountLabel->setObjectName("claimCountLabel");
    claimCountLabel->setStyleSheet("font-size: 10px; color: #555; background-color: transparent;"); // Basic style

    offerToggleButton->setObjectName("offerToggleButton");
    offerToggleButton->setCursor(Qt::PointingHandCursor);
    // Set style via QSS
    offerToggleButton->setDisabled(true);
    offerToggleButton->setToolTip("Activation manuelle non supportée");


    // --- Layouts ---
    QHBoxLayout *titleStatusLayout = new QHBoxLayout();
    titleStatusLayout->setContentsMargins(0,0,0,0); titleStatusLayout->setSpacing(6);
    titleStatusLayout->addWidget(titleLabel);
    titleStatusLayout->addStretch();
    titleStatusLayout->addWidget(statusLabel);

    QHBoxLayout *dateStoreLayout = new QHBoxLayout();
    dateStoreLayout->setContentsMargins(0,0,0,0); dateStoreLayout->setSpacing(10);
    dateStoreLayout->addWidget(dateLabel);
    dateStoreLayout->addWidget(storeLabel);
    dateStoreLayout->addWidget(claimCountLabel); // <<< ADDED: Add new label to layout
    dateStoreLayout->addStretch();

    QVBoxLayout *detailsLayout = new QVBoxLayout();
    detailsLayout->setContentsMargins(0,0,0,0); detailsLayout->setSpacing(4);
    detailsLayout->addLayout(titleStatusLayout);
    detailsLayout->addLayout(dateStoreLayout);

    QHBoxLayout *mainItemLayout = new QHBoxLayout(this);
    mainItemLayout->setContentsMargins(10, 10, 10, 10); mainItemLayout->setSpacing(15);
    mainItemLayout->addWidget(discountLabel);
    mainItemLayout->addLayout(detailsLayout, 1);
    mainItemLayout->addWidget(offerToggleButton);

    this->setLayout(mainItemLayout);
    // Set base style via QSS
}

// populateData: Sets the text/state on the widgets based on the OfferData struct
void OfferItemWidget::populateData(const OfferData &data)
{
    // --- Text Population ---

    // Display Discount or Price
    QString discountText;
    if (data.percentageReduction > 0) {
        discountText = QString::number(data.percentageReduction, 'f', 0) + "%";
    } else if (data.prixSpecial > 0) {
        discountText = QString::number(data.prixSpecial, 'f', 2); // Consider currency
    } else {
        discountText = "-";
    }
    if(discountLabel) discountLabel->setText(discountText);

    if(titleLabel) titleLabel->setText(data.description);

    // Display Store ID - Still needs improvement to show name
    if(storeLabel) storeLabel->setText(QString("Magasin ID: %1").arg(data.magasinId));

    // Use the helper function to get the current status
    QString currentStatus = data.getCurrentStatus();
    if(statusLabel) statusLabel->setText(currentStatus);

    // Format and set the date range string
    if (data.startDateTime.isValid() && data.endDateTime.isValid()) {
        QString formattedRange = data.startDateTime.toString("dd/MM/yy hh:mm") + " - " + data.endDateTime.toString("dd/MM/yy hh:mm");
        if(dateLabel) dateLabel->setText(formattedRange);
    } else {
        if(dateLabel) dateLabel->setText("Dates invalides");
    }

    // <<< ADDED: Format and set claim count text >>>
    if(claimCountLabel) {
        QString claimText;
        if (data.quantiteLimitee > 0) {
            // Show count / limit
            claimText = QString("Réclamée: %1 / %2").arg(data.claimCount).arg(data.quantiteLimitee);
        } else {
            // Show only count if limit is 0 or less (unlimited)
            claimText = QString("Réclamée: %1").arg(data.claimCount);
        }
        claimCountLabel->setText(claimText);
        claimCountLabel->setToolTip(QString("Nombre de réclamations / Quantité limitée"));
    }
    // <<< END ADDED >>>

    // --- Set Toggle Button State ---
    if(offerToggleButton) offerToggleButton->setChecked(false); // Keep disabled and unchecked

    // --- Adjust Status Label Style based on calculated status ---
    if(statusLabel) {
        QString statusStyleSheet =
            "QLabel#statusLabel {"
            "   font-size: 10px;"
            "   font-weight: bold;"
            "   padding: 3px 8px;"
            "   border-radius: 8px;"
            "   color: white; ";

        if (currentStatus.compare("En cours", Qt::CaseInsensitive) == 0) {
            statusStyleSheet += "background-color: #5cb85c;"; // Green
        } else if (currentStatus.compare("Terminee", Qt::CaseInsensitive) == 0) {
            statusStyleSheet += "background-color: #f0ad4e;"; // Orange/Yellow
        } else if (currentStatus.compare("Validee", Qt::CaseInsensitive) == 0){
            statusStyleSheet += "background-color: #2980B9;"; // Blue
        } else { // Includes "Invalide" or other states
            statusStyleSheet += "background-color: #777777;"; // Default grey
        }
        statusStyleSheet += "}";
        statusLabel->setStyleSheet(statusStyleSheet);
    }

    // --- Set Tooltips ---
    if(discountLabel) discountLabel->setToolTip(data.description);
    if(statusLabel) statusLabel->setToolTip("Statut: " + currentStatus);
    // offerToggleButton tooltip is set in setupUi
}


// Implementation of the internal slot
void OfferItemWidget::onToggleClicked(bool checked)
{
    qDebug() << "Internal slot: Toggle clicked for offer ID:" << m_offerId << "New state:" << checked;
    // Emit the signal with the correct ID type (int)
    // Note: The receiving slot in OffresFlashScreen might still be commented out
    emit offerToggled(m_offerId, checked);
}
