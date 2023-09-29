import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    color: "grey"
    width: Math.round(parent.width / 3)

    Keys.onEnterPressed: sendFriendRequestButton.activate()
    Keys.onEscapePressed: goBackButton.activate()

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
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

            Button {
                id: sendFriendRequestButton
                function activate() {
                    const email = emailField.text;
                    if (email.length !== 0) {
                        inviteToChannelController.sendChannelInvitation(email);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Email field is empty";
                        errorPopup.open();
                    }
                }

                text: qsTr('Send invitation to channel')

                Keys.onEnterPressed: activate()
                Keys.onReturnPressed: activate()
                onClicked: activate()
            }
            Button {
                id: goBackButton
                function activate() {
                    inviteToChannelController.goBack();
                }

                text: qsTr('Go back')

                onClicked: activate()
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
