#ifndef POINTITEMDELEGATE_H
#define POINTITEMDELEGATE_H

#include <QObject>
#include <QItemDelegate>

class PointItemDelegate : public QItemDelegate
{
        Q_OBJECT

    public:
        explicit PointItemDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif // POINTITEMDELEGATE_H
