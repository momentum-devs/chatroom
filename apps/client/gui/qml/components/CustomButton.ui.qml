import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Button {
    id: button

    property var baseColor: "#8e9297"
    property var color: baseColor
    property var hoverColor: "#d8d9da"
    property var size: 45

    signal mouseEnteredArea
    signal mouseExitedArea

    hoverEnabled: true

    background: Rectangle {
        opacity: 0
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        propagateComposedEvents: true

        onEntered: {
            button.color = button.hoverColor;
            button.mouseEnteredArea();
        }
        onExited: {
            button.color = button.baseColor;
            button.mouseExitedArea();
        }
        onPressed: mouse.accepted = false
    }
}