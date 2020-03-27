import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../accessoires"
import "../"


ScrollView{
    id:scroll
    contentWidth: -1
    anchors.fill: parent
    clip: true
    ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

    onWidthChanged:{
        if(width > 550){
            gridbox.columns=2}
        else{gridbox.columns=1}
    }

    function calc(){
        switch(combo_cordon.currentIndex){
        case 0:
            image_cordon.source="qrc:/Images/cordon_frontal.jpg"
            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonFrontal(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value)).toFixed(2)
            champs_angle.visible=false
            break;
        case 1:
            image_cordon.source="qrc:/Images/cordon_lateral.jpg"
            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonLateral(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value)).toFixed(2)
            champs_angle.visible=false
            break;
        case 2:
            image_cordon.source="qrc:/Images/cordon_oblique.jpg"
            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonOblique(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value,champs_angle.value)).toFixed(2)
            champs_angle.visible=true
            break;
        }

    }

    GridLayout{
    id:gridbox
    anchors.fill: parent

    Style_h1 {
        text: "Vérification d'un cordon de soudure"
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
        Layout.columnSpan:gridbox.columns>-1?gridbox.columns:1
    }

    GroupBox {
        id: box_geometrie
        label: Style_h2{text:"Géométrie du cordon"}
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                Layout.fillWidth: true
                Style_txt {text: qsTr("Type de cordon :")}

                ComboBox {
                    id: combo_cordon
                    Layout.fillWidth: true
                    model:["Cordon frontal","Cordon latéral","Cordon oblique"]
                    onCurrentIndexChanged: calc()
                  /*  onCurrentIndexChanged: {switch (currentIndex){
                        case 0:
                            image_cordon.source="qrc:/Images/cordon_frontal.jpg"
                            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonFrontal(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value)).toFixed(2)
                            champs_angle.visible=false
                            break;
                        case 1:
                            image_cordon.source="qrc:/Images/cordon_lateral.jpg"
                            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonLateral(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value)).toFixed(2)
                            champs_angle.visible=false
                            break;
                        case 2:
                            image_cordon.source="qrc:/Images/cordon_oblique.jpg"
                            champs_resistance.value=(Math.pow(10,-3)*formule.getResistanceCordonOblique(champs_gorge.value,champs_longueur.value,champ_fu.value,champ_bw.value,champ_gammaM2.value,champs_angle.value)).toFixed(2)
                            champs_angle.visible=true
                            break;
                        }
                    }*/
                }
            }

            Image{
                id:image_cordon
                Layout.preferredWidth: 400
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 180
                Layout.fillWidth: true
                fillMode: Image.PreserveAspectFit
            }

            Ligne_Champs{
                id:champs_longueur
                value: "100"
                value_nom: "Longueur de cordon :"
                value_unite: "mm"
                onValueChanged: calc()
            }

            Ligne_Champs{
                id : champs_gorge
                value: "3"
                value_nom: "Gorge :"
                value_unite : "mm"
                onValueChanged: calc()
            }

            Ligne_Champs{
                id:champs_angle
                value: "30"
                value_nom: "Angle :"
                value_unite: "°"
                onValueChanged: calc()
            }

            RowLayout {
                Style_txt {text: "Type d'acier :"}

                ComboBox {
                    id: combo_acier
                    model:formule.getBDD_Noms("Acier","Nuance")
                }
            }

            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    GroupBox {
        id: box_carac_acier
        label: Style_h2{text:"Caractéristiques de l'acier"}
        Layout.fillWidth: true
        Layout.alignment:Qt.AlignTop

        ColumnLayout {
            anchors.fill: parent

            Style_h3 {text: "Coefficents partiels de sécurité :"}

            Ligne_Champs{
                id:champ_gammaM2
                value_isEditable: false
                value_nom: "γM2 ="
                value: "1.25"
            }

            Style_h3 {text: "Propriétés physiques de l'acier :"}

            Ligne_Champs{
                id:champ_fy
                value_isEditable: false
                value_nom: "fy ="
                value: formule.getBDD_Value("Acier","fy","Nuance",combo_acier.currentText)
                value_unite: "MPa"
            }

            Ligne_Champs{
                id:champ_fu
                value_isEditable: false
                value_nom: "fu ="
                value: formule.getBDD_Value("Acier","fu","Nuance",combo_acier.currentText)
                value_unite: "MPa"
            }

            Ligne_Champs{
                id:champ_bw
                value_isEditable: false
                value_nom: "bw ="
                value: formule.getBDD_Value("Acier","bw","Nuance",combo_acier.currentText)
                value_unite: "MPa"
            }
        }
    }

    GroupBox {
        id: box_verif
        label: Style_h2{text:"Vérification"}
        Layout.fillWidth: true
        Layout.alignment:Qt.AlignTop

        ColumnLayout {
            anchors.fill: parent

            Ligne_Champs{
                id:champs_effort
                value: "10"
                value_nom: "Effort de traction :"
                value_unite: "kN"
            }

            Ligne_Champs{
                id:champs_resistance
                value_nom: "Résistance du cordon :"
                value_unite: "kN"
                value_isEditable: false
            }

            Ligne_Champs{
                id:txt_result
                value_nom: "Taux de travail η ="
                value_unite: "%"
                value: formule.getTauxtravail(champs_effort.value,champs_resistance.value).toFixed(2)
                onValueChanged: {
                    var cast=0.00;
                    cast=txt_result.value
                    if (cast>=100){value_color="red";}
                    else {value_color="green";}
                }
            }

          /*  GridLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBaseline
                rows: 2
                columns: 3





                Text {
                    text: "Taux de travail :"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                }
                Text {
                    text: "η ="
                    font.bold: true
                    Layout.fillWidth: false
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    id: txt_result
                    text: formule.getTauxtravail(champs_effort.value,champs_resistance.value).toFixed(2)
                    font.bold: true
                    Layout.fillWidth: false
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 12
                    onTextChanged: {
                        var cast=0.00;
                        cast=txt_result.text
                        if (cast>=100){color="red";txt_pourcent.color="red"}
                        else {color="green";txt_pourcent.color="green"}
                    }
                }

                Text {
                    id: txt_pourcent
                    text: "%"
                    font.bold: true
                    Layout.fillWidth: false
                    horizontalAlignment: Text.AlignHCenter
                }
            }*/

        }

    }

    }


}
        /*    Volet_Resultats{
                id:volet_resultats
                visible: cfg.isAndro()? false : true
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClickedChanged: {
                    remplir="<H2>Vérification d'un plat en Flexion</H2><BR> "
                   // remplir=remplir+"Pente de l'entaille i : "+chef.pente_i+"<BR>";
               }
            }*/



