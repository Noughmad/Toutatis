import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: top
    property QtObject task
    height: 30
    
    function formatDuration(duration) {
        if (duration > 10 * 3600 * 1000)
            text = i18np("%1 hour", "%1 hours", Math.floor(duration / 3600 / 1000), Math.floor(duration / 3600 / 1000))
        else if (duration > 1 * 3600 * 1000)
            text = i18n("%1 h, %2 min", Math.floor(duration / 3600 / 1000), Math.floor((duration / 60 / 1000) % 60))
        else if (duration > 60 * 1000)
            text = i18n("%1 min %2 s", Math.floor(duration / 60 / 1000), Math.floor((duration / 1000) % 60))
        else
            text = i18np("%1 second", "%1 seconds", Math.floor(duration / 1000), Math.floor(duration / 1000))
        return text
    }

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
            text: formatDuration(task.duration)
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
