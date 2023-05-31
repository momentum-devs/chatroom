
import QtQuick 2.14
// import QtQuick.Controls2 2.14

Rectangle{
    anchors.fill: parent

    MainView {
        id: mainView
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: usernameField
            placeholderText: "Nazwa użytkownika"
        }

        TextField {
            id: passwordField
            placeholderText: "Hasło"
            echoMode: TextInput.Password
        }

        TextField {
            id: passwordRepeatField
            placeholderText: "Powtórz hasło"
            echoMode: TextInput.Password
        }

        Button {
            text: "Zarejestruj"
            anchors.horizontalCenter: passwordField.horizontalCenter
            onClicked: {
                var username = usernameField.text
                var password = passwordField.text
                var passwordRepeat = passwordRepeatField.text

                if (password === passwordRepeat) {
                    successPopup.open()

                    //TODO: implement signal to client app
                    console.log("Użytkownik: ", username)
                    console.log("Hasło: ", password)
                    mainView.onRegisterRequest()
                } else {
                    errorPopup.open()
                }
            }
        }
    }

    Popup {
        id: successPopup
        width: 200
        height: 50
        anchors.centerIn: parent
        contentItem: Text {
            text: "Zalogowano pomyślnie!"
            color: "green"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Popup {
        id: errorPopup
        width: 200
        height: 50
        anchors.centerIn: parent
        contentItem: Text {
            text: "Błąd logowania!"
            color: "red"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
