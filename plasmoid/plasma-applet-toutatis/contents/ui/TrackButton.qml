import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaComponents.Button {
    id: button
    checkable: true
    width: 70
    
    property QtObject task
    
    checked: task.active
    text: task.active ? "Stop" : "Start"
    iconSource: task.active ? "media-playback-pause" : "player-time"
    
    onClicked: {
        task.active = !task.active
    }
}