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
            width: parent.width - leftColumn.width - friendsColumn.width

            FriendTopBar {
                id: friendTopBar
                visible: false
                width: parent.width
            }
            Item {
                id: defaultView
                height: parent.height - friendTopBar.height
                visible: true
                width: parent.width
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
        function onAddFriendRequest(friendName: string, requestId: string) {
            friendsColumn.addFriendRequest(friendName, requestId);
        }
        function onClearFriendList() {
            friendsColumn.clearFriendList();
        }
        function onClearFriendRequestList() {
            friendsColumn.clearFriendRequestList();
        }
        function onRemovedFromFriends() {
            friendTopBar.visible = false;
        }
        function onSetCurrentFriendName(friendName: string) {
            friendTopBar.setFriendName(friendName);
            friendTopBar.visible = true;
        }

        target: privateMessagesController
    }
}
