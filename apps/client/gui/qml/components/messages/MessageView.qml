import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings

Rectangle {
    id: messages
    color: "#2b2d31"

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        color: "#383940"
        height: 70
        radius: 10
        width: parent - 40
    }
}
