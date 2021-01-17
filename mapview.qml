import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

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
                if (d <= 0.80) {
                    mapParent.mapItems[i].select();
                    root.checked(mapParent.mapItems[i].selected, mapParent.mapItems[i].codeAirway, mapParent.mapItems[i].codePoint);
                    break;
                }
            }
        }
    }

    function degreesToRadians(degrees) {
        return degrees * Math.PI / 180;
    }

    function radiansToDegrees(radians) {
        return radians * 180 / Math.PI;
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

    function getAngle(point1, point2)
    {
        var p1Lat = degreesToRadians(point1.latitude);
        var p1Lon = degreesToRadians(point1.longitude);
        var p2Lat = degreesToRadians(point2.latitude);
        var p2Lon = degreesToRadians(point2.longitude);
        var cl1 = Math.cos(p1Lat)
        var cl2 = Math.cos(p2Lat)
        var sl1 = Math.sin(p1Lat)
        var sl2 = Math.sin(p2Lat)
        var delta = p2Lon - p1Lon
        var cdelta = Math.cos(delta)
        var sdelta = Math.sin(delta)
        var x = (cl1 * sl2) - (sl1 * cl2 * cdelta)
        var y = sdelta * cl2
        var z = radiansToDegrees(Math.atan(-y / x))

        if (x < 0)
            z = z + 180.

        var z2 = (z + 180.) % 360. - 180.
        z2 = -1 * degreesToRadians(z2)
        var anglerad2 = z2 - ((2 * Math.PI) * Math.floor((z2 / (2 * Math.PI))))
        var angledeg = radiansToDegrees(anglerad2)
        return angledeg;
    }

    function createPolyline(points, codeAirway, mapParent) {
        var numPoints = points.length;
        var pathPoints = [];
        var codePoints = [];
        var infoPoints = [];
        var polyline = null;
        var pathsDetails = [];

        for (var i = 0; i < numPoints; i++) {
            if (pathPoints.length === 0)
                polyline = Qt.createQmlObject('import QtLocation 5.12; MapPolyline { line.width: 2; line.color: "#000"; }', mapParent)

            var details = {};
            var coordinateXY = points[i]['coordinate'];
            var codePoint = points[i]['code'];
            var infoPoint = points[i]['info'];

            // skip point and section too
            if (!codePoint) {
                pathPoints.shift();
                continue;
            }

            details['codeAirway'] = codeAirway;
            details['distance'] = points[i]['distance'];
            details['magneticTrackAngle'] = points[i]['mpu'];
            pathsDetails.push(details);

            var coordinate = QtPositioning.coordinate(coordinateXY.x, coordinateXY.y)

            codePoints.push(codePoint);
            infoPoints.push(infoPoint);
            polyline.addCoordinate(coordinate);
            createPoint(coordinate, codeAirway, codePoint, mapParent);
            pathPoints.push(coordinate);

            if (pathPoints.length === 2) {
                var magneticTrackAngle = pathsDetails[0]['magneticTrackAngle'];
                var pathDetails = {};
                pathDetails['codeAirway'] = pathsDetails[0]['codeAirway'];
                pathDetails['distance'] = pathsDetails[0]['distance'];

                createMagneticTrackAngle(pathPoints, magneticTrackAngle, mapParent);
                createDetailsPath(pathPoints, pathDetails, mapParent);
                createLabel(pathPoints, codePoints[0], infoPoints[0], mapParent);

                if ((i + 1) >= numPoints) {
                    pathPoints.push(pathPoints[0]);
                    pathPoints.shift();
                    createLabel(pathPoints, codePoints[1], infoPoints[1], mapParent);
                }
                pathsDetails.shift();
                codePoints.shift();
                infoPoints.shift();
                pathPoints.shift();
                mapParent.addMapItem(polyline);
            }
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

    function createLabel(path, codePoint, infoPoint, mapParent) {
        var component = Qt.createComponent("qrc:/qml/label.qml");

        if (component.status === Component.Ready) {
            var label = component.createObject(parent);
            label.coordinate = path[0];
            label.textName = codePoint + "\n" + infoPoint;
            label.angleRoute = getAngle(path[0], path[1]);
            mapParent.addMapItem(label);
        }
    }

    function createDetailsPath(points, details, mapParent) {
        var component = Qt.createComponent("qrc:/qml/DetailsPathLabel.qml");

        if (component.status === Component.Ready) {
            var detailsLabel = component.createObject(parent);
            detailsLabel.coordinate = getCenterOfSection(points);
            detailsLabel.nameAirway = details['codeAirway'];
            detailsLabel.distance = details['distance'];
            detailsLabel.setRotation(getAngle(points[0], points[1]));
            mapParent.addMapItem(detailsLabel);
        }
    }

    function createMagneticTrackAngle(points, magneticTrackAngle, mapParent) {
        var component = Qt.createComponent("qrc:/qml/MagneticTrackAngleLabel.qml");

        if (component.status === Component.Ready) {
            if (magneticTrackAngle.length > 0) {
                var labelForward = component.createObject(parent);
                labelForward.direction = "forward";
                labelForward.coordinate = points[0]
                labelForward.magneticTrackAngle = magneticTrackAngle[0];
                labelForward.setRotation(getAngle(points[0], points[1]));
                mapParent.addMapItem(labelForward);
            }
            if (magneticTrackAngle.length > 1) {
                var labelBack = component.createObject(parent);
                labelBack.direction = "back";
                labelBack.coordinate = points[1]
                labelBack.magneticTrackAngle = magneticTrackAngle[1];
                labelBack.setRotation(getAngle(points[1], points[0]));
                mapParent.addMapItem(labelBack);
            }
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
    }
}
