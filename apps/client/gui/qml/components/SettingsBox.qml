import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    color: "#232428"
    height: settingsButton.height + 16
    width: parent.width

    Text {
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
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
            color: "white"
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
