import QtQuick 6.2
import QtQuick.Controls 6.2

Rectangle {
    anchors.fill: parent
    color: "grey"

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
        Row {
            anchors.horizontalCenter: passwordField.horizontalCenter

            Button {
                text: qsTr('Login')

                onClicked: {
                    const email = usernameField.text;
                    const password = passwordField.text;
                    if (email.length !== 0 && password.length !== 0) {
                        loginController.loginRequest(email, password);
                        successPopup.open();
                    } else {
                        errorPopup.open();
                    }
                }
            }
            Button {
                text: qsTr('Go to register')

                onClicked: {
                    loginController.goToRegisterState();
                }
            }
        }
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
            text: qsTr('Successfully login')
            verticalAlignment: Text.AlignVCenter
        }
    }
    Popup {
        id: errorPopup
        height: 50
        width: 200
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
