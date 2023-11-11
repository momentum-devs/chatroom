import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Rectangle {
    id: leftRectangle
    color: "#2b2d31"
    height: parent.height
    width: leftColumn.width + 16

    Column {
        id: leftColumn
        anchors.left: parent.left
        anchors.leftMargin: 7
        height: parent.height
        spacing: 5

        Column {
            id: menuColumn
            spacing: 5
            width: parent.width

            Item {
                height: 6
                width: parent.width
            }
            Button {
                id: privateMessagesButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Private messages')
                width: parent.width

                onClicked: {
                    leftColumnController.goToPrivateMessages();
                }
            }
            Item {
                height: 1
                width: parent.width
            }
            Separator {
                width: parent.width
            }
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: 'Channels: '
        }
        ChannelList {
            id: channelList
            height: (parent.height - menuColumn.height) * 0.6
            width: parent.width
        }
        Separator {
            width: parent.width
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: 'Channel Invitations:'
        }
        ChannelInvitationList {
            id: channelInvitationList
            height: (parent.height - menuColumn.height) / 3
            width: parent.width
        }
    }
    SettingsBox {
        id: settingsBox
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
    }
    Connections {
        function onAddChannel(channelName: string, channelId: string, isOwner: bool) {
            channelList.addChannel(channelName, channelId, isOwner);
        }
        function onAddChannelInvitation(channelName: string, channelId: string) {
            channelInvitationList.addChannelInvitation(channelName, channelId);
        }
        function onClearChannelInvitationList() {
            channelInvitationList.clearChannelInvitationList();
        }
        function onClearChannelList() {
            channelList.clearChannelList();
        }
        function onSetUserName(nickname: string) {
            settingsBox.setUserName(nickname);
        }

        target: leftColumnController
    }
}
