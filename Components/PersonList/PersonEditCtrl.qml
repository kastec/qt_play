import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0

Item {
	signal updateEditPerson(string mess)
	property Person person

	Column {

		spacing: 2

		Text {
			text: qsTr("CONTROL. Edit person: " + person.name + " " + person.age)
		}

		Row {
			Text {
				anchors.verticalCenter: parent.verticalCenter
				text: "Is enabled"
			}
			Switch {
				anchors.verticalCenter: parent.verticalCenter
				id: switchEnable
				checked: person.enabled
			}
		}

		Button {
			text: "make sig"
			onClicked: {
				person.enabled = switchEnable.checked
				console.log("editor click child button")
				updateEditPerson("hello from editor")
			}
		}
	}
}
