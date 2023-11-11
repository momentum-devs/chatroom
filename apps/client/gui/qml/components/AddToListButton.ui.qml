import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

RoundButton {
    height: 35
    width: 35

    contentItem: Text {
        color: Settings.addColor
        font.pointSize: 20
        horizontalAlignment: Text.AlignHCenter
        text: '+'
        verticalAlignment: Text.AlignVCenter
    }
}
