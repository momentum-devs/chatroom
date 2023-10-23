import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    property var channelId: ""
    property var isOwner: false

    function setChannel(channel) {
        channelName.text = '<b>' + channel[0] + '</b>';
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

        Text {
            id: channelName
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Row {
            spacing: 5

            Button {
                id: addToChannelButton
                anchors.horizontalCenter: parent.horizontalRight
                text: qsTr('Add to channel')

                onClicked: {
                    mainController.addToChannel();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: "#FF0000"
                    text: qsTr('Left the channel')
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
                    text: qsTr('Delete the channel')
                }

                onClicked: {
                    mainController.deleteTheChannel();
                }
            }
        }
    }
}

