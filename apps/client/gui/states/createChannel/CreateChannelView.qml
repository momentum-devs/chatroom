import QtQuick 6.4
import QtQuick.Controls 6.4

Rectangle {
    color: "grey"

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: channelNameField
            placeholderText: qsTr('Channel name')
        }
        Row {
            anchors.horizontalCenter: channelNameField.horizontalCenter

            Button {
                function activate() {
                    const channelName = channelNameField.text;
                    if (channelName.length !== 0) {
                        createChannelController.createChannel(channelName);
                        successPopup.open();
                    } else {
                        errorPopup.contentItem.text = "Channel name field is empty";
                        errorPopup.open();
                    }
                }

                text: qsTr('Create channel')

                Keys.onEnterPressed: activate()
                Keys.onReturnPressed: activate()
                onClicked: activate()
            }
            Button {
                text: qsTr('Go back')

                onClicked: {
                    createChannelController.goBack();
                }
            }
        }
    }
    Connections {
        function onCreateChannelFailure(message: string) {
            errorPopup.contentItem.text = message;
            errorPopup.open();
        }

        target: createChannelController
    }
    Popup {
        id: successPopup
        height: 50
        width: 200
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) * 3 / 4)

        contentItem: Text {
            color: "green"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: qsTr('Send login request')
            verticalAlignment: Text.AlignVCenter
        }
    }
    Popup {
        id: errorPopup
        height: 50
        width: Math.round(parent.width / 2)
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) * 3 / 4)

        contentItem: Text {
            color: "red"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: qsTr('Login error!')
            verticalAlignment: Text.AlignVCenter
        }
    }
}
