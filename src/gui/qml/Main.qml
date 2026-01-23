import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 720
    height: 480
    visible: true
    title: qsTr("Hotspot — Modern")

    header: ToolBar {
        contentHeight: 48
        RowLayout {
            anchors.fill: parent
            spacing: 8
            Label { text: qsTr("Hotspot") ; font.bold: true }
            Item { Layout.fillWidth: true }
            Label { text: HotspotClient.status }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.window.background

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 16
            width: parent.width * 0.6

            Frame {
                Layout.fillWidth: true
                ColumnLayout {
                    spacing: 8
                    anchors.margins: 12

                    Label { text: qsTr("Create Hotspot"); font.pixelSize: 20 }

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("SSID:"); width: 80 }
                        TextField { id: ssidField; placeholderText: qsTr("MyHotspot") ; Layout.fillWidth: true }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: qsTr("Password:"); width: 80 }
                        TextField { id: pskField; echoMode: TextInput.Password ; placeholderText: qsTr("8+ characters") ; Layout.fillWidth: true }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 12
                        Button {
                            text: qsTr("Start Hotspot")
                            onClicked: {
                                HotspotClient.startHotspot(ssidField.text || "ExampleSSID", pskField.text || "password123")
                            }
                        }
                        Button {
                            text: qsTr("Stop Hotspot")
                            onClicked: HotspotClient.stopHotspot()
                        }
                    }

                    Label {
                        text: HotspotClient.lastError ? "Error: " + HotspotClient.lastError : ""
                        color: "#b00020"
                        visible: HotspotClient.lastError !== ""
                    }
                }
            }
        }
    }
}
