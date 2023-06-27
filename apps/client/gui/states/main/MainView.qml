import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    anchors.fill: parent
    color: "grey"

    Row {
        anchors.fill: parent
        spacing: 5

        Column {
            id: menuColumn
            height: parent.height
            spacing: 5

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Create channel')
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Logout')
            }
        }
        Rectangle {
            color: 'black'
            width: 2

            anchors {
                height: parent.height
            }
        }
    }
}
