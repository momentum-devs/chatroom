import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    property var friendId: ""

    function setFriend(friend) {
        channelName.text = '<b>' + friend[0] + '</b>';
        channelName.color = "white";
        channelName.font.pointSize = 18;
        friendId = friend[1];
    }

    Column {
        id: channel
        height: parent.height
        spacing: 5
        width: parent.width

        Item {
            height: 6
            width: parent.width
        }
        Text {
            id: channelName
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
                    color: "#FF0000"
                    text: qsTr('Remove from friends')
                }

                onClicked: {
                    mainController.removeFromFriends();
                }
            }
        }
        Rectangle {
            color: '#3f4147'
            height: 1
            width: parent.width
        }
    }
}

