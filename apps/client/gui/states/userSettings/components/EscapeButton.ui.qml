import QtQuick 6.4
import QtQuick.Controls 6.4
import Qt5Compat.GraphicalEffects
import "../../../qml/common/settings.js" as Settings

Button {
    id: button

    property var baseColor: "#8e9297"
    property var color: baseColor
    property var hoverColor: "#d8d9da"
    property var size: 45

    hoverEnabled: true

    background: Rectangle {
        opacity: 0
    }
    contentItem: Column {
        id: buttonColumn
        spacing: button.size / 5

        Rectangle {
            id: frame
            border.color: button.color
            border.width: 2
            color: "transparent"
            height: button.size
            radius: button.size / 2
            width: button.size

            Image {
                id: cross
                anchors.centerIn: parent
                antialiasing: true
                height: width
                source: "../../../../resources/svg/close.svg"
                visible: false
                width: parent.width * 0.5
            }
            ColorOverlay {
                id: crossOverlay
                anchors.fill: cross
                antialiasing: true
                color: button.color
                source: cross
                transform: rotation
            }
        }
        Text {
            id: escText
            anchors.horizontalCenter: frame.horizontalCenter
            color: button.color
            text: "ESC"
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        propagateComposedEvents: true

        onEntered: {
            button.color = button.hoverColor;
        }
        onExited: {
            button.color = button.baseColor;
        }
        onPressed: mouse.accepted = false
    }
}