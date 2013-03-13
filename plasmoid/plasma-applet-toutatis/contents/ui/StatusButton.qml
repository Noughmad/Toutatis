import QtQuick 1.0
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: button
    property string status
    signal clicked()
    
    function statusColor(status) {
        if (status == "done") {
            return "blue"
        } else if (status == "inprogress") {
            return "green"
        } else if (status == "todo") {
            return "yellow"
        } else if (status == "urgent") {
            return "red"
        } else if (status == "cancelled") {
            return "gray"
        } else {
            return "yellow"
        }
    }
    
    function statusText(status) {
        if (status == "done") {
            return i18n("Done")
        } else if (status == "inprogress") {
            return i18n("In progress")
        } else if (status == "todo") {
            return i18n("To do")
        } else if (status == "urgent") {
            return i18n("Urgent")
        } else if (status == "cancelled") {
            return i18n("Cancelled")
        } else {
            return i18n("To do")
        }
    }
    
    width: label.width + 10
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            button.clicked()
        }
    }
    
    Rectangle {
        anchors.fill: parent
        color: statusColor(status)
        radius: 3
    }
    
    PlasmaComponents.Label {
        id: label
        text: statusText(status)
        elide: Text.ElideRight   
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
    }
}