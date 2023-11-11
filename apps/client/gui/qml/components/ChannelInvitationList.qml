import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

ScrollView {
    property var channelInvitations: []

    function addChannelInvitation(channelName: string, channelId: string) {
        channelInvitations.push([channelName, channelId]);
        channelInvitationsView.model = channelInvitations;
    }
    function clearChannelInvitationList() {
        channelInvitations = [];
        channelInvitationsView.model = channelInvitations;
    }

    clip: true

    ListView {
        id: channelInvitationsView
        contentWidth: parent.width
        spacing: 5

        delegate: Row {
            height: 30
            width: parent.width

            Text {
                color: Settings.textColor
                text: modelData[0]
            }
            Button {
                id: acceptChannelInvitation
                width: 30

                contentItem: Text {
                    color: Settings.acceptColor
                    text: qsTr('✓')
                }

                onClicked: {
                    leftColumnController.acceptChannelInvitation(modelData[1]);
                }
            }
            Button {
                width: 30

                contentItem: Text {
                    color: Settings.rejectColor
                    text: qsTr('✕')
                }

                onClicked: {
                    leftColumnController.rejectChannelInvitation(modelData[1]);
                }
            }
        }
    }
}
