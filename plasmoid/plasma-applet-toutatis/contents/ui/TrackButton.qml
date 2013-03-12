import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

PlasmaComponents.Button {
    id: button
    checkable: true
    width: 70
    
    property QtObject task
    property bool tracking: task.active
    
    text: tracking ? "Stop" : "Start"
    iconSource: tracking ? "media-playback-pause" : "player-time"
    
    onClicked: {
        task.active = !task.active
    }
    
    onTrackingChanged: {
        checked = tracking
    }
}