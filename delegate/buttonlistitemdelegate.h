#ifndef BUTTONLISTITEMDELEGATE_H
#define BUTTONLISTITEMDELEGATE_H

#include <QStyledItemDelegate>

class ButtonListItemDelegate : public QStyledItemDelegate
{
    public:
        explicit ButtonListItemDelegate(QObject *parent = nullptr);
        virtual ~ButtonListItemDelegate();

//        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif // BUTTONLISTITEMDELEGATE_H
