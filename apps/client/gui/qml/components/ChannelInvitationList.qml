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

        delegate: Invitation {
            invitationName: modelData[0]
            width: parent.width

            onAccepted: {
                leftColumnController.acceptChannelInvitation(modelData[1]);
            }
            onRejected: {
                leftColumnController.rejectChannelInvitation(modelData[1]);
            }
        }
    }
}
