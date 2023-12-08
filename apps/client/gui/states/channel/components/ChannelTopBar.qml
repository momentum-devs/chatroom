import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/common/settings.js" as Settings

Item {
    property string channelId: ""
    property bool isOwner: false

    function setChannel(channel) {
        channelName.text = channel[0];
        channelName.color = Settings.textColor;
        channelName.font.pointSize = 18;
        channelId = channel[1];
        isOwner = channel[2];
        deleteChannelButton.visible = isOwner;
        addToChannelButton.visible = isOwner;
    }

    height: channel.height
    width: channel.width

    Column {
        id: channel
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
                    channelController.addToChannel();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: Settings.leaveColor
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr('Leave')
                }

                onClicked: {
                    channelController.leaveChannel();
                }
            }
            Button {
                id: deleteChannelButton
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: Settings.deleteColor
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr('Delete')
                }

                onClicked: {
                    channelController.deleteChannel();
                }
            }
        }
        Rectangle {
            color: Settings.separatorColor
            height: 1
            width: parent.width
        }
    }
}

