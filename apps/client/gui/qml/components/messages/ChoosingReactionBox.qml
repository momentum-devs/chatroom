import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

Popup {
    id: chooseReactionBox
    width: 180
    height: 200
    Text {
        id: headerText
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 10
        }

        text: qsTr("Choose reaction:")
        color: Settings.textColor
    }

    ReactionList {
        height: parent.height - headerText.height - 20
        width: parent.width - 20
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: headerText.bottom
            topMargin: 10
        }
    }


    background: Rectangle {
        anchors.fill: parent
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