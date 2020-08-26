#ifndef CHECKBOXITEMDELEGATE_H
#define CHECKBOXITEMDELEGATE_H

#include <QItemDelegate>

class CheckboxItemDelegate : public QItemDelegate
{
        Q_OBJECT
    public:
        explicit CheckboxItemDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
        QRect checkRect(const QStyleOptionViewItem& option, const QRect& bounding) const;
};

#endif // CHECKBOXITEMDELEGATE_H
