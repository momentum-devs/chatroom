import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    property var channelInvitations: []
    property var channels: []

    color: "grey"
    focus: true

    Row {
        anchors.fill: parent

        Column {
            id: leftColumn
            height: parent.height
            spacing: 5

            Column {
                id: menuColumn
                spacing: 5
                width: parent.width

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
                Rectangle {
                    color: 'black'
                    height: 2
                    width: parent.width
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: '<b>Channels:</b> '
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
                        anchors.horizontalCenter: parent.horizontalCenter
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
                color: 'black'
                height: 2
                width: parent.width
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: '<b>Channel Invitations:</b> '
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
                            text: '<b>' + modelData[0] + ':    </b>'
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
            color: 'black'
            height: parent.height
            width: 2
        }
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
            color: 'black'
            height: parent.height
            width: 2
        }
        Column {
            id: rightColumn
            width: leftColumn.width

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: '<b>Friends:</b> '
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
        function onClearChannelList() {
            channels = [];
            channelsView.model = channels;
        }

        target: mainController
    }
}
