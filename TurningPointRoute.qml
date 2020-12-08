import QtQuick 2.0
import QtLocation 5.12

MapCircle {
    objectName: "turningPointRoute"
    property bool selected: false
    property string codeAirway: ""
    property string codePoint: ""
    radius: 800
    color: selected ? "red" : "#000"

    function select() {
        selected = !selected;
    }
}

