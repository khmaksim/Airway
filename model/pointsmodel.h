#ifndef POINTSMODEL_H
#define POINTSMODEL_H

#include <QAbstractTableModel>
#include <QList>

class PointsModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit PointsModel(QObject *parent = nullptr);
        ~PointsModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
//        void fetchMore(const QModelIndex &parent);
//        bool canFetchMore(const QModelIndex &parent) const;

        void setDataModel(const QList<QVariant> &data);

    private:
        QStringList m_headers;
        QList<QVariant> m_points;
        QList<bool> m_checkedRows;
        int m_pointCount;
};

#endif // POINTSMODEL_H
