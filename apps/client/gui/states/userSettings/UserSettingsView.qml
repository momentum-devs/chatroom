import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../qml/common/settings.js" as Settings
import "components"

Rectangle {
    id: userSettingsRectangle
    color: Settings.backgroundColor

    EscapeButton {
        size: 45
        x: parent.width * 0.75
        y: parent.height * 0.1

        onClicked: userSettingsController.goBack()
    }
    Column {
        anchors.centerIn: parent
        spacing: 10
        width: Math.round(parent.width * 0.75)

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: "User Settings"
        }
        Item {
            height: userSettingsRectangle.height * 0.05
            width: 1
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width

            Column {
                height: parent.height
                spacing: 10
                width: Math.round(parent.width / 3)

                Avatar {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: nicknameColumn.height - changeAvatar.height - 10
                    width: nicknameColumn.height - changeAvatar.height - 10
                }
                Button {
                    id: changeAvatar
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr('Change avatar')
                }
            }
            Column {
                id: nicknameColumn
                spacing: 10
                width: Math.round(parent.width / 3)

                TextField {
                    id: emailField
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: true
                    text: "example@email.com"
                    width: Math.round(parent.width * 0.8)
                }
                TextField {
                    id: nicknameField
                    anchors.horizontalCenter: parent.horizontalCenter
                    focus: true
                    placeholderText: qsTr('Nickname')
                    width: Math.round(parent.width * 0.8)
                }
                Button {
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

                    // width: changePassword.width
                    onClicked: activate()
                }
            }
            Column {
                spacing: 10
                width: Math.round(parent.width / 3)

                TextField {
                    id: passwordField
                    anchors.horizontalCenter: parent.horizontalCenter
                    echoMode: TextInput.Password
                    placeholderText: qsTr('Password')
                    width: Math.round(parent.width * 0.8)
                }
                TextField {
                    id: passwordRepeatField
                    anchors.horizontalCenter: parent.horizontalCenter
                    echoMode: TextInput.Password
                    placeholderText: qsTr('Repeat password')
                    width: Math.round(parent.width * 0.8)
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
            }
        }
        Item {
            height: userSettingsRectangle.height * 0.05
            width: 1
        }
        Button {
            id: deleteUser
            function activate() {
                userSettingsController.deleteUser();
            }

            anchors.horizontalCenter: parent.horizontalCenter
            width: changePassword.width

            contentItem: Text {
                color: Settings.deleteColor
                horizontalAlignment: Text.AlignHCenter
                text: qsTr('Delete user')
            }

            onClicked: activate()
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr('Logout')
            width: changePassword.width

            onClicked: {
                userSettingsController.logout();
            }
        }
    }
    Connections {
        function onDeleteUserFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }
        function onSetUserData(email: string, nickname: string) {
            nicknameField.text = nickname;
            emailField.text = email;
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
