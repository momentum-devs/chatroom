import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    height: 480
    width: 720

    Rectangle {
        anchors.fill: parent
        color: "grey"

        Column {
            anchors.centerIn: parent
            spacing: 10

            TextField {
                id: usernameField
                placeholderText: qsTr('User name')
            }
            TextField {
                id: passwordField
                echoMode: TextInput.Password
                placeholderText: "Hasło"
            }
            TextField {
                id: passwordRepeatField
                echoMode: TextInput.Password
                placeholderText: "Powtórz hasło"
            }
            Button {
                anchors.horizontalCenter: passwordField.horizontalCenter
                text: "Zarejestruj"

                onClicked: {
                    var username = usernameField.text;
                    var password = passwordField.text;
                    var passwordRepeat = passwordRepeatField.text;
                    if (password === passwordRepeat && username.length != 0 && password.length != 0) {
                        successPopup.open();
                        MainView.registerRequest(username, password);
                    } else {
                        errorPopup.open();
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
                text: "Zalogowano pomyślnie!"
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
                text: "Błąd logowania!"
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
