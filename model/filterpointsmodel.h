#ifndef FILTERPOINTSMODEL_H
#define FILTERPOINTSMODEL_H

#include <QSortFilterProxyModel>

class FilterPointsModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit FilterPointsModel(QObject *parent = nullptr);
        ~FilterPointsModel();

        void setFilterProperty(QString, QVariant);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    private:
        QStringList m_codeAirways;
};

#endif // FILTERPOINTSMODEL_H
