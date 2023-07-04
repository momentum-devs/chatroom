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
            text: "Send friend request"
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

            Button {
                id: sendFriendRequestButton
                function activate() {
                    const friendEmail = friendEmailField.text;
                    if (friendEmail.length !== 0) {
                        sendFriendRequestController.sendFriendRequest(friendEmail);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Friend email field is empty";
                        errorPopup.open();
                    }
                }

                text: qsTr('Send friend request')

                Keys.onEnterPressed: activate()
                Keys.onReturnPressed: activate()
                onClicked: activate()
            }
            Button {
                id: goBackButton
                function activate() {
                    sendFriendRequestController.goBack();
                }

                text: qsTr('Go back')

                onClicked: activate()
            }
        }
    }
    Connections {
        function onSendFriendRequestFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: sendFriendRequestController
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
