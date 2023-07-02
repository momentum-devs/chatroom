import QtQuick 6.2
import QtQuick.Controls 6.2

Rectangle {
    color: "grey"

    Keys.onEnterPressed: registerButton.activate()
    Keys.onReturnPressed: registerButton.activate()

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: usernameField
            placeholderText: qsTr('Email')
        }
        TextField {
            id: passwordField
            echoMode: TextInput.Password
            placeholderText: qsTr('Password')
        }
        TextField {
            id: passwordRepeatField
            echoMode: TextInput.Password
            placeholderText: qsTr('Repeat password')
        }
        Row {
            anchors.horizontalCenter: passwordField.horizontalCenter

            Button {
                id: registerButton
                function activate() {
                    const email = usernameField.text;
                    const password = passwordField.text;
                    const passwordRepeat = passwordRepeatField.text;
                    if (password === passwordRepeat && email.length !== 0 && password.length !== 0) {
                        registerController.registerRequest(email, password);
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
                text: qsTr('Go back')

                onClicked: {
                    registerController.goBack();
                }
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

