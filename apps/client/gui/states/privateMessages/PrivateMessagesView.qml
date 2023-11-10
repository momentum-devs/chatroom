import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/components"

Rectangle {
    property var channelInvitations: []
    property var channels: []
    property var friendRequests: []
    property var friends: []

    color: "#313338"
    focus: true

    Row {
        anchors.fill: parent

        LeftColumn {
        }
    }
}
