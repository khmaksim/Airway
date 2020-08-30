import QtQuick 2.0
import QtLocation 5.13

MapQuickItem {
    property string codePoint: ""
    property color colorLabel: "#000"
    zoomLevel: 6.7

    sourceItem: Text {
        color: colorLabel
        text: codePoint
        font.pixelSize: 10
        anchors.left: parent.right
        anchors.leftMargin: 5
    }
}
