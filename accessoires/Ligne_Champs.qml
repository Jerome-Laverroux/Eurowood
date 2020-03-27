import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12



RowLayout{
    SystemPalette{id:palette;colorGroup: SystemPalette.Active}

    property string value_nom:""
    property string value_unite:""
    property string value:""
    property double value_todouble:parseFloat(value)
    property bool value_isEditable: true
    property bool value_isBold: false
    property string value_mask: "9999"
    property string value_color : palette.text
    property int value_decimales: 2
    property real value_valmax: 9999
    property string last:value

    onValueChanged: {if(value != input_value.text){input_value.text=value}}

    Style_txt {
       text:value_nom
       color:palette.text
       font.bold:value_isBold
       Layout.fillWidth: true
       Layout.preferredHeight: 25
       Layout.topMargin: 1
    }
    TextField{
        id:input_value
        text: value
        color: value_color
        enabled: value_isEditable
        Layout.preferredWidth: 70
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        selectByMouse: true

        validator:  DoubleValidator{
                        locale: "en_US"
                        decimals: value_decimales
                        top:value_valmax
        }


        Keys.onEscapePressed:{focus=false; editingFinished()}
        Keys.onPressed: {
            if((event.key === Qt.Key_Enter) || (event.key === Qt.Key_Return)){input_value.focus=false;}
        }

        onActiveFocusChanged:{
            if(activeFocus && value_isEditable){text="";cursorPosition=0}
            if(!activeFocus && text === ""){
                text=last;
            }
        }
        onTextChanged: {
            if(value != text && text !==""){value=text;last=text}
        }
    }
    Style_txt{text:value_unite; color:palette.text}
}
