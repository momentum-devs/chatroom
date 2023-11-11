import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

ScrollView {
    property var channels: []

    function addChannel(channelName: string, channelId: string, isOwner: bool) {
        channels.push([channelName, channelId, isOwner]);
        channelsView.model = channels;
        channelsView.height = 30 * channels.length;
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
                    leftColumnController.goToChannel(modelData[0], modelData[1], modelData[2]);
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
                    color: Settings.addColor
                    // font.bold: true
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
