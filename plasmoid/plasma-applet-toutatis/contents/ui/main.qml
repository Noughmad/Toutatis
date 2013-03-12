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
            id: projectColumn
            width: parent.width / 3
            height: parent.height
            spacing: 5
            
            PlasmaComponents.Label {
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
                        id: projectMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        
                        onClicked: {
                            projectView.currentIndex = index
                            selectedProject = modelData
                        }
                        
                        PlasmaComponents.Button {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            width: height
                            
                            visible: (projectView.currentIndex == index) || projectMouseArea.containsMouse
                            iconSource: "list-remove"
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
        
        Column {
            height: parent.height
            width: parent.width - projectColumn.width - divider.width - 2 * parent.spacing
            spacing: 5
            
            PlasmaComponents.Label {
                text: i18n("Tasks")
                anchors.horizontalCenter: parent.horizontalCenter
            }
            
            ListView {
                id: taskView
                width: parent.width
                height: model.length * 40

                model: dataSource.data.Toutatis[selectedProject.id + "/tasks"]
                delegate: PlasmaComponents.ListItem {
                    
                    TaskItem {
                        task: modelData
                    }
                }
            }
            
            HorizontalLine {
                width: parent.width
            }
            
            Row {
                width: parent.width - 105
                height: 30
                spacing: 0
                PlasmaComponents.TextField {
                    id: taskNameEdit
                    anchors.fill: parent
                    clearButtonShown: true
                    
                    onAccepted: {
                        if (selectedProject && text) {
                            selectedProject.createTask(text)
                            text = ""
                        }
                    }
                }
                PlasmaComponents.Button {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 100
                    height: taskNameEdit.height

                    text: i18n("Add task")
                    onClicked: {
                        taskNameEdit.accepted()
                    }
                }
            }
        }
    }
}