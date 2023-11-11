import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/components"
import "../../qml/common/settings.js" as Settings
import "components"

Rectangle {
    color: Settings.backgroundColor
    focus: true

    Row {
        anchors.fill: parent

        LeftColumn {
            id: leftColumn
        }
        Column {
            height: parent.height
            width: parent.width - leftColumn.width

            ChannelTopBar {
                id: channelTopBar
                width: parent.width
            }
            Item {
                id: defaultView
                height: parent.height - channelTopBar.height
                visible: true
                width: parent.width
            }
        }
        Rectangle {
            color: Settings.separatorColor
            height: parent.height
            width: 1
        }
    }
    Connections {
        function onSetChannel(channelName: string, channelId: string, isOwner: bool) {
            channelTopBar.setChannel([channelName, channelId, isOwner]);
        }

        target: channelController
    }
}
