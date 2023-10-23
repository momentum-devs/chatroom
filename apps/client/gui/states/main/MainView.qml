import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    property var channelInvitations: []
    property var channels: []

    color: "#313338"
    focus: true

    Row {
        anchors.fill: parent

        Rectangle {
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
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr('Create channel')
                        width: parent.width

                        onClicked: {
                            mainController.goToCreateChannel();
                        }
                    }
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr('Invite friend')
                        width: parent.width

                        onClicked: {
                            mainController.goToSendFriendRequest();
                        }
                    }
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr('Settings')
                        width: parent.width

                        onClicked: {
                            mainController.goToUserSettings();
                        }
                    }
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr('Logout')
                        width: parent.width

                        onClicked: {
                            mainController.logout();
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
                    height: (parent.height - menuColumn.height) / 2
                    width: parent.width

                    ListView {
                        id: channelsView
                        contentWidth: parent.width
                        spacing: 5
                        width: parent.width

                        delegate: Button {
                            text: modelData[0]
                            width: parent.width

                            onClicked: {
                                channelView.visible = true;
                                defaultView.visible = false;
                                channelView.setChannel(modelData);
                                mainController.setCurrentChannel(modelData[1]);
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
                    height: (parent.height - menuColumn.height) / 2
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
        }
        // Rectangle {
        //     color: '#3f4147'
        //     height: parent.height
        //     width: 1
        // }
        ChannelView {
            id: channelView
            height: parent.height
            visible: false
            width: parent.width - leftColumn.width - rightColumn.width
        }
        Item {
            id: defaultView
            height: parent.height
            visible: true
            width: parent.width - leftColumn.width - rightColumn.width
        }
        Rectangle {
            color: '#3f4147'
            height: parent.height
            width: 1
        }
        Column {
            id: rightColumn
            width: leftColumn.width

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                text: 'Friends: '
            }
        }
    }
    Connections {
        function onAddChannel(channelName: string, channelId: string, isOwner: bool) {
            channels.push([channelName, channelId, isOwner]);
            channelsView.model = channels;
        }
        function onAddChannelInvitation(channelName: string, channelId: string) {
            channelInvitations.push([channelName, channelId]);
            channelInvitationsView.model = channelInvitations;
        }
        function onClearChannelInvitationList() {
            channelInvitations = [];
            channelInvitationsView.model = channelInvitations;
        }
        function onClearChannelList() {
            channels = [];
            channelsView.model = channels;
        }

        target: mainController
    }
}
