import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtLocation 5.12

MapQuickItem {
    property string nameAirway: ""
    property string distance: ""
    property color colorLabel: "#000"
    property double rotation: 0
    property double angleRoute: 0
    zoomLevel: 6.7

    onAngleRouteChanged: {
        if (angleRoute > 0 && angleRoute <= 90) {
            anchorPoint.x = nameLabel.width
            anchorPoint.y = nameLabel.height
        } else if (angleRoute > 90 && angleRoute <= 180) {
            anchorPoint.x = nameLabel.x
            anchorPoint.y = nameLabel.height
        } else if (angleRoute > 180 && angleRoute <= 270) {
            anchorPoint.x = nameLabel.x
            anchorPoint.y = nameLabel.y
        } else if (angleRoute > 270 && angleRoute <= 360) {
            anchorPoint.x = nameLabel.width
            anchorPoint.y = nameLabel.y
        }
        // Set padding to point
//        anchorPoint.x = anchorPoint.x + 3
//        anchorPoint.y = anchorPoint.y + 3
    }

    onNameAirwayChanged: {
//        anchorPoint.x = fontMetrics.advanceWidth(textName) / 2;
    }

    FontMetrics {
        id: fontMetrics
        font.family: "Arial"
    }

    sourceItem: ColumnLayout {
        Text {
            id: labelDistance
//            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
//            Layout.fillHeight: true
            //        y: anchorPoint.y - fontMetrics.boundingRect(textName).height
            color: colorLabel
            text: distance
            font.pixelSize: 6
            horizontalAlignment: Text.AlignHCenter
//            transform: Rotation { angle: rotation  }
        }
        Text {
            id: labelAirway
//            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
//            Layout.fillHeight: true
            //        y: anchorPoint.y - fontMetrics.boundingRect(textName).height
            color: colorLabel
            text: nameAirway
            font.pixelSize: 6
            horizontalAlignment: Text.AlignHCenter
//            transform: Rotation { angle: rotation  }
        }
        transform: Rotation { angle: rotation  }
        spacing: 0
    }

    function setRotation(rot) {
        if (rot >= 0 && rot < 90)
            rotation = (90 - rot) * -1;
        else if (rot >= 90 && rot < 180)
            rotation = (90 - (180 - rot));
        else if (rot >= 180 && rot < 270)
            rotation = (270 - rot) * -1;
        else if (rot >= 270 && rot <= 360)
            rotation = (90 - (360 - rot));
    }
}
