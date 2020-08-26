#include "searchmodel.h"
#include "listitemdelegate.h"

SearchModel::SearchModel(QObject *parent)
{
    Q_UNUSED(parent)
}

SearchModel::~SearchModel()
{

}

bool SearchModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
  {
      QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

      return (sourceModel()->data(index, ListItemDelegate::CodeICAORole).toString().contains(filterRegExp())
              || sourceModel()->data(index, ListItemDelegate::NameAirfieldRole).toString().contains(filterRegExp()));
  }
