import QtQuick 2.0

Rectangle {
    id: main_rectangle;
    anchors.fill: parent

    Loader {
        id: myLoader;
        anchors.fill: parent;
        visible: true;
        source: "loginScreen.qml";
        onVisibleChanged:      { loadIfNotLoaded(); }
        Component.onCompleted: { loadIfNotLoaded(); }

        function loadIfNotLoaded () {
            // to load the file at first show
            if (visible && !active) {
                active = true;
            }
        }
    }

    Connections {
        id: screenLoaderConnections
        target: myLoader.item
        onSignalScreenChange: changeScreen(screenPath)

        function changeScreen(screenPath) {
            myLoader.source = screenPath
        }
    }

}
