#include "pointitemdelegate.h"
#include <QPainter>
#include <QStylePainter>
#include <QMouseEvent>
#include <QApplication>
#include "filterpointsmodel.h"
#include "pointsmodel.h"

PointItemDelegate::PointItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void PointItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto *filterModel = qobject_cast<const FilterPointsModel*>(index.model());
    auto *pointsModel = qobject_cast<PointsModel*>(filterModel->sourceModel());
    QModelIndex sourceIndex = filterModel->mapToSource(index);

    QStyleOptionViewItem opt(option);
    drawBackground(painter, opt, index);
    drawFocus(painter, opt, opt.rect);

    if (index.row() == index.model()->rowCount() - 1)
        return;
    if (index.column() > 0 && index.column() < 5)
        opt.text = index.data().toString();
    if (index.column() == 5)
        opt.text = pointsModel->getMagneticTrackAngle(sourceIndex).join('/');
    if (index.column() == 6)
        opt.text = QString::number(pointsModel->getDistance(sourceIndex));
    if (index.column() == 7)
        opt.text = pointsModel->getLimit(sourceIndex).join('/');
    if (index.column() == 8)
        opt.text = QString::number(pointsModel->getMinimumAltitude(sourceIndex));
    if (index.column() == 9)
        opt.text = QString::number(pointsModel->getWidth(sourceIndex));
    if (index.column() == 10)
        opt.text = pointsModel->getDirectionTrains(sourceIndex).join('/');

    painter->setPen(opt.state.testFlag(QStyle::State_Selected) ? QColor(Qt::white) : QColor(Qt::black));

    qApp->style()->drawItemText(painter, opt.rect, Qt::AlignCenter, opt.palette, true, opt.text);
}

QSize PointItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    if(!index.isValid())
        return {};

    return QItemDelegate::sizeHint(option, index);
}
