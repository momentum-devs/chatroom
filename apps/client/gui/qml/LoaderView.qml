import QtQuick 6.2
import QtQuick.Controls 6.2

Item {
    property int minHeight: 480
    property int minWidth: 720

    height: 480
    width: 720

    Loader {
        id: loader
        anchors.fill: parent

        onLoaded: {
            loaderController.loaded();
        }
    }
    Connections {
        function onLoadView(qUrl: url) {
            loader.setSource(qUrl);
        }

        target: loaderController
    }
}
