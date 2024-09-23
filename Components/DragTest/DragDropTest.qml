import QtQuick 2.15
import QtQuick.Controls
import AppQtTest12 1.0

Item {
    anchors.fill: parent

	BookList {
        id: dataModel
    }

    Rectangle {
        width: 420
        height: 420
        color: "#000000"

        Component {
            id: dndDelegate
		
            Item {
                id: wrapper
                width: dndGrid.cellWidth
                height: dndGrid.cellHeight
                Rectangle {
                    id: itemImage
					color: model.index<=4? "white":model.color
                    anchors.centerIn: parent
                    width: 90
                    height: 90
                    //					smooth: true
					
					Rectangle {
						id: imageBorder
						anchors.fill: parent
						radius: 5
						color: "transparent"
						border.color: "#ffffff"
						border.width: 6
						opacity: 0
					}
					
					state: "inactive"
					states: [
						State {
							name: "greyedOut"
							when: (dndGrid.draggedItemIndex != -1) && (dndGrid.draggedItemIndex != index)
							PropertyChanges { target: itemImage; opacity: 0.8}
						},
						State {
							name: "inactive"
							when: (dndGrid.draggedItemIndex == -1) || (dndGrid.draggedItemIndex == index)
							PropertyChanges { target: itemImage; opacity: 1.0}
						}
					]
					
					Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
					Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
					Behavior on opacity {NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
                }			
				
				Rectangle {
					id: circlePlaceholder
					width: 30; height: 30; radius: 30
					smooth: true
					anchors.centerIn: parent
					color: "#cecece"
					opacity: 0
					Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
				}
				
				
				states: [
					State {
						name: "inDrag"
						when: index === dndGrid.draggedItemIndex
						PropertyChanges { target: circlePlaceholder; opacity: 1 }
						PropertyChanges { target: imageBorder; opacity: 1 }
						PropertyChanges { target: itemImage; parent: dndContainer }
						PropertyChanges { target: itemImage; width: 80 }
						PropertyChanges { target: itemImage; height: 80 }
						PropertyChanges { target: itemImage; anchors.centerIn: undefined }
						PropertyChanges { target: itemImage; x: coords.mouseX - itemImage.width / 2 }
						PropertyChanges { target: itemImage; y: coords.mouseY - itemImage.height / 2 }
				
					}
				]
				transitions: [
					Transition {
						from: "inDrag"
						to: "*"
						PropertyAnimation {
							target: itemImage
							properties: "scale, opacity"
							easing.overshoot: 1.5
							easing.type: "OutBack"
							from: 0.0
							to: 1.0
							duration: 750
						}
					}
				]
				
            }
	
        }


        GridView {
            id: dndGrid
            anchors.fill: parent
            anchors.margins: 10
            cellWidth: 100
            cellHeight: 100
//            model: dndModel
			model: dataModel.data
            delegate: dndDelegate
			
			property int draggedItemIndex: -1
			interactive: true
			
			Item {
				id: dndContainer
				anchors.fill: parent
			}
			
			property int possibleDropIndex: -1
			property int xCoordinateInPossibleDrop: -1
			property int yCoordinateInPossibleDrop: -1
			
			Item {
				id: dropPosIndicator
				visible: false
				height: dndGrid.cellHeight
				width: 10
				
				Rectangle{
					visible: parent.visible
					anchors.centerIn: parent
					height: parent.height+10
					color:"red"
					width: 5				
				}
			
				states: [
					State {
						name: "shown"
						when: dndGrid.draggedItemIndex!=-1 && dndGrid.possibleDropIndex != -1 && dndGrid.draggedItemIndex!=dndGrid.possibleDropIndex
						PropertyChanges {
							target: dropPosIndicator
							visible: true
							x:  Math.floor(dndGrid.xCoordinateInPossibleDrop/dndGrid.cellWidth) *
								dndGrid.cellWidth - 5
							y:  Math.floor((dndGrid.yCoordinateInPossibleDrop + dndGrid.contentY)/dndGrid.cellHeight) *
								dndGrid.cellHeight - dndGrid.contentY 
						}
					}
				]
			} //dropPosIndicator
			
		
			
			MouseArea {
//				enabled: false
				id: coords
				anchors.fill: parent
				onReleased: {
					if (dndGrid.draggedItemIndex != -1) {
						var draggedIndex = dndGrid.draggedItemIndex				
						dndGrid.draggedItemIndex = -1
						dndGrid.possibleDropIndex = -1
						var contentY = dndGrid.contentY
						var moveToIndex = dndGrid.indexAt(mouseX, mouseY + contentY)
						console.log("MOVE: "+draggedIndex+"->"+moveToIndex + " contY:" + contentY)
						
//						dndModel.move(draggedIndex, moveToIndex,1)	
						
						dataModel.moveItemTest(draggedIndex, moveToIndex)
						dndGrid.contentY = contentY;
						dndGrid.interactive=true
					}
				}
				
				onPressed: {					
//					dndGrid.draggedItemIndex = dndGrid.indexAt(mouseX, mouseY)
				}
				onPressAndHold: {
					dndGrid.draggedItemIndex = dndGrid.indexAt(mouseX,dndGrid.contentY + mouseY)
				}

				onPositionChanged: {			
					if(dndGrid.draggedItemIndex>-1 && dndGrid.interactive==true){
						dndGrid.interactive=false;
					}
						
					var newPos = dndGrid.indexAt(mouseX, dndGrid.contentY + mouseY)
					console.log("inx:"+newPos)
					if(newPos<=4)
					{ // IsDone items
						dndGrid.possibleDropIndex = -1
						return
					}
					
					if (newPos !== dndGrid.possibleDropIndex) {
						dndGrid.possibleDropIndex = newPos
						dndGrid.xCoordinateInPossibleDrop = mouseX
						dndGrid.yCoordinateInPossibleDrop = mouseY
//						console.log("possibleDropIndex: "+ dndGrid.possibleDropIndex)
//						console.log("dropPosIndicator: "+ dropPosIndicator.state)
						
					}
				}
			}
        }  // grid vew 
		
		
		
    } // Rectangle
	
	Button{
		text:"MV TST"
		onClicked: dataModel.moveItemTest(2,4)
	}	
	
} // root item
