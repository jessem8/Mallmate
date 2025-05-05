#ifndef OFFERITEMWIDGET_H
#define OFFERITEMWIDGET_H

#include <QWidget>
#include <QString>
#include "offerdata.h" // Include your corrected offer data structure definition

// Include necessary Qt classes
#include <QLabel>
#include <QCheckBox>

// Forward declarations for Qt layout classes used only as pointers in .cpp
class QHBoxLayout;
class QVBoxLayout;


class OfferItemWidget : public QWidget
{
    Q_OBJECT // Essential for signals and slots

public:
    // Constructor - takes data for the offer to display
    explicit OfferItemWidget(const OfferData &data, QWidget *parent = nullptr);
    ~OfferItemWidget() override; // Use override keyword for virtual destructor

signals:
    // Signal emitted when the toggle button is clicked
    // Passes the offer's ID (now int) and the new state (true=checked, false=unchecked)
    void offerToggled(int offerId, bool newState); // offerId is now int

private slots:
    // Internal slot to react to the checkbox's toggled signal
    void onToggleClicked(bool checked);

private:
    // UI Elements
    QLabel *discountLabel = nullptr;
    QLabel *titleLabel = nullptr;
    QLabel *dateLabel = nullptr;
    QLabel *storeLabel = nullptr;
    QLabel *statusLabel = nullptr;
    QLabel *claimCountLabel = nullptr; // <<< ADDED: Declaration for claim count label
    QCheckBox *offerToggleButton = nullptr;

    // Data Storage
    int m_offerId = -1; // Member variable to store this offer's ID is now int

    // Helper function to set up the UI layout for this widget
    void setupUi();
    // Helper to set text/state on widgets
    void populateData(const OfferData &data);
};

#endif // OFFERITEMWIDGET_H
