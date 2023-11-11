import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Item {
    property var isActive: false

    height: active.height
    width: active.width

    Text {
        id: active
        anchors.verticalCenter: parent.verticalCenter
        color: Settings.activeColor
        font.pointSize: 12
        text: "●"
        visible: isActive
    }
    Text {
        id: inactive
        anchors.verticalCenter: parent.verticalCenter
        color: Settings.inactiveColor
        font.pointSize: 12
        text: "○"
        visible: !isActive
    }
}
