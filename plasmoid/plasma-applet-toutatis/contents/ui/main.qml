import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
// import com.noughmad.toutatis 1.0

Item {
    id: main
    property QtObject selectedProject: null
    
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
        
        ListView {
            id: projectView
            width: parent.width / 3
            height: parent.height
            
            model: dataSource.data.Toutatis.projects
            
            delegate: PlasmaComponents.ListItem {
                width: parent.width
                height: 30
                
                PlasmaComponents.Label {
                    anchors.fill: parent
                    text: modelData.name
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
        
        PlasmaCore.SvgItem {
            id: divider
            height: parent.height
            width: 3
            
            svg: PlasmaCore.Svg {
                imagePath: "widgets/line"
            }
            elementId: "vertical-line"
        }
        
        ListView {
            id: taskView
            height: parent.height
            width: parent.width - projectView.width - divider.width - 2 * parent.spacing
            
            model: dataSource.data.Toutatis[dataSource.data.Toutatis.projects[projectView.currentIndex].id + "/tasks"]
            
            delegate: PlasmaComponents.ListItem {
                height: 30
                
                TaskItem {
                    anchors.fill: parent
                    task: model.modelData
                }
            }
            
            highlight: PlasmaComponents.Highlight {
                pressed: true
            }
        }
    }
}