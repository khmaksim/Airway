#include "filterpointsmodel.h"
#include "listitemdelegate.h"
#include <QDebug>
#include <QtMath>
#include <QString>
#include <QMetaType>
#include "helper.h"

FilterPointsModel::FilterPointsModel(QObject *parent) : QSortFilterProxyModel(parent),
    m_codeAirways(QString())
{
}

FilterPointsModel::~FilterPointsModel()
= default;

void FilterPointsModel::setFilterProperty(const QString& filterName, const QVariant& value)
{
    m_codeAirways.clear();

    if (filterName == "codeAirway") {
        if (value.canConvert<QString>())
            m_codeAirways << value.toString();
        else
            m_codeAirways << value.toStringList();
    }
    invalidate();
}

bool FilterPointsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    return m_codeAirways.contains(sourceModel()->index(sourceRow, filterKeyColumn()).data().toString());
}
