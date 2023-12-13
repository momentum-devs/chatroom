import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../../qml/components"
import "../../../qml/common/settings.js" as Settings

Rectangle {
    id: rightRectangle

    property var members: []

    function addMember(memberName: string, memberId: string, isActive: bool) {
        members.push([memberName, memberId, isActive]);
        membersView.model = members;
        membersView.height = 36 * members.length;
    }

    function clearMembersList() {
        members = [];
        membersView.model = members;
    }

    color: Settings.backgroundColor

    Column {
        id: rightColumn
        anchors.left: parent.left
        anchors.leftMargin: 7
        height: parent.height
        spacing: 5
        width: parent.width - 14

        Item {
            height: 6
            width: parent.width
        }
        Text {
            id: membersText
            anchors.horizontalCenter: parent.horizontalCenter
            color: Settings.textColor
            text: 'Members: '
        }
        ScrollView {
            anchors.horizontalCenter: parent.horizontalCenter
            clip: true
            width: parent.width

            ListView {
                id: membersView
                contentWidth: parent.width
                height: 0
                spacing: 5
                width: parent.width

                delegate: Item {
                    height: memberButton.height + memberActive.height * 0.5
                    width: parent.width

                    Button {
                        id: memberButton
                        text: modelData[0]
                        width: parent.width - memberActive.width - 5

                        onClicked: {
                            channelController.goToPrivateMessage(modelData[1], modelData[0]);
                        }
                    }
                    ActivityIndicator {
                        id: memberActive
                        anchors.horizontalCenter: memberButton.right
                        anchors.verticalCenter: memberButton.bottom
                        isActive: modelData[2]
                    }
                }
            }
        }
    }
}
