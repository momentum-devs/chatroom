import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messagesView
    color: Settings.backgroundColor

    function setTextPlaceholder(text: string) {
        messageInput.placeholderText = text;
    }

    function sendMessage() {
        messagesController.sendMessage(messageInput.text);
        messageInput.text = "";
    }

    ScrollView {
        id: messagesScrollView
        anchors {
            top: parent.top
            bottom: messageInputRectangle.top
            left: parent.left
            topMargin: 20
            bottomMargin: 20
        }
        clip: true
        width: parent.width
        ListView {
            id: messagesRepeater
            model: messagesController.messages
            width: parent.width
            spacing: 1
            delegate: Message {
                messageData: modelData
                width: messagesView.width
            }
        }
    }

    Rectangle {
        id: messageInputRectangle
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

    Connections {
        target: messagesController

        function onScrollDown() {
            messagesScrollView.ScrollBar.vertical.position = messagesScrollView.contentHeight
        }
    }
}
