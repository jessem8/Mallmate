#include "offeritemdelegate.h"
// #include "offerdata.h" // Not strictly needed here if status is calculated elsewhere

#include <QApplication>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QFontMetrics>
#include <QDateTime>
#include <QDebug>
#include <QStyle>
#include <QIcon> // Include QIcon if you plan to draw icons later
const QString aiSuggestionPrefix = QStringLiteral("Suggestion IA:");
// Constructor
OfferItemDelegate::OfferItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    qDebug() << "OfferItemDelegate instance created.";
}

// Custom painting logic - Explicit default backgrounds
void OfferItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // --- Get Data ---
    QString description = index.data(Qt::DisplayRole).toString();
    QString discountInfo = index.data(Qt::UserRole + 1).toString();
    QDateTime startDate = index.data(Qt::UserRole + 2).toDateTime();
    QDateTime endDate = index.data(Qt::UserRole + 3).toDateTime();
    QString status = index.data(Qt::UserRole + 4).toString();
    QString storeName = index.data(Qt::UserRole + 5).toString();

    // --- Check if it's an AI suggestion ---
    bool isAiSuggestion = description.startsWith(aiSuggestionPrefix);
    QString displayDescription = description; // Use full description by default
    // If you want to HIDE the prefix in the display uncomment the next line:
    // if (isAiSuggestion) displayDescription = description.mid(aiSuggestionPrefix.length()).trimmed();

    // --- Determine Background and Text Colors ---
    QColor backgroundColor;
    QColor textColor;
    QColor statusTextColor;

    bool isSelected = option.state & QStyle::State_Selected;
    bool isHovered = option.state & QStyle::State_MouseOver;

    // --- STEP 1: Determine base colors based on selection ---
    if (isSelected) {
        backgroundColor = option.palette.highlight().color();
        textColor = option.palette.highlightedText().color();
        statusTextColor = textColor;
    } else {
        // --- STEP 2: Set EXPLICIT default/alternating background ---
        backgroundColor = (index.row() % 2 == 0) ? QColor(Qt::white) : QColor("#f9f9f9"); // White and very light grey
        textColor = option.palette.text().color(); // Standard text color
        statusTextColor = textColor; // Default status text color

        // --- STEP 3: Override colors based on STATUS ---
        if (status.compare(QString::fromUtf8("En Attente"), Qt::CaseInsensitive) == 0) {
            // --- Different background for AI suggestions that are Pending ---
            if (isAiSuggestion) {
                backgroundColor = QColor(230, 245, 255); // Very Light Blue background for AI suggestions
                statusTextColor = QColor(0, 100, 180);   // Darker blue text
            } else {
                backgroundColor = QColor(255, 253, 208); // Light Yellow background for normal Pending
                statusTextColor = QColor(204, 130, 0);   // Darker orange/brown text
            }
        } else if (status.compare(QString::fromUtf8("Rejetée"), Qt::CaseInsensitive) == 0) {
            backgroundColor = QColor("#FFEBEE"); // Keep light grey/pink for rejected
            statusTextColor = QColor(196, 43, 55);   // Darker red text
        } else {
            // Determine status text color for other statuses (background remains default)
            if (status.compare("En cours", Qt::CaseInsensitive) == 0) {
                statusTextColor = QColor(0, 100, 0); // Dark Green
            } else if (status.compare("Terminee", Qt::CaseInsensitive) == 0) {
                statusTextColor = QColor(105, 105, 105); // Dim Gray
            } else if (status.compare("Validee", Qt::CaseInsensitive) == 0) {
                statusTextColor = QColor("#2980B9"); // Blue text
            } else if (status.compare("Planifiée", Qt::CaseInsensitive) == 0 || status.startsWith("Planifiée")) { // Handle variants
                statusTextColor = QColor("#8E44AD"); // Purple-ish text for scheduled
            } else if (status.compare("Invalide", Qt::CaseInsensitive) == 0) { // Handle invalid state
                statusTextColor = QColor(196, 43, 55); // Use red for invalid
            }
            // else statusTextColor remains default textColor
        }

        // --- STEP 4: Apply hover effect (optional) ---
        if (isHovered && !isSelected) { // Don't apply hover if selected
            // Slightly darken the determined background color on hover
            backgroundColor = backgroundColor.darker(105); // Adjust factor as needed
        }
    }

    // --- Draw Background ---
    painter->fillRect(option.rect, backgroundColor);

    // --- Define Rects and Fonts ---
    QRect contentRect = option.rect.adjusted(padding, padding, -padding, -padding);
    int currentY = contentRect.top();
    int availableWidth = contentRect.width();

    QFont defaultFont = painter->font();
    QFont boldFont = defaultFont;
    boldFont.setBold(true);
    QFont smallFont = defaultFont;
    smallFont.setPointSize(qMax(6, defaultFont.pointSize() - 1));
    QFont italicSmallFont = smallFont; // Font for AI indicator
    italicSmallFont.setItalic(true);

    QFontMetrics fmBold(boldFont);
    QFontMetrics fmDefault(defaultFont);
    QFontMetrics fmSmall(smallFont);
    QFontMetrics fmItalicSmall(italicSmallFont);

    int smallLineHeight = fmSmall.height();
    int defaultLineHeight = fmDefault.height();
    int statusTextWidth = fmSmall.horizontalAdvance(status);
    int aiIndicatorWidth = 0;
    QString aiIndicatorText = "[IA]";

    // Calculate width for AI indicator if needed
    if (isAiSuggestion) {
        aiIndicatorWidth = fmItalicSmall.horizontalAdvance(aiIndicatorText) + spacing; // Add spacing
    }

    // Adjust main content width to account for status AND AI indicator
    int mainContentWidth = availableWidth - statusTextWidth - aiIndicatorWidth - spacing * 2; // Subtract both widths
    if (mainContentWidth < 50) mainContentWidth = availableWidth / 2; // Basic sanity check

    // --- Draw Main Content (Left Aligned) ---
    painter->setPen(textColor); // Set the correct text color

    // 1. Dates (Small Font, Top Left)
    painter->setFont(smallFont);
    QString dateString = QString("Du %1 au %2")
                             .arg(startDate.isValid() ? startDate.toString("dd/MM/yy hh:mm") : "N/A") // Handle invalid dates
                             .arg(endDate.isValid() ? endDate.toString("dd/MM/yy hh:mm") : "N/A");
    QRect dateRect(contentRect.left(), currentY, mainContentWidth, smallLineHeight);
    painter->drawText(dateRect, Qt::AlignLeft | Qt::AlignVCenter, dateString);

    // 2. Status Text & AI Indicator (Define Rects for Right Alignment)
    QRect statusDrawRect(contentRect.right() - statusTextWidth, contentRect.top(), statusTextWidth, smallLineHeight);
    QRect aiIndicatorDrawRect;
    if (isAiSuggestion) {
        aiIndicatorDrawRect = QRect(statusDrawRect.left() - aiIndicatorWidth, contentRect.top(), aiIndicatorWidth - spacing, smallLineHeight);
    }
    // Increment Y position AFTER drawing the top line elements
    currentY += smallLineHeight + spacing;

    // 3. Description (Bold Font, Potentially Wrapped)
    painter->setFont(boldFont);
    painter->setPen(textColor);
    // Use displayDescription which might have prefix removed (or not)
    QRect descBoundingRect = fmBold.boundingRect(contentRect.left(), currentY, mainContentWidth, 0, Qt::TextWordWrap, displayDescription);
    painter->drawText(descBoundingRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, displayDescription);
    currentY += descBoundingRect.height() + spacing;

    // 4. Store Name (Default Font)
    painter->setFont(defaultFont);
    painter->setPen(textColor);
    QRect storeRect(contentRect.left(), currentY, mainContentWidth, defaultLineHeight);
    painter->drawText(storeRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight, storeName);
    currentY += storeRect.height() + spacing / 2;

    // 5. Discount/Price (Default Font, Bottom Left)
    QRect discountRect(contentRect.left(), currentY, mainContentWidth, defaultLineHeight);
    painter->drawText(discountRect, Qt::AlignLeft | Qt::AlignVCenter, discountInfo);

    // --- Draw Status (Right Aligned - Actual Drawing) ---
    painter->setFont(smallFont);
    painter->setPen(statusTextColor); // Use the specific status color
    painter->drawText(statusDrawRect, Qt::AlignRight | Qt::AlignVCenter, status);

    // --- Draw AI Indicator (if applicable) ---
    if (isAiSuggestion) {
        painter->setFont(italicSmallFont);
        painter->setPen(statusTextColor); // Use same color as status, or a different one e.g., QColor("#555")
        painter->drawText(aiIndicatorDrawRect, Qt::AlignRight | Qt::AlignVCenter, aiIndicatorText);
    }

    painter->restore(); // Restore painter state
}


