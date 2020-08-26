import QtQuick 2.0
import QtLocation 5.13

MapQuickItem {
    property string nameZone: ""
    property string codeIcao: ""
    property string nameSector: ""
    property string call: ""
    property string func: ""
    property string freq: ""
    property color colorLabel: "#00D031"
    zoomLevel: 6.7

    sourceItem: Rectangle {
        id: label
        anchors.centerIn: parent
        border.color: colorLabel
        border.width: 1
        color: "transparent"
        width: 150
        height: zone.height + sector.height + callFuncFreq.height + 5

        Column {
            id: colLayout
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: zone
                color: colorLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: nameZone + " " + codeIcao
                font.pixelSize: 12

                onTextChanged: {
                    if (width > label.width)
                        label.width = width + 5;
                }
            }
            Text {
                id: sector
                color: colorLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: '(' + nameSector + ')'
                font.pixelSize: 12

                onTextChanged: {
                    if (width > label.width)
                        label.width = width + 5;
                }
            }
            Text {
                id: callFuncFreq
                color: colorLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: call + '-' + func + ' - ' + freq
                font.pixelSize: 8

                onTextChanged: {
                    if (width > label.width)
                        label.width = width + 5;
                }
            }
        }
    }
}
