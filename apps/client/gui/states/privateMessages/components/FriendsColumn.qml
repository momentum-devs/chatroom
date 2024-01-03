import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/components"
import "../../../qml/common/settings.js" as Settings

Rectangle {
    id: rightRectangle

    property var friendInvitations: []
    property var friends: []

    function addFriend(friendName: string, friendId: string, isActive: bool) {
        friends.push([friendName, friendId, isActive]);
        friendsView.model = friends;
        friendsView.height = 36 * friends.length;
    }

    function addFriendInvitation(friendName: string, requestId: string) {
        friendInvitations.push([friendName, requestId]);
        friendInvitationsView.model = friendInvitations;
    }

    function clearFriendInvitationList() {
        friendInvitations = [];
        friendInvitationsView.model = friendInvitations;
    }

    function clearFriendList() {
        friends = [];
        friendsView.model = friends;
        friendsView.height = 0;
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

                    delegate: Item {
                        height: friendButton.height + firendActive.height * 0.5
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
                            id: firendActive
                            anchors.horizontalCenter: friendButton.right
                            anchors.verticalCenter: friendButton.bottom
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
                            privateMessagesController.goToSendFriendInvitation()();
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
                id: friendInvitationsView
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
                            id: acceptFriendInvitation
                            onClicked: {
                                privateMessagesController.acceptFriendInvitation(modelData[1]);
                            }
                        }
                        RejectButton {
                            onClicked: {
                                privateMessagesController.rejectFriendInvitation(modelData[1]);
                            }
                        }
                    }
                }
            }
        }
    }
}
