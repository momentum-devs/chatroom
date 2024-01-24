import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messagesView
    color: Settings.backgroundColor
    property bool requestedMoreMessages: false
    property int previousMessagesCount: 0
    property var previousScrollBarPosition: 0

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
            onContentYChanged: {
                if (messagesScrollView.ScrollBar.vertical.position > 0.01) {
                    messagesView.previousScrollBarPosition = messagesScrollView.ScrollBar.vertical.position;
                }
                if (messagesScrollView.ScrollBar.vertical.position > 0.4) {
                    messagesView.requestedMoreMessages = false;
                }
                if (messagesScrollView.contentHeight > messagesScrollView.height
                    && messagesScrollView.ScrollBar.vertical.position < 0.3
                    && messagesView.requestedMoreMessages === false) {
                    messagesView.requestedMoreMessages = true;
                    messagesController.requestMoreMessages();
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true
            propagateComposedEvents: true
            onClicked: {
                console.log("Clicked on messages view")
                for (var i = 0; i < messagesRepeater.rowCount; i++) {
                    console.log(messagesRepeater.get(i).isSelected);
                }
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
                    messageInput.cursorPosition = messageInput.length;
                } else {
                    sendMessage();
                }
            }
            background: Rectangle {
                visible: false
            }
        }
    }

    Connections {
        id: connections

        target: messagesController

        function onScrollDown() {
            messagesScrollView.ScrollBar.vertical.position = 1 - messagesScrollView.ScrollBar.vertical.size;
        }

        function onMessagesChanged() {
            if (messagesView.previousMessagesCount !== 0 && messagesController.messages.length !== messagesView.previousMessagesCount) {
                let maxScrollPosition = 1 - messagesScrollView.ScrollBar.vertical.size;

                let newScrollPosition = (1 - messagesView.previousMessagesCount / messagesController.messages.length)
                    + messagesView.previousScrollBarPosition * messagesView.previousMessagesCount / messagesController.messages.length;
                messagesScrollView.ScrollBar.vertical.position = newScrollPosition * maxScrollPosition;
            }
            messagesView.previousMessagesCount = messagesController.messages.length;

            if (messagesScrollView.ScrollBar.vertical.position > 0.4) {
                messagesView.requestedMoreMessages = false;
            }
        }
    }
}
