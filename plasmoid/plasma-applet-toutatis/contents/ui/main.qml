import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    PlasmaCore.DataSource {
        id: dataSource
        engine: "com.noughmad.toutatis"
        connectedSources: ["Projects"]
        interval: 500
        
        onNewData:{
            if (sourceName== "Projects") {
                listView.model = data.names
            }
        }
    }

    ListView {
        id: listView
        width: parent.width
        height: parent.height
        
        delegate: Text {
            width: parent.width
            height: 30
            text: modelData
        }
    }
}