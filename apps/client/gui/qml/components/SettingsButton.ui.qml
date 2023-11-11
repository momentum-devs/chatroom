import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Button {
    height: 28
    icon.color: Settings.settingsButtonColor
    icon.height: height
    icon.source: "../../../resources/svg/gear.svg"
    icon.width: width
    width: 28
}
