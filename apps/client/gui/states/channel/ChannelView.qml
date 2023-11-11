import QtQuick 6.2
import QtQuick.Controls 6.2
import "../../qml/components"
import "../../qml/common/settings.js" as Settings

Rectangle {
    color: Settings.backgroundColor
    focus: true

    Row {
        anchors.fill: parent

        LeftColumn {
            id: leftColumn
        }
        Column {
            height: parent.height
            width: parent.width - leftColumn.width - friendsColumn.width

            Item {
                id: defaultView
                height: parent.height
                visible: true
                width: parent.width
            }
        }
        Rectangle {
            color: Settings.separatorColor
            height: parent.height
            width: 1
        }
    }
    Connections {
    }
}
