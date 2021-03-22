#include "buttonlistitemdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QLineEdit>

ButtonListItemDelegate::ButtonListItemDelegate(QObject *parent) : QStyledItemDelegate()
{

}

ButtonListItemDelegate::~ButtonListItemDelegate() = default;

//void ButtonListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    Q_UNUSED(index)

//    QStyleOptionButton button;

//    // Set our button to fill the entire cell contents
//    button.rect = QRect(option.rect.width() - option.rect.height(), 0, option.rect.height(), option.rect.height());
//    button.text = QString("...");
//    // Set our button state to enabled
//    button.state |= QStyle::State_Enabled;

//    if (option.state & QStyle::State_Selected) {
//        painter->fillRect(option.rect, option.palette.highlight());
//    }

//    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
//}

QWidget* ButtonListItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    auto *lineEdit = new QLineEdit(parent);
    return lineEdit;
}

void ButtonListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *lineEdit = qobject_cast<QLineEdit*>(editor);
    lineEdit->setText(index.data().toString());
}

void ButtonListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *lineEdit = qobject_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}
