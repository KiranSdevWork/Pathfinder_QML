import QtQml
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import PathModel

ApplicationWindow {
    id: root
    title:"QML Pathfinder [Red:Barrier Yellow:Path Green:Start Blue:Goal]"
    visible: true
    color:"lightgrey"
    width: 1024
    height: 768
    minimumWidth: 800
    minimumHeight: 600
    TableView {
        id: tableView //used to show field cells with status
        anchors.fill: parent
        rowSpacing: 1
        columnSpacing: 1
        interactive: true
        clip: true
        ScrollBar.vertical: ScrollBar {
            id: tableVerticalBar;
            active: tableHorizontalBar.active
            policy:ScrollBar.AlwaysOn
        }
        ScrollBar.horizontal: ScrollBar {
            id: tableHorizontalBar;
            active:  tableVerticalBar.active
        }
        delegate: Rectangle {
            id: cell // Each grid cell
            implicitWidth: 35
            implicitHeight: 35
            required property int value
            color : {
                        if (value==11) { "green";
                            } else if (value==12) { "yellow"; //Path
                        }
                            else if (value==13) {"blue"; //destination
                        }
                            else if (value==3) {"red"; //barrier
                        }
                            else {"gray"; //walkable cell
                        }
                    }
            Text {
                text: { //show X:Y on each cell
                    var message = "%1";
                    var row = model.row;
                    var message1 = ":%1";
                    var col = model.column
                    text : message.arg(col) + message1.arg(row)
                }
            }
        }
        model: PathModel {
            id: pathModel //backend model to process data
        }
    }

    header: Rectangle {
            id: header
            height: 50
            color: "#F3F3F4"
            GridLayout {
                columns:2
                 ColumnLayout {
                    RowLayout {
                        Button { //File selection
                          text: "Select json [Riskylab TileMap Compatible]"
                           palette.buttonText: "red"
                           onClicked: pathModel.loadfile()
                           Layout.rightMargin: 50
                           Layout.fillWidth: false
                        }
                        Text { //file name
                            text: pathModel.theChange
                        }
                    }
                 }
                ColumnLayout {
                    Text { //start point
                        textFormat: Text.StyledText
                        text: pathModel.startText
                    }
                    Text { //destination point
                        textFormat: Text.StyledText
                        text: pathModel.goalText
                    }
                }

            }
        }
}
