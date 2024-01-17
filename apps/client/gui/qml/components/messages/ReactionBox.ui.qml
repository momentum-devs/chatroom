import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

Rectangle {
    z: 1
    id: reactionBox
    visible: false
    width: 60
    height: 25
    radius: 5
    color: Settings.hoverMessageColor
    border.color: Settings.boxColor
    border.width: 1

        signal
    addReaction

    CustomButton {
        id: reactionButton
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        size: 25
        height: size
        icon.color: color
        icon.height: size
        icon.source: "../../../../resources/svg/happy-face.svg"
        icon.width: size
        width: size
        enabled: true

        background: Rectangle {
            height: reactionButton.size
            opacity: 0.5
            radius: 3
            visible: false
            width: reactionButton.size
        }

        onMouseEnteredArea: {
            background.visible = true;
        }
        onMouseExitedArea: {
            background.visible = false;
        }
        onClicked: {
            reactionBox.addReaction();
        }
    }
}