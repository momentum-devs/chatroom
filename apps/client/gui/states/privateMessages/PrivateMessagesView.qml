import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/components"
import "../../qml/common/settings.js" as Settings
import "components"
import "../../qml/components/messages"

Rectangle {
    id: privateMessagesView
    color: Settings.backgroundColor
    focus: true

    Row {
        anchors.fill: parent

        LeftColumn {
            id: leftColumn
            height: parent.height
        }
        Item {
            width: parent.width - 2 * leftColumn.width
            height: parent.height

            Item {
                id: friendView
                anchors.fill: parent
                visible: false

                FriendTopBar {
                    id: friendTopBar
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                }
                MessageView {
                    id: messageView
                    anchors.left: parent.left
                    anchors.top: friendTopBar.bottom
                    height: parent.height - friendTopBar.height
                    width: parent.width
                }
            }
        }
        Separator {
            height: parent.height
        }
        FriendsColumn {
            id: friendsColumn
            height: parent.height
            width: leftColumn.width
        }
    }
    Connections {
        function onAddFriend(friendName: string, friendId: string, isActive: bool) {
            friendsColumn.addFriend(friendName, friendId, isActive);
        }

        function onAddFriendInvitation(friendName: string, requestId: string) {
            friendsColumn.addFriendInvitation(friendName, requestId);
        }

        function onClearFriendInvitationList() {
            friendsColumn.clearFriendInvitationList();
        }

        function onClearFriendList() {
            friendsColumn.clearFriendList();
        }

        function onRemovedFromFriends() {
            privateMessagesView.state = "OutsideChat"
        }

        function onSetCurrentFriendName(friendName: string) {
            friendTopBar.setFriendName(friendName);
            privateMessagesView.state = "OnChat"
            messageView.setTextPlaceholder("Message @" + friendName)
        }

        target: privateMessagesController
    }
    state: "OutsideChat"
    states: [
        State {
            name: "OnChat"
            PropertyChanges {
                target: friendView; visible: true
            }
        },
        State {
            extend: ""
            name: "OutsideChat"
            PropertyChanges {
                target: friendView; visible: false
            }

            PropertyChanges {
                target: friendsColumn
                width: leftColumn.width
            }
        }
    ]
}
