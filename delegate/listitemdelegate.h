#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class ListItemDelegate : public QStyledItemDelegate
{
        Q_OBJECT

    public:
        enum DataRole { CodeICAORole = Qt::UserRole + 1, NameAirfieldRole, IdRole };

        explicit ListItemDelegate(QStyledItemDelegate *parent = 0);
        ~ListItemDelegate();

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif // LISTITEMDELEGATE_H
