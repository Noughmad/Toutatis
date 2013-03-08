import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: top
    property QtObject task
    height: 30
    
    Row {
        spacing: 10

        anchors.fill: parent
        
        PlasmaComponents.Label {
            height: parent.height
            text: task.name
            elide: Text.ElideRight
        }
        
        VerticalLine {
            height: parent.height
        }
        
        PlasmaComponents.Label {
            height: parent.height
            text: task.status
            elide: Text.ElideRight
        }
        
        VerticalLine {
            height: parent.height
        }
        
        PlasmaComponents.Label {
            height: parent.height
            text: task.duration
        }
        
        PlasmaComponents.BusyIndicator {
            height: parent.height * 0.8
            width: parent.height * 0.8
            anchors.verticalCenter: parent.verticalCenter

            running: top.task.active
            visible: top.task.active
        }
        
        VerticalLine {
            height: parent.height
        }
        
        TrackButton {
            height: parent.height
            task: top.task
        }
        
    }
}
