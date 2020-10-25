#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QVector>

//struct ObstraclePoint {
//        enum TypeObstraction { NATURAL, ARTIFICIAL, ARTIFICIAL_MARKING, GROUP, KTA };

//        QString id;
//        double lat;
//        double lon;
//        QString height;
//        TypeObstraction type;
//};

class MapView : public QQuickView
{
        Q_OBJECT

    public:
        explicit MapView();
        ~MapView() {}

        void setRadius(QVariant);
        void clearMap();
        void setCenter(QVariant);
//        void addObstracle(ObstraclePoint);
        void drawAirway(QList<QVariant> &path, const QMap<QString, QString> &args);

    signals:
        void checked(bool, QString, QString);
};

#endif // MAPVIEW_H
