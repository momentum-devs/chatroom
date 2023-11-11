import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/components"
import "../../../qml/common/settings.js" as Settings

Rectangle {
    id: rightRectangle

    property var friendRequests: []
    property var friends: []

    function addFriend(friendName: string, friendId: string, isActive: bool) {
        friends.push([friendName, friendId, isActive]);
        friendsView.model = friends;
        friendsView.height = 30 * friends.length;
    }
    function addFriendRequest(friendName: string, requestId: string) {
        friendRequests.push([friendName, requestId]);
        friendRequestsView.model = friendRequests;
    }
    function clearFriendList() {
        friends = [];
        friendsView.model = friends;
    }
    function clearFriendRequestList() {
        friendRequests = [];
        friendRequestsView.model = friendRequests;
    }

    color: Settings.backgroundColor

    Column {
        id: rightColumn
        anchors.left: parent.left
        anchors.leftMargin: 7
        height: parent.height
        spacing: 5
        width: parent.width - 14

        Item {
            height: 6
            width: parent.width
        }
        Text {
            id: firendsText
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: 'Friends: '
        }
        ScrollView {
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true
            height: parent.height / 2 - firendsText.height
            width: parent.width

            Column {
                id: friendsColumn
                width: parent.width

                ListView {
                    id: friendsView
                    contentWidth: parent.width
                    height: 0
                    spacing: 5
                    width: parent.width

                    delegate: Row {
                        spacing: 5
                        width: parent.width

                        Button {
                            id: friendButton
                            text: modelData[0]
                            width: parent.width - firendActive.width - 5

                            onClicked: {
                                privateMessagesController.setCurrentFriend(modelData[1], modelData[0]);
                            }
                        }
                        ActivityIndicator {
                            anchors.verticalCenter: friendButton.verticalCenter
                            isActive: modelData[2]
                        }
                    }
                }
                Item {
                    height: addToFriend.height
                    width: rightColumn.width

                    AddToListButton {
                        id: addToFriend
                        anchors.horizontalCenter: parent.horizontalCenter

                        onClicked: {
                            privateMessagesController.goToSendFriendRequest()();
                        }
                    }
                }
            }
        }
        Separator {
            width: parent.width
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: 'Friend Requests:'
        }
        ScrollView {
            clip: true
            height: parent.height / 2
            width: parent.width

            ListView {
                id: friendRequestsView
                contentWidth: parent.width
                spacing: 5

                delegate: Column {
                    width: parent.width

                    Text {
                        color: Settings.textColor
                        text: modelData[0]
                    }
                    Row {
                        width: parent.width

                        AcceptButton {
                            id: acceptFriendRequest
                            onClicked: {
                                privateMessagesController.acceptFriendRequest(modelData[1]);
                            }
                        }
                        RejectButton {
                            onClicked: {
                                privateMessagesController.rejectFriendRequest(modelData[1]);
                            }
                        }
                    }
                }
            }
        }
    }
}
