import QtQuick 6.2
import QtQuick.Controls 6.2

Window {
    Loader {
        id: loader
        anchors.fill: parent
    }
    Connections {
        function onLoadView(qUrl: url) {
            loader.setSource(qUrl);
        }

        target: loaderController
    }
}
