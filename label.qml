import QtQuick 2.0
import QtLocation 5.14

MapQuickItem {
    property string name: ""
    property color colorLabel: "#000"
    zoomLevel: 6.7

    sourceItem: Text {
        color: colorLabel
        text: name
        font.pixelSize: 6
        anchors.left: parent.right
        anchors.leftMargin: 5
    }
}
