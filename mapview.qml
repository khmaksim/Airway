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
    signal checked(bool f, string id);

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

//                    onDoubleClicked: {
//                        var coordinate = mapEsriView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapEsriView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapEsriView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapEsriView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapEsriView.mapItems[i].selected = !mapEsriView.mapItems[i].selected;
//                                    root.checked(mapEsriView.mapItems[i].selected, mapEsriView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
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

//                    onDoubleClicked: {
//                        var coordinate = mapOsmView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapOsmView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapOsmView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapOsmView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapOsmView.mapItems[i].selected = !mapOsmView.mapItems[i].selected;
//                                    root.checked(mapOsmView.mapItems[i].selected, mapOsmView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
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

//                MouseArea {
//                    anchors.fill: parent

//                    onDoubleClicked: {
//                        var coordinate = mapMapboxView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapMapboxView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapMapboxView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapMapboxView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapMapboxView.mapItems[i].selected = !mapMapboxView.mapItems[i].selected;
//                                    root.checked(mapMapboxView.mapItems[i].selected, mapMapboxView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                }
            }
        }
    }

    Component {
        id: mapCircleComponent
        MapCircle {
            objectName: "circle"
            border.width: 1
            border.color: 'blue'
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

    function createPolyline(points, nameAirway, mapParent) {
        var polyline = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 2; line.color: "#000"; }', mapParent)
        var numPoints = points.length;
        var pointsSection = [];

        for (var i = 0; i < numPoints;) {
            var coordinate = QtPositioning.coordinate(points[i].x, points[i].y)
            polyline.addCoordinate(coordinate);
            createPoint(coordinate, mapParent);
            createLabel(coordinate, points[i + 1], mapParent);

            pointsSection.push(coordinate);

            if (pointsSection.length === 2) {
                createNameAirway(pointsSection, nameAirway, mapParent);
                pointsSection.shift();
            }
            i = i + 2;
        }

        mapParent.addMapItem(polyline)
    }

    function createPoint(coordinate, mapParent) {
        var point = Qt.createQmlObject('import QtLocation 5.14; MapCircle { radius: 1500; color: "#fff"; }', mapParent)
        point.center = coordinate;
        mapParent.addMapItem(point)
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
