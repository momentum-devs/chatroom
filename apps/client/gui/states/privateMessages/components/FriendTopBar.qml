import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/common/settings.js" as Settings

Item {
    property var friendId: ""

    function setFriendName(friend) {
        friendNickname.text = '<b>' + friend + '</b>';
        friendNickname.color = Settings.textColor;
        friendNickname.font.pointSize = 18;
        friendId = friend[1];
    }

    Column {
        id: friendTopBar
        height: parent.height
        spacing: 5
        width: parent.width

        Item {
            height: 6
            width: parent.width
        }
        Text {
            id: friendNickname
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Item {
            height: 6
            width: parent.width
        }
        Row {
            spacing: 5

            Button {
                id: deleteChannelButton
                anchors.horizontalCenter: parent.horizontalRight

                contentItem: Text {
                    color: Settings.removeColor
                    text: qsTr('Remove from friends')
                }

                onClicked: {
                    privateMessagesController.removeFromFriends();
                }
            }
        }
        Rectangle {
            color: Settings.separatorColor
            height: 1
            width: parent.width
        }
    }
}

