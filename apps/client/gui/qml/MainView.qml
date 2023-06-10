import QtQuick 6.4
import QtQuick.Controls 6.4

Item {
    height: 480
    width: 720

    Loader {
        id: loader
        anchors.fill: parent
    }
    Connections {
        function onLoadView(qUrl: url) {
            loader.source = qUrl;
        }

        target: loaderController
    }
}