// Calculate the preferred size for the item
QSize OfferItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QSize();
    }

    // --- Get Data needed for size calculation ---
    QString description = index.data(Qt::DisplayRole).toString();

    // --- Calculate Heights based on Fonts ---
    QFont defaultFont = QApplication::font();
    QFont boldFont = defaultFont;
    boldFont.setBold(true);
    QFont smallFont = defaultFont;
    smallFont.setPointSize(qMax(6, defaultFont.pointSize() - 1));

    QFontMetrics fmBold(boldFont);
    QFontMetrics fmDefault(defaultFont);
    QFontMetrics fmSmall(smallFont);

    int smallLineHeight = fmSmall.height();
    int defaultLineHeight = fmDefault.height();

    // Estimate width available for text (subtract padding)
    int availableWidth = option.rect.width() - 2 * padding;
    if (availableWidth <= 0) availableWidth = 100; // Prevent negative width

    // Calculate height needed for potentially wrapped description
    QRect descBoundingRect = fmBold.boundingRect(0, 0, availableWidth, 0, Qt::TextWordWrap, description);
    int descriptionHeight = descBoundingRect.height();

    // Calculate total estimated height based on drawing order and spacing:
    // Padding + Date + Spacing + Status + Spacing + Description + Spacing + Store + Spacing + Discount + Padding
    int height = padding +          // Top padding
                 smallLineHeight +  // Date line
                 spacing / 2 +      // Space
                 smallLineHeight +  // Status line
                 spacing +          // Space
                 descriptionHeight +// Description (wrapped)
                 spacing +          // Space
                 defaultLineHeight +// Store Name line
                 spacing / 2 +      // Space
                 defaultLineHeight +// Discount/Price line
                 padding;           // Bottom padding

    // Ensure a minimum reasonable height
    height = qMax(height, 85); // Adjust minimum height as needed

    // Return the calculated size hint
    return QSize(option.rect.width(), height);
}
