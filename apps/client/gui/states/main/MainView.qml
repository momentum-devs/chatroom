import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    property var channels: []

    color: "grey"
    focus: true

    Row {
        anchors.fill: parent
        spacing: 5

        Column {
            id: menuColumn
            height: parent.height
            spacing: 5

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Create channel')

                onClicked: {
                    mainController.goToCreateChannel();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Send friend request')

                onClicked: {
                    mainController.goToSendFriendRequest();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('User settings')

                onClicked: {
                    mainController.goToUserSettings();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Logout')

                onClicked: {
                    mainController.logout();
                }
            }
            Rectangle {
                color: 'black'
                height: 2
                width: parent.width
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: '<b>Channels:</b> '
            }
            ScrollView {
                height: parent.height / 3
                width: parent.width

                ListView {
                    id: channelsView
                    contentWidth: parent.width

                    delegate: Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: modelData[0]

                        onClicked: {
                            console.log('clicked channel ' + modelData[0] + ' with id ' + modelData[1]);
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
                text: '<b>Friends:</b> '
            }
        }
        Rectangle {
            color: 'black'
            height: parent.height
            width: 2
        }
    }
    Connections {
        function onAddChannel(channelName: string, channelId: string) {
            channels.push([channelName, channelId]);
            channelsView.model = channels;
        }

        target: mainController
    }
}
