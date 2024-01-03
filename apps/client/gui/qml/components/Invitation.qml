import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Item {
    id: invitation

    property alias invitationName: nameText.text

        signal
    accepted
        signal
    rejected

    height: nameRectangle.height + reject.background.height / 2

    Rectangle {
        id: nameRectangle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Settings.boxColor
        height: 35
        radius: 5
        width: parent.width - accept.width

        Text {
            id: nameText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: Settings.textColor
        }
    }
    CustomButton {
        id: accept
        anchors.horizontalCenter: nameRectangle.right
        anchors.verticalCenter: nameRectangle.bottom
        hoverColor: Settings.acceptColor
        icon.color: color
        icon.height: size - 5
        icon.source: "../../../resources/svg/check.svg"
        icon.width: size - 5
        size: 18

        background: Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: accept.baseColor
            height: accept.size
            opacity: 0.5
            radius: accept.size / 5
            visible: false
            width: accept.size
        }

        onClicked: {
            invitation.accepted();
        }
        onMouseEnteredArea: {
            background.visible = true;
        }
        onMouseExitedArea: {
            background.visible = false;
        }
    }
    CustomButton {
        id: reject
        anchors.horizontalCenter: nameRectangle.left
        anchors.verticalCenter: nameRectangle.bottom
        hoverColor: Settings.rejectColor
        icon.color: color
        icon.height: accept.icon.height
        icon.source: "../../../resources/svg/cross.svg"
        icon.width: accept.icon.width
        size: accept.size

        background: Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: reject.baseColor
            height: reject.size
            opacity: 0.5
            radius: reject.size / 5
            visible: false
            width: reject.size
        }

        onClicked: {
            invitation.rejected();
        }
        onMouseEnteredArea: {
            background.visible = true;
        }
        onMouseExitedArea: {
            background.visible = false;
        }
    }
}
