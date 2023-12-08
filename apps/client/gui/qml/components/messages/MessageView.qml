import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messagesView
    color: Settings.backgroundColor

    property var messages: []

    function setTextPlaceholder(text: string) {
        messageInput.placeholderText = text;
    }

    function sendMessage() {
        console.log("send message: " + messageInput.text);
        messages.push({messageText: messageInput.text});
        messageInput.text = "";
        messagesRepeater.model = messages;
    }

    ScrollView {
        anchors.top: parent.top
        anchors.bottom: messageInput.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        width: parent.width
        spacing: 5
        ListView {
            id: messagesRepeater
            model: messages
            width: parent.width
            spacing: 5
            delegate: Text {
                color: "white"
                text: modelData.messageText
            }

        }
    }

    Rectangle {
        color: "#383940"
        radius: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 40
        height: messageInput.height
        TextArea {
            id: messageInput
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            font.pointSize: 12
            width: parent.width - 2 * anchors.leftMargin
            placeholderText: qsTr('Message ...')
            Keys.onReturnPressed: (event) => {
                if (event.modifiers & Qt.ShiftModifier) {
                    text += "\n"
                    messageInput.cursorPosition = messageInput.length
                } else {
                    sendMessage()
                }
            }
            background: Rectangle {
                visible: false
            }
        }
    }
}
