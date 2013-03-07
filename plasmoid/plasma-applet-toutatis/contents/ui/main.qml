import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import com.noughmad.toutatis 1.0

Item {
    id: main
    property QtObject selectedProject: null
    
    Toutatis {
        id: daemon
    }
    
    Component.onCompleted: {
        console.log(daemon.projects)
        projectView.model = daemon.projects
    }
    
    Row {
        spacing: 10
        anchors.fill: parent

        ListView {
            id: projectView
            width: parent.width / 3
            height: parent.height
            
            // model: daemon.projects
            
            delegate: PlasmaComponents.ListItem {
                width: parent.width
                height: 30
                
                PlasmaComponents.Label {
                    anchors.fill: parent
                    text: "Blank"
                    // text: modelData.name
                    // text: model.modelData.name
                }
                
                Component.onCompleted: {
                    // console.log(modelData)
                }
                
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    
                    onClicked: {
                        console.log(modelData)
                        console.log(model.modelData.name)
                        projectView.currentIndex = index
                        selectedProject = model.modelData
                    }
                }
            }

            highlight: PlasmaComponents.Highlight {
                pressed: true
            }
        }
        
        ListView {
            id: taskView
            width: parent.width * 2 / 3
            
            model: selectedProject ? selectedProject.tasks : null
            
            delegate: PlasmaComponents.ListItem {
                width: parent.width
                height: 30
                
                PlasmaComponents.Label {
                    anchors.fill: parent
                    text: model.modelData.name
                }
                
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    
                    onClicked: {
                        console.log("Task clicked: " + model.modelData.id)
                    }
                }
            }
            
            highlight: PlasmaComponents.Highlight {
                pressed: true
            }
        }
    }
}