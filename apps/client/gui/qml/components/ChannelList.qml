import QtQuick 6.4
import QtQuick.Controls 6.4

ScrollView {
    property var channels: []

    function addChannel(channelName: string, channelId: string, isOwner: bool) {
        channels.push([channelName, channelId, isOwner]);
        channelsView.model = channels;
        channelsView.height = (privateMessagesButton.height + 5) * channels.length;
    }
    function clearChannelList() {
        channels = [];
        channelsView.model = channels;
    }

    clip: true
    spacing: 5

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
                    leftColumnController.setCurrentChannel(modelData[1]);
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
                    leftColumnController.goToCreateChannel();
                }
            }
        }
    }
}
