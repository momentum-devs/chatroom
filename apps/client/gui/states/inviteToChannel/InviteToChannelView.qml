import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor

    Keys.onEnterPressed: sendFriendInvitationButton.activate()
    Keys.onEscapePressed: goBackButton.activate()
    Keys.onReturnPressed: sendFriendInvitationButton.clicked()

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: "Send invitation to channel"
        }
        TextField {
            id: emailField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            placeholderText: qsTr('Email')
            width: parent.width
        }
        Row {
            anchors.horizontalCenter: emailField.horizontalCenter
            spacing: 5

            Button {
                id: sendFriendInvitationButton
                text: qsTr('Send invitation to channel')

                onClicked: {
                    const email = emailField.text;
                    if (email.length !== 0) {
                        inviteToChannelController.sendChannelInvitation(email);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Email field is empty";
                        errorPopup.open();
                    }
                }
            }
            Button {
                id: goBackButton
                text: qsTr('Go back')

                onClicked: {
                    inviteToChannelController.goBack();
                }
            }
        }
    }
    Connections {
        function onSendChannelInvitationFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: inviteToChannelController
    }
    Popup {
        id: successPopup
        height: 50
        width: 200
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) * 3 / 4)

        contentItem: Text {
            color: "green"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: qsTr('Invitation to channel succeed')
            verticalAlignment: Text.AlignVCenter
        }
    }
    Popup {
        id: errorPopup
        height: 50
        width: Math.round(parent.width / 2)
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) * 3 / 4)

        contentItem: Text {
            color: "red"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: qsTr('Invitation error!')
            verticalAlignment: Text.AlignVCenter
        }
    }
}
