import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/components"
import "../../qml/common/settings.js" as Settings
import "components"
import "../../qml/components/messages"

Rectangle {
    id: channelView
    color: Settings.backgroundColor
    focus: true
    Keys.onEnterPressed: messageView.sendMessage()

    Row {
        anchors.fill: parent

        LeftColumn {
            id: leftColumn
        }
        Item {
            height: parent.height
            width: parent.width - leftColumn.width - membersColumn.width

            ChannelTopBar {
                id: channelTopBar
                width: parent.width
                anchors.left: parent.left
                anchors.top: parent.top
            }
            MessageView {
                id: messageView
                anchors.left: parent.left
                anchors.top: channelTopBar.bottom
                height: parent.height - channelTopBar.height
                width: parent.width
            }
        }
        Separator {
            height: parent.height
        }
        MembersColumn {
            id: membersColumn
            height: parent.height
            width: leftColumn.width
        }
    }
    Connections {
        function onAddMember(memberName: string, memberId: string, isActive: bool) {
            membersColumn.addMember(memberName, memberId, isActive);
        }

        function onClearMembersList() {
            membersColumn.clearMembersList();
        }

        function onSetChannel(channelName: string, channelId: string, isOwner: bool) {
            channelTopBar.setChannel([channelName, channelId, isOwner]);
            messageView.setTextPlaceholder("Message #" + channelName)
            channelView.focus = true;
        }

        target: channelController
    }
}
