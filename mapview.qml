import QtQuick 2.14
import QtQuick.Window 2.14
import QtLocation 5.14
import QtPositioning 5.14
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.14

Item {
    id: root
    visible: true
    width: 640
    height: 480
    signal checked(bool f, string codeAirway, string codePoint);
    property var airways: []

    TabBar {
        id: toolBar
        width: parent.width
        TabButton {
            text: qsTr("ESRI")
        }
        TabButton {
            text: qsTr("OSM")
        }
        TabButton {
            text: qsTr("Mapbox")
        }
    }

    StackLayout {
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        currentIndex: toolBar.currentIndex

        Item {
            id: esriTab

            Map {
                id: mapEsriView
                anchors.fill: parent
                plugin: Plugin {
                    preferred: ["esri"]
//                    required: Plugin.AnyMappingFeatures | Plugin.AnyGeocodingFeatures
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: root.selectTurningPointRoute(Qt.point(mouse.x,mouse.y), mapEsriView)
                }
            }
        }
        Item {
            id: osmTab

            Map {
                id: mapOsmView
                anchors.fill: parent
                plugin: Plugin {
                    preferred: ["osm"]
//                    required: Plugin.AnyMappingFeatures | Plugin.AnyGeocodingFeatures
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: root.selectTurningPointRoute(Qt.point(mouse.x,mouse.y), mapOsmView)
                }
            }
        }
        Item {
            id: mapboxTab

            Map {
                id: mapMapboxView
                anchors.fill: parent
                plugin: Plugin {
                    name: "mapbox"
                    PluginParameter {
                        name: "mapbox.access_token"
                        value: "pk.eyJ1IjoibWF4aW1raCIsImEiOiJjazMzaTNoaTIwc2N6M25tajg4ZGhtbXdiIn0.KZ6632nxyVFDhN2i8QYVkw"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: root.selectTurningPointRoute(Qt.point(mouse.x,mouse.y), mapMapboxView)
                }
            }
        }
    }

//    Component {
//        id: mapCircleComponent
//        MapCircle {
//            objectName: "circle"
//            border.width: 1
//            border.color: 'blue'
//        }
//    }

    function selectTurningPointRoute(position, mapParent) {
        var coordinate = mapParent.toCoordinate(position)
        var numPoints = mapParent.mapItems.length;

        for (var i = 0; i < numPoints; i++) {
            if (mapParent.mapItems[i].objectName === "turningPointRoute") {
                var coordinatePoint = mapParent.mapItems[i].center;
                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinatePoint.latitude) / 2)), 2) +
                                                       Math.cos(degreesToRadians(coordinatePoint.latitude)) *
                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
                                                                                                   coordinatePoint.longitude) / 2)), 2)));
                if (d <= 0.15) {
                    mapParent.mapItems[i].select();
                    root.checked(mapParent.mapItems[i].selected, mapParent.mapItems[i].codeAirway, mapParent.mapItems[i].codePoint);
                    break;
                }
            }
        }
    }

    function degreesToRadians(degrees) {
        return (degrees * Math.PI)/180;
    }

    function clearMap() {
        mapOsmView.clearMapItems();
        mapEsriView.clearMapItems();
        mapMapboxView.clearMapItems();
    }

    function setCenter(lat, lon) {
        mapOsmView.center = QtPositioning.coordinate(lat, lon);
        mapEsriView.center = QtPositioning.coordinate(lat, lon);
        mapMapboxView.center = QtPositioning.coordinate(lat, lon);
    }

    function getCenterOfPolygon(path)
    {
        var numPoints = path.length;
        var centerX = 0;
        var centerY = 0;
        var a = 0;

        for (var i = 0; i < numPoints - 1; i++) {
            var point1 = QtPositioning.coordToMercator(QtPositioning.coordinate(path[i].x, path[i].y));
            var point2 = QtPositioning.coordToMercator(QtPositioning.coordinate(path[i + 1].x, path[i + 1].y));

            centerX += (point1.x + point2.x) * (point1.x * point2.y - point2.x * point1.y)
            centerY += (point1.y + point2.y) * (point1.x * point2.y - point2.x * point1.y)
            a += (point1.x * point2.y - point2.x * point1.y)
        }
        centerX = centerX * (1 / (6 * (a * 0.5)))
        centerY = centerY * (1 / (6 * (a * 0.5)))

        return QtPositioning.mercatorToCoord(Qt.point(centerX, centerY));
    }

    function getCenterOfSection(path)
    {
        var centerX = 0;
        var centerY = 0;
        var point1 = QtPositioning.coordToMercator(path[0]);
        var point2 = QtPositioning.coordToMercator(path[1]);

        centerX = (point1.x + point2.x) / 2;
        centerY = (point1.y + point2.y) / 2;
        return QtPositioning.mercatorToCoord(Qt.point(centerX, centerY));
    }

    function createPolyline(points, codeAirway, mapParent) {
        var numPoints = points.length;
        var pointsSection = [];
        var polyline = null;

        for (var i = 0; i < numPoints;) {
            if (pointsSection.length === 0)
                polyline = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 2; line.color: "#000"; }', mapParent)

            // skip point and section too
            if (points[i].x === 0 || points[i].y ===0) {
                pointsSection.shift();
                i = i + 2;
                continue;
            }

            var coordinate = QtPositioning.coordinate(points[i].x, points[i].y)
            var codePoint = points[i + 1];
            polyline.addCoordinate(coordinate);
            createPoint(coordinate, codeAirway, codePoint, mapParent);
            createLabel(coordinate, codePoint, mapParent);

            pointsSection.push(coordinate);

            if (pointsSection.length === 2) {
                createNameAirway(pointsSection, codeAirway, mapParent);
                pointsSection.shift();
                mapParent.addMapItem(polyline);
            }
            i = i + 2;
        }
    }

    function createPoint(coordinate, codeAirway, codePoint, mapParent) {
        var component = Qt.createComponent("qrc:/qml/TurningPointRoute.qml");

        if (component.status === Component.Ready) {
            var point = component.createObject(null);
            point.center = coordinate;
            point.codeAirway = codeAirway;
            point.codePoint = codePoint;
            mapParent.addMapItem(point);
        }
    }

    function createLabel(coordinate, codePoint, mapParent) {
        var component = Qt.createComponent("qrc:/qml/label.qml");

        if (component.status === Component.Ready) {
            var label = component.createObject(parent);
            label.coordinate = coordinate;
            label.name = codePoint;
            mapParent.addMapItem(label);
        }
    }

    function createNameAirway(sectionPoints, nameAirway, mapParent) {
        var component = Qt.createComponent("qrc:/qml/label.qml");

        if (component.status === Component.Ready) {
            var nameAirwayLabel = component.createObject(parent);
            nameAirwayLabel.coordinate = getCenterOfSection(sectionPoints);
            nameAirwayLabel.name = nameAirway;
            mapParent.addMapItem(nameAirwayLabel);
        }
    }

    function drawAirway(points, nameAirway) {
        root.airways.push(points);
        createPolyline(points, nameAirway, mapOsmView);
        createPolyline(points, nameAirway, mapEsriView);
        createPolyline(points, nameAirway, mapMapboxView);

//        var coordinate = getCenterOfPolygon(path);
//        setLabelOfZone(coordinate, nameZone, codeIcao, nameSector, call, func, freq);

//        polyline = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 3; line.color: "#FF4040"; }', mapEsriView)
////        var zoneBorder = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 1; line.color: "#42E712"; }', mapEsriView)
//        numPoints = path.length;
//        for (i = 0; i < numPoints; i++) {
//            polyline.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
////            zoneBorder.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
//        }
//        mapEsriView.addMapItem(polyline)
////        mapEsriView.addMapItem(zoneBorder)

    }

//    function setLabelOfZone(coordinate, nameZone, codeIcao, nameSector, call, func, freq) {
//        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapOsmView);
//        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapEsriView);
//        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapMapboxView);
//    }

//    function drawRadius(radius) {
//        var circle = mapCircleComponent.createObject(mapOsmView, {"center" : mapOsmView.center, "radius": radius * 1000});
//        mapOsmView.addMapItem(circle);
//        circle = mapCircleComponent.createObject(mapEsriView, {"center" : mapEsriView.center, "radius": radius * 1000});
//        mapEsriView.addMapItem(circle);
//        circle = mapCircleComponent.createObject(mapMapboxView, {"center" : mapMapboxView.center, "radius": radius * 1000});
//        mapMapboxView.addMapItem(circle);
//    }
}
