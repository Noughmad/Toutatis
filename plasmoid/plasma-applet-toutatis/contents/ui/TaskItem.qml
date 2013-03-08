import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    property QtObject task
    
    PlasmaComponents.Label {
        anchors.fill: parent
        text: task.name
        elide: Text.ElideRight
    }
    
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: {
            console.log("Task clicked: " + task.id)
        }
    }
}
