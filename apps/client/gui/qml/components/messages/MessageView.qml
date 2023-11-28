import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messages
    color: Settings.backgroundColor

    Rectangle {
        id: messageInput
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter : parent.horizontalCenter
        color: "#383940"
        height: 35
        radius: 10
        width: parent.width - 40
    }
}
