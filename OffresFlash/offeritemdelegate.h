#ifndef OFFERITEMDELEGATE_H
#define OFFERITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

// Forward declarations
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

/**
 * @brief The OfferItemDelegate class provides custom drawing for OfferData items
 * in views like QListWidget or QTableView.
 */
class OfferItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit OfferItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    // Layout constants
    const int padding = 10;
    const int spacing = 4; // Vertical spacing between lines

};

#endif // OFFERITEMDELEGATE_H
