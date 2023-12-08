import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messages
    color: Settings.backgroundColor

    function setTextPlaceholder(text: string) {
        messageInput.placeholderText = text;
    }

    Rectangle {
        color: "#383940"
        radius: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        height: 35
        TextField {
            id: messageInput
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            width: parent.width - 2 * anchors.leftMargin
            height: parent.height
            focus: true
            placeholderText: qsTr('Message ...')
            background: Rectangle {
                visible: false
            }
        }
    }
}
