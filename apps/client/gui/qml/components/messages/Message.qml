import QtQuick 6.4
import QtQuick.Controls 6.4
import "../../common/settings.js" as Settings
import ".."

Rectangle {
    id: message

    height: messageColumn.height + 10

    property var messageData

    property string baseColor: Settings.backgroundColor
    color: baseColor
    property string hoverColor: "#2e2f35"

    Column {
        id: messageColumn
        anchors {
            leftMargin: 20
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        width: parent.width - 2 * anchors.leftMargin
        spacing: 3
        Item {
            width: parent.width
            height: date.height + 5
            visible: messageData.showSeparator
            Separator {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: parent.width
                Rectangle {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    width: date.width
                    height: date.height
                    color: Settings.backgroundColor
                    Text {
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        id: date
                        color: Settings.textColor
                        text: messageData.sendTime.toLocaleDateString(Qt.locale(), "MMMM d, yyyy")
                    }
                }
            }
        }

        Row {
            spacing: 10
            visible: messageData.showNameAndDate
            Text {
                id: userName
                color: Settings.textColor
                text: messageData.senderName
                font.bold: true
            }
            Item {
                height: parent.height
                width: 3 * userName.width
                Text {
                    anchors {
                        bottom: parent.bottom
                        left: parent.left
                    }
                    id: time
                    color: Settings.textColor
                    text: messageData.sendTime.toLocaleString("M/d/yy h:mm a")
                    font.pixelSize: messageText.font.pixelSize * 4 / 5
                }
            }
        }
        Text {
            id: messageText
            color: Settings.textColor
            text: messageData.messageText
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        propagateComposedEvents: true

        onEntered: {
            message.color = message.hoverColor
        }
        onExited: {
            message.color = message.baseColor
        }
    }
}