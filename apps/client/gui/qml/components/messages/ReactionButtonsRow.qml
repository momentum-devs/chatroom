import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

ListView {
    id: reactionRow
    orientation: ListView.Horizontal
    height: Settings.reactionButtonSize

    delegate: ReactionButton {
        width: Settings.reactionButtonSize
        height: Settings.reactionButtonSize
        icon.source: modelData.icon
        icon.color: "white"
        icon.height: Settings.reactionButtonSize
        icon.width: Settings.reactionButtonSize
        onClicked: {
            console.log("Clicked reaction button ", modelData.name)
        }
    }
}