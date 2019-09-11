import QtQuick 2.0
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5


RowLayout{
    property string value_nom:""
    property string value_unite:""
    property string value:""
    property double value_largeur: 150
    property double value_todouble:parseFloat(value)
    property bool value_isEditable: true
    property bool value_isBold: false
    property string value_mask: "9999"
    property string value_color : "black"
    property int value_decimales: 2
    property real value_valmax: 9999
    property string last:value

    onValueChanged: {
       // console.log(value_nom,value)
        if(value != input_value.text){input_value.text=value}
    }

    Reglage_defaut{
        id:reglages
    }


    Text {
       text:value_nom
       font.bold:value_isBold
       Layout.preferredWidth:cfg.isAndro()?value_largeur+50 : value_largeur
    }

    Rectangle{
        Layout.preferredWidth: reglages.defaut_largeur_champs
        Layout.preferredHeight: reglages.defaut_hauteur_champs
        color: (value_isEditable)? "#EDE259" : "white"

        TextInput{
            id:input_value
            anchors.fill: parent
            text: value
            validator:  DoubleValidator{
                                        locale: "en_US"
                                        decimals: value_decimales
                                        top:value_valmax
            }
            color : value_color
            readOnly: (value_isEditable) ? false : true
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            selectByMouse: true

            Keys.onPressed: {
                if((event.key == Qt.Key_Enter) || (event.key == Qt.Key_Return)){input_value.focus=false;}
            }

            onActiveFocusChanged:{
                if(activeFocus && value_isEditable){text="";cursorPosition=0}
                if(!activeFocus && text === ""){
                    console.log(value_nom+ " n'a plus le focus et est vide");
                    text=last;
                }
            }
            onTextChanged: {
                if(value != text && text !==""){value=text;last=text}
            }


        }
    }
    Text{
        text:value_unite
        color : value_color
    }

    Item{Layout.fillWidth: true}
}
