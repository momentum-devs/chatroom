import QtQuick 6.4
import QtQuick.Controls 6.4
import Qt5Compat.GraphicalEffects
import "../../../qml/components"
import "../../../qml/common/settings.js" as Settings

Image {
    id: img

    property bool adapt: true
    property bool rounded: true

    layer.enabled: rounded
    source: "../../../resources/example_avatar.jpg"

    layer.effect: OpacityMask {
        maskSource: Item {
            height: img.height
            width: img.width

            Rectangle {
                anchors.centerIn: parent
                height: img.adapt ? img.height : width
                radius: Math.min(width, height)
                width: img.adapt ? img.width : Math.min(img.width, img.height)
            }
        }
    }
}