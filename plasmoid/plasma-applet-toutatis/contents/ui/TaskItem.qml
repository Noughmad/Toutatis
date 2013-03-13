import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: top
    property QtObject task
    property bool choosingStatus: false
    property variant statusStrings: ["done", "inprogress", "todo", "urgent", "cancelled"]
    
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
    
    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true

        Row {
            id: row
            spacing: 4

            anchors.fill: parent
            
            PlasmaComponents.BusyIndicator {
                id: busy
                height: parent.height * 0.8
                width: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                
                running: top.task.active
                visible: top.task.active
            }
            
            PlasmaComponents.Label {
                height: parent.height
                width: parent.width - 260 - (choosingStatus ? 100 : (task.active ? (busy.width + row.spacing - 1) : 0))
                text: task.name
                elide: Text.ElideRight
            }
            
            VerticalLine {
                height: parent.height
            }
            
            StatusButton {
                visible: !choosingStatus
                height: parent.height
                width: 80
                status: task.status
                
                onClicked: choosingStatus = true
            }
            
            VerticalLine {
                visible: !choosingStatus
                height: parent.height
            }
            
            Item {
                visible: !choosingStatus
                height: parent.height
                width: 80

                PlasmaComponents.Label {
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: formatDuration(task.duration)
                }
            }
            
            TrackButton {
                visible: (task.active || mouse.containsMouse) && !choosingStatus
                height: parent.height
                task: top.task
            }
            
            Repeater {
                id: statusChooser
              
                model: statusStrings
                delegate: StatusButton {
                    visible: choosingStatus
                    height: parent.height
                    status: modelData
                    onClicked: {
                        task.status = status
                        choosingStatus = false
                    }
                }
            }
        }
    }
}
