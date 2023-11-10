import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    property var channelId: ""
    property var isOwner: false

    function setChannel(channel) {
        channelName.text = channel[0];
        channelName.color = "white";
        channelName.font.pointSize = 18;
        channelId = channel[1];
        isOwner = channel[2];
        deleteChannelButton.visible = isOwner;
        addToChannelButton.visible = isOwner;
    }

    Column {
        id: channel
        height: parent.height
        spacing: 5
        width: parent.width

        Item {
            height: 6
            width: parent.width
        }
        Text {
            id: channelName
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Item {
            height: 6
            width: parent.width
        }
        Row {
            spacing: 5

            Button {
                id: addToChannelButton
                anchors.horizontalCenter: parent.horizontalRight
                text: qsTr('Invite')

                onClicked: {
                    mainController.addToChannel();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: "#FF0000"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr('Leave')
                }

                onClicked: {
                    mainController.leftTheChannel();
                }
            }
            Button {
                id: deleteChannelButton
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: "#FF0000"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr('Delete')
                }

                onClicked: {
                    mainController.deleteTheChannel();
                }
            }
            Button {
                id: channelMembersButton
                anchors.horizontalCenter: parent.horizontalRight
                text: qsTr('Members')

                onClicked: {
                    mainController.goToChannelMembersList();
                }
            }
        }
        Rectangle {
            color: '#3f4147'
            height: 1
            width: parent.width
        }
    }
}

