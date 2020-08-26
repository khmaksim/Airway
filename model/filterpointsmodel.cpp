#include "filterpointsmodel.h"
#include "listitemdelegate.h"
#include <QDebug>
#include <QtMath>
#include <QString>
#include "helper.h"

FilterPointsModel::FilterPointsModel(QObject *parent) : QSortFilterProxyModel(parent),
    codeAirway(QString())
{
}

FilterPointsModel::~FilterPointsModel()
{
}

void FilterPointsModel::setFilterProperty(QString filterName, QVariant value)
{
    if (filterName == "codeAirway")
        codeAirway = value.toString();

    invalidate();
}

bool FilterPointsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    return (sourceModel()->index(sourceRow, 0).data(Qt::UserRole).toString() == codeAirway);
}

//bool SortSearchFilterObstracleModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
//{
//    bool result = false;

//    for (int col = 1; col < sourceModel()->columnCount(); col++) {
//        QModelIndex index = sourceModel()->index(sourceRow, col, sourceParent);
//        result |= sourceModel()->data(index).toString().contains(filterRegExp());
//    }

//    if (fromHeight > 0) {
//        result &= (fromHeight <= sourceModel()->data(sourceModel()->index(sourceRow, 12, sourceParent)).toInt());
//    }

//    if (toHeight > 0) {
//        result &= (toHeight >= sourceModel()->data(sourceModel()->index(sourceRow, 12, sourceParent)).toInt());
//    }

//    if (!tags.isEmpty()) {
//        result &= sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent), Qt::UserRole + 1)
//                  .toString().contains(QRegExp(tags.join("|")));
//    }

//    if (!types.isEmpty())
//        result &= sourceModel()->data(sourceModel()->index(sourceRow, 2, sourceParent)).toString().contains(QRegExp(types.join("|")));

//    if (!areas.isEmpty())
//        result &= sourceModel()->data(sourceModel()->index(sourceRow, 1, sourceParent)).toString().contains(QRegExp(QString("^\\w{4}[%1]{1}.").arg(areas.join(""))));

//    if (markingDay)
//        result &= sourceModel()->data(sourceModel()->index(sourceRow, 17, sourceParent))
//                  .toString().contains(QRegExp("1"));

//    if (nightMarking)
//        result &= sourceModel()->data(sourceModel()->index(sourceRow, 20, sourceParent))
//                  .toString().contains(QRegExp("1"));

//    if (lat > 0 && lon > 0 && radius > 0) {
//        double latObstracle = 0;
//        double lonObstracle = 0;
//        if (sourceModel()->data(sourceModel()->index(sourceRow, 6, sourceParent)).isValid() &&
//                sourceModel()->data(sourceModel()->index(sourceRow, 7, sourceParent)).isValid()) {
//            latObstracle = Helper::convertCoordinateInDec(sourceModel()->data(sourceModel()->index(sourceRow, 6, sourceParent)).toString());
//            lonObstracle = Helper::convertCoordinateInDec(sourceModel()->data(sourceModel()->index(sourceRow, 7, sourceParent)).toString());
//        }
//        else {
//            latObstracle = Helper::convertCoordinateInDec(sourceModel()->data(sourceModel()->index(sourceRow, 8, sourceParent)).toString());
//            lonObstracle = Helper::convertCoordinateInDec(sourceModel()->data(sourceModel()->index(sourceRow, 9, sourceParent)).toString());
//        }

//        if (latObstracle > 0 && lonObstracle > 0) {
//            // 6371 - radius Earth
//            double d = 6371 * 2 * qAsin(qSqrt(qPow(qSin(qDegreesToRadians((latObstracle - lat) / 2)), 2) +
//                                                 qCos(qDegreesToRadians(lat)) *
//                                                 qCos(qDegreesToRadians(latObstracle)) *
//                                                 qPow(qSin(qDegreesToRadians(qAbs(lonObstracle - lon) / 2)), 2)));
//            if (d <= radius)
//                result &= true;
//            else
//                result &= false;
//        }
//    }
//    return result;
//}

//bool SortSearchFilterObstracleModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
//{
//    QVariant leftData = sourceModel()->data(source_left);
//    QVariant rightData = sourceModel()->data(source_right);

//    if (source_left.column() == 0 && source_right.column() == 0) {
//        leftData = sourceModel()->data(source_left, Qt::CheckStateRole);
//        rightData = sourceModel()->data(source_right, Qt::CheckStateRole);
//        return leftData.toBool() > rightData.toBool();
//    }
////    if (leftData.type() == QVariant::Int) {
//    QString leftStr = leftData.toString().replace(",", ".");
//    QString rightStr = rightData.toString().replace(",", ".");;
//    if (leftStr.contains(QRegExp("^\\d+\\.?\\d*$")) && rightStr.contains(QRegExp("^\\d+\\.?\\d*$"))) {
//        return leftStr.toFloat() < rightStr.toFloat();
//    } /*else {
//        static const QRegularExpression emailPattern("[\\w\\.]*@[\\w\\.]*");

//        QString leftString = leftData.toString();
//        if (left.column() == 1) {
//            const QRegularExpressionMatch match = emailPattern.match(leftString);
//            if (match.hasMatch())
//                leftString = match.captured(0);
//        }
//        QString rightString = rightData.toString();
//        if (right.column() == 1) {
//            const QRegularExpressionMatch match = emailPattern.match(rightString);
//            if (match.hasMatch())
//                rightString = match.captured(0);
//        }

//        return QString::localeAwareCompare(leftString, rightString) < 0;
//    }*/
//    return QSortFilterProxyModel::lessThan(source_left, source_right);
//}

//void SortSearchFilterObstracleModel::setFilterProperty(QString objectName, QVariant value)
//{
//    if (objectName.contains("markingDayCheckBox", Qt::CaseInsensitive))
//        markingDay = value.toBool();
//    else if (objectName.contains("nightMarkingCheckBox", Qt::CaseInsensitive))
//        nightMarking = value.toBool();
//    else if (objectName.contains("tag", Qt::CaseInsensitive))
//        tags = value.toStringList();
//    else if (objectName.contains("types", Qt::CaseInsensitive))
//        types = value.toStringList();
//    else if (objectName.contains("areas", Qt::CaseInsensitive))
//        areas = value.toStringList();
//    else {
//        fromHeight = value.toList().at(0).toInt();
//        toHeight = value.toList().at(1).toInt();
//    }

//    invalidateFilter();
//}

//void SortSearchFilterObstracleModel::setFilterRadius(QString lat, QString lon, int radius)
//{
//    this->lat = Helper::convertCoordinateInDec(lat);
//    this->lon = Helper::convertCoordinateInDec(lon);
//    this->radius = radius;

//    invalidateFilter();
//}
