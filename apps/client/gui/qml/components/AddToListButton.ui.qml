import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

RoundButton {
    height: 35
    icon.color: Settings.addColor
    icon.height: height
    icon.source: "../../../resources/svg/plus.svg"
    icon.width: width
    width: 35
}
