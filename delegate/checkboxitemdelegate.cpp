#include <QPainter>
#include <QStylePainter>
#include <QMouseEvent>
#include <QApplication>
#include "checkboxitemdelegate.h"
#include <QDebug>

CheckboxItemDelegate::CheckboxItemDelegate(QObject *parent) : QItemDelegate(parent)
{
}

void CheckboxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    drawBackground(painter, opt, index);
    drawFocus(painter, opt, opt.rect);
    opt.rect = checkRect(option, option.rect);
    opt.state = opt.state & ~QStyle::State_HasFocus;
    opt.state|=(index.data(Qt::CheckStateRole).toBool() ? QStyle::State_On : QStyle::State_Off);
    qApp->style()->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &opt, painter);
}

QRect CheckboxItemDelegate::checkRect(const QStyleOptionViewItem& option, const QRect& bounding) const
{
    QStyleOptionButton opt;
    opt.QStyleOption::operator = (option);
    opt.rect = bounding;
    QRect cr = qApp->style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);
    int deltaX = (bounding.width() - cr.width()) / 2;
    int deltaY = (bounding.height() - cr.height()) / 2;
    return QRect(bounding.left() + deltaX, bounding.top() + deltaY, cr.width(), cr.height());
}

bool CheckboxItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(!event || ! model)
        return false;

    Qt::ItemFlags flags = model->flags(index);
    if (!(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled))
        return false;

    switch(event->type())
    {
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        {
            QRect cr(checkRect(option, option.rect));
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if (me->button() != Qt::LeftButton || !cr.contains(me->pos()))
                return false;

            // eat the double click events inside the check rect
            if(event->type() == QEvent::MouseButtonDblClick)
                return true;
            break;
        }
        case QEvent::KeyPress:
        {
            QKeyEvent* kev = static_cast<QKeyEvent*>(event);
            if(kev->key() != Qt::Key_Space && kev->key() != Qt::Key_Select)
                return false;
            break;
        }
        default: return false;
    }

    int value = (index.data(Qt::CheckStateRole).toBool() ? false : true);
    return model->setData(index, value, Qt::CheckStateRole);
}
