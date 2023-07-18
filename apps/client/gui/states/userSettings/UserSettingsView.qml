import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    color: "grey"

    Column {
        anchors.centerIn: parent
        spacing: 10
        width: Math.round(parent.width / 3)

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "User Settings"
        }
        TextField {
            id: emailField
            readOnly: true
            text: "example@email.com"
            width: parent.width
        }
        TextField {
            id: nicknameField
            focus: true
            placeholderText: qsTr('Nickname')
            width: parent.width
        }
        Button {
            id: changeNicknaem
            function activate() {
                const nickname = nicknameField.text;
                if (nickname.length !== 0) {
                    userSettingsController.changeNickname(nickname);
                    successPopup.open();
                } else {
                    errorPopup.contentItem.text = "Empty nickname field";
                    errorPopup.open();
                }
            }

            anchors.horizontalCenter: nicknameField.horizontalCenter
            text: qsTr('Change nickname')

            onClicked: activate()
        }
        TextField {
            id: passwordField
            echoMode: TextInput.Password
            placeholderText: qsTr('Password')
            width: parent.width
        }
        TextField {
            id: passwordRepeatField
            echoMode: TextInput.Password
            placeholderText: qsTr('Repeat password')
            width: parent.width
        }
        Button {
            id: changePassword
            function activate() {
                const password = passwordField.text;
                const passwordRepeat = passwordRepeatField.text;
                if (password === passwordRepeat && password.length !== 0) {
                    userSettingsController.changePassword(password);
                    successPopup.open();
                } else {
                    errorPopup.contentItem.text = "Passwords mismatch";
                    errorPopup.open();
                }
            }

            anchors.horizontalCenter: passwordRepeatField.horizontalCenter
            text: qsTr('Change password')

            onClicked: activate()
        }
        Button {
            id: deleteUser
            function activate() {
                userSettingsController.deleteUser();
            }

            anchors.horizontalCenter: passwordRepeatField.horizontalCenter

            contentItem: Text {
                color: "#FF0000"
                text: qsTr('Delete user')
            }

            onClicked: activate()
        }
        Button {
            id: goBackButton
            function activate() {
                userSettingsController.goBack();
            }

            anchors.horizontalCenter: passwordRepeatField.horizontalCenter
            text: qsTr('Go back')

            onClicked: activate()
        }
    }
    Connections {
        function onDeleteUserFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }
        function onSetUserData(email: string, nickname: string) {
            nicknameField.contentItem.text = nickname;
            emailField.contentItem.text = email;
        }
        function onUpdateUserFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: userSettingsController
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
            verticalAlignment: Text.AlignVCenter
        }
    }
}
