import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras
import com.noughmad.toutatis 1.0

Item {
    id: main
    property int minimumWidth: 500
    property int minimumHeight: 200

    property QtObject selectedProject: projectView.model[projectView.currentIndex]
    property QtObject daemon: dataSource.data.Toutatis.daemon
    
    PlasmaCore.DataSource {
        id: dataSource
        engine: "com.noughmad.toutatis"
        connectedSources: ["Toutatis"]
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

                model: dataSource.data.Toutatis.projects

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
                        }
                        
                        PlasmaComponents.Button {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            width: height
                            
                            visible: (projectView.currentIndex == index) || projectMouseArea.containsMouse
                            iconSource: "list-remove"
                            onClicked: {
                                modelData.remove()
                            }
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
                        main.daemon.createProject(text)
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
                height: model.length * 35

                model: dataSource.data.Toutatis[selectedProject.id + "/tasks"]
                delegate: PlasmaComponents.ListItem {
                    width: parent.width
                    height: 35
                    
                    TaskItem {
                        task: modelData
                        anchors.fill: parent
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