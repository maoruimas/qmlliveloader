import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: _window
    objectName: "_window"
    visible: true
    flags: Qt.Window | Qt.WindowSystemMenuHint
           | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint | Qt.WindowCloseButtonHint
           | Qt.WindowStaysOnTopHint
    width: 320
    height: $integratedConsole ? 480 : 120
    title: qsTr("Qml Live Engine")

    ColumnLayout {
        id: _layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        CheckBox {
            id: _checkBox
            objectName: "_checkBox"
            Layout.fillWidth: true
            checked: true
            text: qsTr("Reload automatically")
        }
        Button {
            Layout.fillWidth: true
            text: qsTr("Reload")
            onClicked: _reload()
        }
        ScrollView {
            id: _scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextArea {
                id: _console
                visible: $integratedConsole
                objectName: "_console"
                textFormat: "RichText"
                selectByMouse: true
                readOnly: true
                wrapMode: "Wrap"
                selectedTextColor: "white"
                onTextChanged: _scrollView.scrollToBottom()
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
                    }
                }
            }
            function scrollToBottom() {
                ScrollBar.vertical.position = 1 - ScrollBar.vertical.size;
            }
        }
    }

    Loader {
        id: _loader
        asynchronous: true
    }

    property point _geo: Qt.point(0, 0)

    function _printLoadingMessage(msg) {
        var str = `${(new Date()).toTimeString().split(' ')[0]}: ${msg}`;
        if ($integratedConsole) {
            console.log(`<b style='color: blue;'>${str}</b>`);
        } else {
            console.log(str);
        }
    }

    function _start() {
        _printLoadingMessage("Starting ...");
        _loader.setSource($QmlLiveEngine.getMainQml(), {"flags": flags});
    }

    function _reload() {
        _printLoadingMessage("Reloading ...");

        var	s = _loader.source;
        if (_loader.status == Loader.Ready) {
            _geo.x = _loader.item.x;
            _geo.y = _loader.item.y;
        }

        _loader.source = "";
        $QmlLiveEngine.clearCache();
        _loader.setSource(s, {"flags": flags, "x": _geo.x, "y": _geo.y});
    }
}
