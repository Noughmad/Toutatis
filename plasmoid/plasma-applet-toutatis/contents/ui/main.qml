import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import com.noughmad.toutatis 1.0

Item {
    PlasmaCore.DataSource {
        id: dataSource
        engine: "com.noughmad.toutatis"
        connectedSources: ["Projects"]
        interval: 500
        
        onNewData: {
            if (sourceName== "Projects") {
                projects.model = data.projects
                console.log(data.projects)
            }
        }
    }
    
    Row {
        spacing: 10
        anchors.fill: parent

        ListView {
            id: projects
            width: parent.width / 3
            height: parent.height
            
            delegate: PlasmaComponents.ListItem {
                width: parent.width
                height: 30
                
                Text {
                    anchors.fill: parent
                    text: modelData
                    
                    Component.onCompleted: {
                        console.log(modelData)
                        console.log(modelData.name)
                    }
                }
            }
        }
        
        ListView {
            id: tasks
            width: parent.width * 2 / 3
            height: parent.height
            
            delegate: PlasmaComponents.ListItem {
                Text {
                    anchors.fill: parent
                    text: modelData
                }
            }
        }
    }
}