import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    property var chatId: ""

    function setChat(chat) {
        chatName.text = '<b>' + chat[0] + '</b>';
        chatId = chat[1];
    }

    Column {
        id: chat
        height: parent.height
        spacing: 5
        width: parent.width

        Text {
            id: chatName
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Row {
            spacing: 5

            Button {
                anchors.horizontalCenter: parent.horizontalRight
                text: qsTr('Add to chat')

                onClicked: {
                    mainController.addToChat();
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: "#FF0000"
                    text: qsTr('Left the chat')
                }

                onClicked: {
                    mainController.leftTheChat();
                }
            }
        }
    }
}

