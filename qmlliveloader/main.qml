import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: _window
    visible: true
    flags: Qt.Window | Qt.WindowSystemMenuHint
       | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
       | Qt.WindowMaximizeButtonHint | Qt.WindowCloseButtonHint
       | Qt.WindowStaysOnTopHint
    width: 600
    height: 450
    title: qsTr("Qml Live Loader")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        RowLayout {
            Layout.fillWidth: true

            TextField {
                id: _mainQmlInput
                Layout.fillWidth: true
                placeholderText: qsTr("Main qml file path")
                selectByMouse: true
                selectedTextColor: "white"
            }
            Button {
                text: qsTr("Load")
                onClicked: _start()
            }
        }

        ScrollView {
            id: _scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                objectName: "_console"
                id: _console
                background: Rectangle {
                    radius: 2
                    border.color: focus ? "#41adff" : "#ababab"
                }
                textFormat: "RichText"
                wrapMode: "Wrap"
                selectByMouse: true
                selectedTextColor: "white"
                readOnly: true
                onTextChanged: _scrollView.scrollToButtom()

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: _menu.popup()

                    Menu {
                        id: _menu
                        MenuItem {
                            text: qsTr("Clear All")
                            onTriggered: _console.text = ""
                        }
                        MenuItem {
                            text: _reloadOnChanges ? qsTr("Reload manually")
                                                   : qsTr("Reload automatically")
                            onTriggered: _reloadOnChanges = !_reloadOnChanges
                        }
                    }
                }
            }
            function scrollToButtom() {
                ScrollBar.vertical.position = 1 - ScrollBar.vertical.size;
            }
        }
    }

    DropArea {
        anchors.fill: parent
        onDropped: {
            if (drop.hasUrls) {
                _mainQmlInput.text = drop.urls[0].replace("file://", "");
                _start();
            }
        }
    }

    Loader {
        id: _loader
        asynchronous: true
    }

    property bool _reloadOnChanges: true
    property point _geo: Qt.point(0, 0)

    function _printLoadingMessage(msg) {
        var str = `${(new Date()).toTimeString().split(' ')[0]}: ${msg}`;
        console.log(`<b style='color: blue;'>${str}</b>`);
    }

    function _start() {
        $QmlLiveLoader.watch(_mainQmlInput.text);

        _console.text = "";
        _printLoadingMessage("Starting ...");

        _loader.source = "";
        $QmlLiveLoader.clearCache();
        _loader.setSource(`file://${_mainQmlInput.text}`, {"flags": flags});
    }

    function _reload() {
        _printLoadingMessage("Reloading ...");

        var	s = _loader.source;
        if (_loader.status == Loader.Ready) {
            _geo.x = _loader.item.x;
            _geo.y = _loader.item.y;
        }

        _loader.source = "";
        $QmlLiveLoader.clearCache();
        _loader.setSource(s, {"flags": flags, "x": _geo.x, "y": _geo.y});
    }
}
