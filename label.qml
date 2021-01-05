import QtQuick 2.14
import QtLocation 5.14

MapQuickItem {
    property string textName: ""
    property color colorLabel: "#000"
    property double rotation: 0
    property double angleRoute: 0
    zoomLevel: 6.7

    onRotationChanged: {
        if (rotation >= 0 && rotation < 90)
            rotation = (90 - rotation) * -1;
        else if (rotation >= 90 && rotation < 180)
            rotation = (90 - (180 - rotation));
        else if (rotation >= 180 && rotation < 270)
            rotation = (270 - rotation) * -1;
        else if (rotation >= 270 && rotation <= 360)
            rotation = (90 - (360 - rotation));
    }

    onAngleRouteChanged: {
//        console.log(angleRoute)
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
    }

    onTextNameChanged: {
//        anchorPoint.x = fontMetrics.advanceWidth(textName) / 2;
//        console.log(fontMetrics.advanceWidth(textName));
    }

    FontMetrics {
        id: fontMetrics
        font.family: "Arial"
    }

    sourceItem: Text {
        id: nameLabel
//        y: anchorPoint.y - fontMetrics.boundingRect(textName).height
        color: colorLabel
        text: textName
        font.pixelSize: 6
        transform: Rotation { angle: rotation  }
    }
}
