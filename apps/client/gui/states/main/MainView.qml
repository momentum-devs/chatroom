import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
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

                onClicked: {
                    mainController.goToCreateChannel();
                    console.log("create channel from qml");
                }
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Logout')

                onClicked: {
                    mainController.logout();
                    console.log("logout from qml");
                }
            }
        }
        Rectangle {
            color: 'black'
            height: parent.height
            width: 2
        }
    }
}
