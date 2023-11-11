import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Rectangle {
    function setUserName(nickname: string) {
        userNickname.text = nickname;
    }

    anchors.bottom: parent.bottom
    anchors.left: parent.left
    color: "#232428"
    height: settingsButton.height + 16
    width: parent.width

    Text {
        id: userNickname
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        color: Settings.textColor
        font.pointSize: 11
        text: "user nickname"
    }
    Button {
        id: settingsButton
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        height: 28
        width: 28

        contentItem: Text {
            color: Settings.textColor
            font.pointSize: 17
            horizontalAlignment: Text.AlignHCenter
            text: '⛭'
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            leftColumnController.goToUserSettings();
        }
    }
}
