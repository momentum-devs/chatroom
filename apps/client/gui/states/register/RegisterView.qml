import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor

    Keys.onEnterPressed: registerUserButton.activate()
    Keys.onEscapePressed: goBackButton.activate()

    Column {
        anchors.centerIn: parent
        spacing: 10
        width: Math.round(parent.width / 3)

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: "Register"
        }
        TextField {
            id: emailField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            placeholderText: qsTr('Email')
            width: parent.width
        }
        TextField {
            id: nicknameField
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr('Nickname')
            width: parent.width
        }
        TextField {
            id: passwordField
            anchors.horizontalCenter: parent.horizontalCenter
            echoMode: TextInput.Password
            placeholderText: qsTr('Password')
            width: parent.width
        }
        TextField {
            id: passwordRepeatField
            anchors.horizontalCenter: parent.horizontalCenter
            echoMode: TextInput.Password
            placeholderText: qsTr('Repeat password')
            width: parent.width
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Button {
                id: registerUserButton
                function activate() {
                    const email = emailField.text;
                    const nickname = nicknameField.text;
                    const password = passwordField.text;
                    const passwordRepeat = passwordRepeatField.text;
                    if (password === passwordRepeat && email.length !== 0 && password.length !== 0 && nickname.length !== 0) {
                        registerController.registerRequest(email, nickname, password);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Incorrect register data";
                        errorPopup.open();
                    }
                }

                text: qsTr('Register')

                onClicked: activate()
            }
            Button {
                id: goBackButton
                function activate() {
                    registerController.goBack();
                }

                text: qsTr('Go back')

                onClicked: activate()
            }
        }
    }
    Connections {
        function onRegisterFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: registerController
    }
    Popup {
        id: successPopup
        height: 50
        width: Math.round(parent.width / 2)
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) * 3 / 4)

        contentItem: Text {
            color: "green"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: qsTr('Send register request')
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
            text: qsTr('Register error!')
            verticalAlignment: Text.AlignVCenter
        }
    }
}

