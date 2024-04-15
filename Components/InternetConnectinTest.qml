import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0

Item {

    Connections {
        target: InternetChecker

        onConnected: {
            console.log("QML INET CONNECTD")
            tbState.text = "QML INET CONNECTD"
        }
        onDisconnected: {
            console.log("QML INET DISCONNECTD")
            tbState.text = "QML INET DISCONNECTD"
        }
    }

    Column {
        anchors.fill: parent

        Row {
            Text {
                text: "IsOnline: "
            }
            Text {
                text: InternetChecker.isOnline ? "TR" : "FLS"
            }
        }

        Row {
            Text {
                id: tbState
                text: "------------"
            }
            Button {
                text: InternetChecker.allowConnection
                onClicked: {
                    console.log("CLICKED, curr allow:" + InternetChecker.allowConnection)
                    InternetChecker.allowConnection = !InternetChecker.allowConnection
                    console.log("CLICKED - after curr allow:" + InternetChecker.allowConnection)
                }
            }
        }
    }
}
