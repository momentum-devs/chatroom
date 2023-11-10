import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    id: channelMembersView
    anchors.fill: parent
    color: "#313338"

    Column {
        anchors.centerIn: parent
        spacing: 5

        Row {
            id: membersRow
            anchors.horizontalCenter: parent.horizontalCenter
            height: channelMembersView.height * 0.8
            width: channelMembersView.height * 0.8

            Column {
                id: memberList
                height: parent.height
                spacing: 5
                width: parent.width / 2

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    font.pointSize: 16
                    text: qsTr('Members:')
                }
            }
            Column {
                id: invitationList
                height: parent.height
                spacing: 5
                width: parent.width / 2

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    font.pointSize: 16
                    text: qsTr('Invitations:')
                }
            }
        }
        Button {
            id: goBackButton
            function activate() {
                channelMembersController.goBack();
            }

            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr('Go back')

            onClicked: activate()
        }
    }
}
