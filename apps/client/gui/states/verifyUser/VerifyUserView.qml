import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor

    Keys.onEnterPressed: verifyButton.clicked()
    Keys.onReturnPressed: verifyButton.clicked()

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
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
            spacing: 5

            Button {
                id: verifyButton
                text: qsTr('Verify')

                onClicked: {
                    const verificationCode = verificationCodeField.text;
                    verifyUserController.verificationRequest(verificationCode);
                }
            }
            Button {
                id: goToLoginButton
                text: qsTr('Go to login')

                onClicked: {
                    verifyUserController.goToLoginState();
                }
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
