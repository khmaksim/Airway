import QtQuick 2.0
import QtLocation 5.14

MapCircle {
    objectName: "turningPointRoute"
    property bool selected: false
    property string codeAirway: ""
    property string codePoint: ""
    radius: 200
    color: selected ? "red" : "#fff"

    function select() {
        selected = !selected;
    }
}

