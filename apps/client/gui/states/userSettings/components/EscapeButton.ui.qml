import QtQuick 6.4
import Qt5Compat.GraphicalEffects
import "../../../qml/components"
import "../../../qml/common/settings.js" as Settings

CustomButton {
    id: button
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
}