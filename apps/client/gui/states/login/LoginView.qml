import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor

    Keys.onEnterPressed: loginButton.clicked()
    Keys.onReturnPressed: loginButton.clicked()

    Column {
        id: column
        anchors.centerIn: parent
        spacing: 10
        width: Math.round(parent.width / 3)

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: "Login"
        }
        TextField {
            id: usernameField
            anchors.horizontalCenter: parent.horizontalCenter
            focus: true
            placeholderText: qsTr('Email')
            width: parent.width
        }
        TextField {
            id: passwordField
            anchors.horizontalCenter: parent.horizontalCenter
            echoMode: TextInput.Password
            placeholderText: qsTr('Password')
            width: parent.width
        }
        Row {
            id: buttonRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Button {
                id: loginButton
                text: qsTr('Login')

                onClicked: {
                    const email = usernameField.text;
                    const password = passwordField.text;
                    if (email.length !== 0 && password.length !== 0) {
                        loginController.loginRequest(email, password);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Email or password field is empty";
                        errorPopup.open();
                    }
                    ab;
                }
            }
            Button {
                text: qsTr('Register')

                onClicked: {
                    loginController.goToRegisterState();
                }
            }
        }
    }
    Connections {
        function onLoginFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: loginController
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
            text: qsTr('Send login request')
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
