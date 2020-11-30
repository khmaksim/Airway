#include "pointitemdelegate.h"
#include "filterpointsmodel.h"
#include "pointsmodel.h"

PointItemDelegate::PointItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void PointItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto *pointsModel = qobject_cast<const PointsModel*>(index.model());
//    auto *filterModel = qobject_cast<const FilterPointsModel*>(index.model());
//    auto *pointsModel = qobject_cast<PointsModel*>(filterModel->sourceModel());
//    QModelIndex sourceIndex = filterModel->mapToSource(index);
QModelIndex sourceIndex = index;
    QStyleOptionViewItem opt(option);

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

    QStyledItemDelegate::paint(painter, opt, index);
}

QSize PointItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    if(!index.isValid())
        return {};

    return QStyledItemDelegate::sizeHint(option, index);
}
