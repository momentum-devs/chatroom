import QtQuick 6.2
import QtQuick.Controls 6.2

Window {
    height: 720
    width: 1280

    Loader {
        id: loader
        anchors.fill: parent
        asynchronous: true
    }
    Connections {
        function onLoadView(qUrl: url) {
            loader.setSource(qUrl);
        }

        target: loaderController
    }
}
