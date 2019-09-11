import QtQuick 2.0
import QtQuick.Layouts 1.12


Text {
    property string txt:""
    text: txt
    font.underline: true
    font.bold: true
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    Layout.fillWidth: true
    Layout.preferredHeight: 30
    Layout.columnSpan:gridbox1.columns
    font.pixelSize: 18
}
