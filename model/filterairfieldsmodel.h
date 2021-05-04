#ifndef FILTERAIRFIELDSMODEL_H
#define FILTERAIRFIELDSMODEL_H

#include <QSortFilterProxyModel>

class FilterAirfieldsModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit FilterAirfieldsModel(QObject *parent = nullptr)
            : QSortFilterProxyModel(parent),
              m_onlySelected(false)
        {}
        ~FilterAirfieldsModel() = default;

    public slots:
        void showOnlySelected(const bool onlySelected);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    private:
        bool m_onlySelected;
};

#endif // FILTERAIRFIELDSMODEL_H
