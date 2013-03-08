import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras
import com.noughmad.toutatis 1.0

Item {
    id: main
    property int minimumWidth: 500
    property int minimumHeight: 200
    property QtObject selectedProject: null
    
    Toutatis {
        id: daemon
    }
    
    PlasmaCore.DataSource {
        id: dataSource
        engine: "com.noughmad.toutatis"
        connectedSources: ["Toutatis"]
        
        onNewData: {
            console.log("New data on source " + sourceName)
            if (sourceName == "Toutatis") {
                projectView.model = data.projects
            }
        }
    }
    
    Row {
        spacing: 5
        anchors.fill: parent
        
        Column {
            width: parent.width / 3
            height: parent.height
            spacing: 5
            
            PlasmaExtras.Title {
                anchors.horizontalCenter: parent.horizontalCenter
                text: i18n("Projects")
            }
            
            HorizontalLine {
                width: parent.width
            }

            ListView {
                id: projectView
                width: parent.width
                height: 30 * model.length

                model: daemon.projects

                delegate: PlasmaComponents.ListItem {
                    width: parent.width
                    height: 30

                    PlasmaComponents.Label {
                        anchors.fill: parent
                        text: modelData.name
                        elide: Text.ElideRight
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        
                        onClicked: {
                            projectView.currentIndex = index
                            selectedProject = model.modelData
                        }
                    }
                }

                highlight: PlasmaComponents.Highlight {
                    pressed: true
                }
            }
            
            HorizontalLine {
                width: parent.width
            }
            
            Row {
                width: parent.width
                height: 30
                PlasmaComponents.TextField {
                    anchors.fill: parent
                    clearButtonShown: true
                    
                    onAccepted: {
                        daemon.createProject(text)
                        text = ""
                    }
                }
            }
        }
        
        VerticalLine {
            id: divider
            height: parent.height
        }
        
        ListView {
            id: taskView
            height: parent.height
            width: parent.width - projectView.width - divider.width - 2 * parent.spacing
            
            model: selectedProject.tasks
            
            delegate: PlasmaComponents.ListItem {
                
                TaskItem {
                    task: model.modelData
                }
            }
        }
    }
}