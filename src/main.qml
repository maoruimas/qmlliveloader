import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: window
    visible: true
    flags: Qt.Window | Qt.WindowSystemMenuHint
           | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint | Qt.WindowCloseButtonHint
           | Qt.WindowStaysOnTopHint
    width: 640
    height: 480
    title: qsTr("Qml Live Loader")

    property bool firstLoad: true
    property int loadItemX
    property int loadItemY

    function load(full) {
        if (loader.status === Loader.Loading) {
            console.log("<font color='red'>Fail to load as loader is busy</font>")
            return
        }

        if (full) {
            consoleArea.clear()
            $FileWatcher.setPath(pathInput.text)
        }
        console.log("<font color='blue'>Loading...</font>")
        if (loader.status === Loader.Ready) {
            firstLoad = false
            loadItemX = loader.item.x
            loadItemY = loader.item.y
        }
        loader.source = ""
        $QmlEngine.clearCache()
        if (firstLoad) {
            loader.setSource(pathInput.text, {"flags": flags})
        } else {
            loader.setSource(pathInput.text, {"flags": flags, "x": loadItemX, "y": loadItemY})
        }
    }

    Connections {
        target: $FileWatcher
        onUpdate: {
            console.log("<font color='purple'>File changes captured</font>")
            load(false)
        }
        onMessage: {
            console.log("<font color='gray'>" + msg + "</font>")
        }
    }

    Loader {
        id: loader
        asynchronous: true
        onLoaded: console.log("<font color='green'>Loaded</font>")
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        RowLayout {
            id: topLayout
            Layout.fillWidth: true

            TextField {
                id: pathInput
                Layout.fillWidth: true
                placeholderText: qsTr("Qml file path")
                onAccepted: load(true)
            }
            Button {
                text: qsTr("Load")
                onClicked: load(true)
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                objectName: "consoleArea"
                id: consoleArea
                background: Rectangle {
                    radius: 2
                    border.color: focus ? "#41adff" : "#ababab"
                }
                textFormat: "RichText"
                wrapMode: "Wrap"
                selectByMouse: true
                persistentSelection: true
                readOnly: true
            }
        }
    }

    DropArea {
        anchors.fill: parent
        onDropped: {
            if (drop.hasUrls) {
                pathInput.text = drop.urls[0]
                load(true)
            }
        }
    }
}
