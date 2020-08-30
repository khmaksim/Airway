#include "pointsmodel.h"
#include <QDebug>

PointsModel::PointsModel(QObject *parent) : QAbstractTableModel(parent),
    m_checkedRows(QList<bool>())
{
    m_headers << tr("*")
              << tr("Code PPM")
              << tr("Name PPM")
              << tr("State")
              << tr("Latitude")
              << tr("Longitude");
}

PointsModel::~PointsModel()
{

}

int PointsModel::rowCount(const QModelIndex &parent) const
{
    return m_points.size();
}

int PointsModel::columnCount(const QModelIndex &parent) const
{
    return m_headers.size();
}

QVariant PointsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (index.column() == 0) {
        if (role == Qt::CheckStateRole)
            return (m_checkedRows.size() > 0 && m_checkedRows.at(row)) ? Qt::Checked : Qt::Unchecked;
        else if (role == Qt::UserRole)
            return m_points.at(row).toList().at(col);
    }
    if (role == Qt::DisplayRole)
        return m_points.at(row).toList().at(col);

    return QVariant();
}

QVariant PointsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section > m_headers.size() || orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    return m_headers.at(section);
}

Qt::ItemFlags PointsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PointsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
        m_checkedRows[index.row()] = value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

//void PointsModel::fetchMore(const QModelIndex &parent)
//{
//    int remainder = m_points.size() - m_pointCount;
//    int rowsToFetch = qMin(100, remainder);

//    if (rowsToFetch <= 0)
//        return;

//    beginInsertRows(QModelIndex(), m_pointCount, m_pointCount + rowsToFetch - 1);

//    m_pointCount += rowsToFetch;

//    endInsertRows();
//}

//bool PointsModel::canFetchMore(const QModelIndex &parent) const
//{
//    return (m_pointCount < m_points.size());
//}

void PointsModel::setDataModel(const QList<QVariant> &data)
{
    beginResetModel();
    m_points = data;

    for (int i = 0; i < m_points.size(); i++)
        m_checkedRows.append(false);
    endResetModel();
}
