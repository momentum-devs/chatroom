import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

CustomButton {
    id: button
    height: size
    icon.color: color
    icon.height: size
    icon.source: "../../../resources/svg/gear.svg"
    icon.width: size
    width: size

    background: Rectangle {
        color: button.color
        height: button.size
        opacity: 0.5
        radius: button.size / 5
        visible: false
        width: button.size
    }

    onMouseEnteredArea: {
        background.visible = true;
    }
    onMouseExitedArea: {
        background.visible = false;
    }
}
