#include "filterairfieldsmodel.h"
#include "listitemdelegate.h"
#include <QDebug>

bool FilterAirfieldsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return (sourceModel()->data(index, Qt::DisplayRole).toString().contains(filterRegExp())
            && (!m_onlySelected || sourceModel()->data(index, Qt::CheckStateRole) == Qt::Checked));
}

void FilterAirfieldsModel::showOnlySelected(const bool onlySelected)
{
    m_onlySelected = onlySelected;
    invalidate();
}
