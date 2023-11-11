import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/common/settings.js" as Settings

Button {
    background: Rectangle {
        opacity: 0
    }
    contentItem: Column {
        Rectangle {
            id: frame
            border.color: "black"
            border.width: 2
            color: "transparent"
            height: width
            radius: width / 2
            width: parent.width

            Image {
                id: cross
                anchors.centerIn: parent
                height: width
                source: "../../../../resources/svg/close.svg"
                width: parent.width * 0.4
            }
        }
        Text {
            id: escText
            anchors.horizontalCenter: frame.horizontalCenter
            text: "ESC"
        }
    }
}