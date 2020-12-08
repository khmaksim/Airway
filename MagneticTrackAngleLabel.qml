import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtLocation 5.14

MapQuickItem {
    property string magneticTrackAngle: ""
    property string back: ""
    property color colorLabel: "#000"
    property double rotation: 0
    property double angleRoute: 0
    zoomLevel: 6.7

    onAngleRouteChanged: {
        if (angleRoute > 0 && angleRoute <= 90) {
            anchorPoint.x = labelMagneticTrackAngleForward.width
            anchorPoint.y = labelMagneticTrackAngleForward.height
        } else if (angleRoute > 90 && angleRoute <= 180) {
            anchorPoint.x = labelMagneticTrackAngleForward.x
            anchorPoint.y = labelMagneticTrackAngleForward.height
        } else if (angleRoute > 180 && angleRoute <= 270) {
            anchorPoint.x = labelMagneticTrackAngleForward.x
            anchorPoint.y = labelMagneticTrackAngleForward.y
        } else if (angleRoute > 270 && angleRoute <= 360) {
            anchorPoint.x = labelMagneticTrackAngleForward.width
            anchorPoint.y = labelMagneticTrackAngleForward.height
        }
    }

    FontMetrics {
        id: fontMetrics
        font.family: "Arial"
    }

    sourceItem: /*ColumnLayout {
        id: layout*/
        Text {
            id: labelMagneticTrackAngleForward
            Layout.fillWidth: true
            color: colorLabel
            text: magneticTrackAngle
            font.pixelSize: 6
            horizontalAlignment: Text.AlignHCenter

//            onTextChanged: parent.
            transform: Rotation { angle: rotation  }
        }
//        Text {
//            id: labelMagneticTrackAngleBack
//            Layout.fillWidth: true
//            color: colorLabel
//            text: magneticTrackAngle.length > 1 ? magneticTrackAngle[1] : ""
//            font.pixelSize: 6
//            horizontalAlignment: Text.AlignHCenter
//        }

//        spacing: 0
//    }

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
