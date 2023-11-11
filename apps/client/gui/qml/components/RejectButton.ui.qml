import QtQuick 6.4
import QtQuick.Controls 6.4
import "../common/settings.js" as Settings

Button {
    height: 25
    width: 25

    contentItem: Text {
        color: Settings.rejectColor
        text: qsTr('✕')
    }
}
