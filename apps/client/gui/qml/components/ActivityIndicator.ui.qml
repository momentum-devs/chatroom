import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Item {
    property bool isActive: false

    height: active.height
    width: active.width

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: Settings.backgroundColor
        height: 15
        radius: 15
        width: 15
    }
    Image {
        id: active
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: 12
        source: "../../../resources/svg/online.svg"
        sourceSize.height: height
        sourceSize.width: width
        visible: isActive
        width: 12
    }
    Image {
        id: inactive
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: active.height
        source: "../../../resources/svg/offline.svg"
        sourceSize.height: height
        sourceSize.width: width
        visible: !isActive
        width: active.width
    }
}
