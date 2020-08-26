#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QSortFilterProxyModel>

class SearchModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit SearchModel(QObject *parent = nullptr);
        ~SearchModel();

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // SEARCHMODEL_H
