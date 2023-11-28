import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor

    Keys.onEnterPressed: sendFriendInvitationButton.clicked()
    Keys.onEscapePressed: goBackButton.clicked()
    Keys.onReturnPressed: sendFriendInvitationButton.clicked()

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: "Invite friend:"
        }
        TextField {
            id: friendEmailField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            placeholderText: qsTr('Friend email')
            width: parent.width
        }
        Row {
            anchors.horizontalCenter: friendEmailField.horizontalCenter
            spacing: 5

            Button {
                id: sendFriendInvitationButton
                text: qsTr('Invite')

                onClicked: {
                    const friendEmail = friendEmailField.text;
                    if (friendEmail.length !== 0) {
                        sendFriendInvitationController.sendFriendInvitation(friendEmail);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Friend email field is empty";
                        errorPopup.open();
                    }
                }
            }
            Button {
                id: goBackButton
                text: qsTr('Go back')

                onClicked: {
                    sendFriendInvitationController.goBack();
                }
            }
        }
    }
    Connections {
        function onSendFriendInvitationFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: sendFriendInvitationController
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
            text: qsTr('Send friend request')
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
            text: qsTr('Login error!')
            verticalAlignment: Text.AlignVCenter
        }
    }
}
