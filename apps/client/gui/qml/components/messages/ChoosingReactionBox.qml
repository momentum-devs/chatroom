import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

Popup {
    width: 120
    height: 250

    Rectangle {
        anchors.fill: parent
        z: 1
        property bool isMouseInside: false
        id: chooseReactionBox
        visible: false
        enabled: false
        radius: 5
        color: Settings.hoverMessageColor
        border.color: Settings.boxColor
        border.width: 1
    }

    function disable() {
        chooseReactionBox.visible = false
        chooseReactionBox.enabled = false
    }
}