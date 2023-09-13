import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    anchors.fill: parent
    color: "grey"
    width: Math.round(parent.width / 3)

    Keys.onEnterPressed: verifyButton.activate()
    Keys.onReturnPressed: verifyButton.activate()

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Enter code from email to verify"
        }
        TextField {
            id: verificationCodeField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            placeholderText: qsTr('Verification Code')
            width: parent.width
        }
        Row {
            anchors.horizontalCenter: verificationCodeField.horizontalCenter

            Button {
                id: verifyButton
                function activate() {
                    const verificationCode = verificationCodeField.text;
                    verifyUserController.verificationRequest(verificationCode);
                }

                text: qsTr('Verify')

                Keys.onEnterPressed: activate()
                Keys.onReturnPressed: activate()
                onClicked: activate()
            }
            Button {
                id: goToLoginButton
                function activate() {
                    verifyUserController.goToLoginState();
                }

                text: qsTr('Go to login')

                onClicked: activate()
            }
        }
    }
    Connections {
        function onVerificationFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: verifyUserController
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