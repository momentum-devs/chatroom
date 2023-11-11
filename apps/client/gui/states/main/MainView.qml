import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../qml/common/settings.js" as Settings

Rectangle {
    property var channelInvitations: []
    property var channels: []
    property var friendRequests: []
    property var friends: []

    color: Settings.backgroundColor
    focus: true

    Row {
        anchors.fill: parent

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
                            console.log("private messages");
                        }
                    }
                    Item {
                        height: 1
                        width: parent.width
                    }
                    Rectangle {
                        color: '#3f4147'
                        height: 1
                        width: parent.width
                    }
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    text: 'Channels: '
                }
                ScrollView {
                    clip: true
                    height: (parent.height - menuColumn.height) * 0.6
                    spacing: 5
                    width: parent.width

                    Column {
                        id: channelsColumn
                        width: parent.width

                        ListView {
                            id: channelsView
                            contentWidth: parent.width
                            spacing: 5
                            width: parent.width

                            delegate: Button {
                                id: channelButton
                                height: privateMessagesButton.height
                                text: modelData[0]
                                width: leftColumn.width

                                onClicked: {
                                    channelView.visible = true;
                                    defaultView.visible = false;
                                    friendView.visible = false;
                                    channelView.setChannel(modelData);
                                    mainController.setCurrentChannel(modelData[1]);
                                }
                            }
                        }
                        Item {
                            height: createChannelButton.height
                            width: parent.width

                            RoundButton {
                                id: createChannelButton
                                anchors.horizontalCenter: parent.horizontalCenter
                                height: 35
                                width: 35

                                contentItem: Text {
                                    color: "#00FF00"
                                    font.bold: true
                                    font.pointSize: 20
                                    horizontalAlignment: Text.AlignHCenter
                                    text: '+'
                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    mainController.goToCreateChannel();
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    color: '#3f4147'
                    height: 1
                    width: parent.width
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    text: 'Channel Invitations:'
                }
                ScrollView {
                    clip: true
                    height: (parent.height - menuColumn.height) / 3
                    width: parent.width

                    ListView {
                        id: channelInvitationsView
                        contentWidth: parent.width
                        spacing: 5

                        delegate: Row {
                            height: 30
                            width: parent.width

                            Text {
                                color: "white"
                                text: modelData[0]
                            }
                            Button {
                                id: acceptChannelInvitation
                                width: 30

                                contentItem: Text {
                                    color: "#00FF00"
                                    text: qsTr('✓')
                                }

                                onClicked: {
                                    mainController.acceptChannelInvitation(modelData[1]);
                                }
                            }
                            Button {
                                width: 30

                                contentItem: Text {
                                    color: "#FF0000"
                                    text: qsTr('✕')
                                }

                                onClicked: {
                                    mainController.rejectChannelInvitation(modelData[1]);
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                color: "#232428"
                height: settingsButton.height + 16
                width: parent.width

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    font.pointSize: 11
                    text: "user nickname"
                }
                Button {
                    id: settingsButton
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    height: 28
                    width: 28

                    contentItem: Text {
                        color: "white"
                        font.pointSize: 17
                        horizontalAlignment: Text.AlignHCenter
                        text: '⛭'
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        mainController.goToUserSettings();
                    }
                }
            }
        }
        ChannelView {
            id: channelView
            height: parent.height
            visible: false
            width: parent.width - leftRectangle.width - rightRectangle.width
        }
        Item {
            id: defaultView
            height: parent.height
            visible: true
            width: parent.width - leftRectangle.width - rightRectangle.width
        }
        FriendView {
            id: friendView
            height: parent.height
            visible: false
            width: parent.width - leftRectangle.width - rightRectangle.width
        }
        Rectangle {
            color: '#3f4147'
            height: parent.height
            width: 1
        }
        Rectangle {
            id: rightRectangle
            color: "#313338"
            height: parent.height
            width: leftRectangle.width

            Column {
                id: rightColumn
                anchors.left: parent.left
                anchors.leftMargin: 7
                height: parent.height
                spacing: 5
                width: leftColumn.width

                Item {
                    height: 6
                    width: parent.width
                }
                Text {
                    id: firendsText
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    text: 'Friends: '
                }
                ScrollView {
                    clip: true
                    height: parent.height / 2 - firendsText.height
                    width: parent.width

                    Column {
                        id: friendsColumn
                        width: parent.width

                        ListView {
                            id: friendsView
                            contentWidth: parent.width
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
                                        channelView.visible = false;
                                        defaultView.visible = false;
                                        friendView.visible = true;
                                        friendView.setFriend(modelData);
                                        mainController.setCurrentFriend(modelData[1]);
                                    }
                                }
                                Text {
                                    id: firendActive
                                    anchors.verticalCenter: friendButton.verticalCenter
                                    color: "green"
                                    font.pointSize: 12
                                    text: "●"
                                    visible: modelData[2]
                                }
                                Text {
                                    anchors.verticalCenter: friendButton.verticalCenter
                                    color: "grey"
                                    font.pointSize: 12
                                    text: "○"
                                    visible: !modelData[2]
                                }
                            }
                        }
                        Item {
                            height: addToFriend.height
                            width: parent.width

                            RoundButton {
                                id: addToFriend
                                anchors.horizontalCenter: parent.horizontalCenter
                                height: 35
                                width: 35

                                contentItem: Text {
                                    color: "#00FF00"
                                    font.bold: true
                                    font.pointSize: 20
                                    horizontalAlignment: Text.AlignHCenter
                                    text: '+'
                                    verticalAlignment: Text.AlignVCenter
                                }

                                onClicked: {
                                    mainController.goToSendFriendRequest()();
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    color: '#3f4147'
                    height: 1
                    width: parent.width
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
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
                                color: "white"
                                text: modelData[0]
                            }
                            Row {
                                width: parent.width

                                Button {
                                    id: acceptFriendRequest
                                    width: 30

                                    contentItem: Text {
                                        color: "#00FF00"
                                        text: qsTr('✓')
                                    }

                                    onClicked: {
                                        mainController.acceptFriendRequest(modelData[1]);
                                    }
                                }
                                Button {
                                    width: 30

                                    contentItem: Text {
                                        color: "#FF0000"
                                        text: qsTr('✕')
                                    }

                                    onClicked: {
                                        mainController.rejectFriendRequest(modelData[1]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Connections {
        function onAddChannel(channelName: string, channelId: string, isOwner: bool) {
            channels.push([channelName, channelId, isOwner]);
            channelsView.model = channels;
            channelsView.height = (privateMessagesButton.height + 5) * channels.length;
        }
        function onAddChannelInvitation(channelName: string, channelId: string) {
            channelInvitations.push([channelName, channelId]);
            channelInvitationsView.model = channelInvitations;
        }
        function onAddFriend(friendName: string, friendId: string, isActive: bool) {
            friends.push([friendName, friendId, isActive]);
            friendsView.model = friends;
            friendsView.height = (privateMessagesButton.height + 5) * friends.length;
        }
        function onAddFriendRequest(friendName: string, requestId: string) {
            friendRequests.push([friendName, requestId]);
            friendRequestsView.model = friendRequests;
        }
        function onClearChannelInvitationList() {
            channelInvitations = [];
            channelInvitationsView.model = channelInvitations;
        }
        function onClearChannelList() {
            channels = [];
            channelsView.model = channels;
        }
        function onClearFriendList() {
            friends = [];
            friendsView.model = friends;
        }
        function onClearFriendRequestList() {
            friendRequests = [];
            friendRequestsView.model = friendRequests;
        }
        function onReturnToDefaultView() {
            channelView.visible = false;
            defaultView.visible = true;
            friendView.visible = false;
        }

        target: mainController
    }
}
