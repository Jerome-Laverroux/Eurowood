import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../accessoires"



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

    GridLayout{
        id:gridbox
        anchors.fill: parent

        Style_h1 {
            text: "Vérification d'un plat en flexion"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
            Layout.columnSpan:gridbox.columns>-1?gridbox.columns:1
        }

        GroupBox {
            id: box_geometrie
            label: Style_h2{text:"Géométrie du plat"}
            Layout.fillWidth: true
            Layout.alignment:Qt.AlignTop

            ColumnLayout {
                anchors.fill: parent

               Ligne_Champs{
                   id:champs_epaisseur
                   value_nom: "Epaisseur :"
                   value_unite: "mm"
                   value: "6"
                   }
               Ligne_Champs{
                   id:champs_hauteur
                   value_nom: "Hauteur :"
                   value_unite: "mm"
                   value: "300"
               }

                RowLayout {
                    Layout.fillWidth: true
                    Style_txt {text: "Type d'acier :"}
                    Item{Layout.fillWidth: true}
                    ComboBox {
                        id: combo_acier
                        model:formule.getBDD_Noms("Acier","Nuance")
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Style_txt {text:"Inox :"}
                    Item{Layout.fillWidth: true}
                    ComboBox {
                        id: combo_inox
                        model :["Non","Oui"]
                        onCurrentIndexChanged: {if(combo_inox.currentIndex==0){
                                champs_gammaM0.value="1.0"}
                                else{champs_gammaM0.value="1.1"}
                        }
                    }
                }

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

        }

        GroupBox {
            id: box_verif_percage
            label: Style_h2{text:"Vérification des perçages"}
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            ColumnLayout {
                anchors.fill: parent

                Style_h3 {text: "Caractéristiques des perçages :"}

                Ligne_Champs{
                    id:champs_nbr_percage
                    value_nom: cfg.isAndro()?"Nbre perçages(suivant h)":"Nombre de perçage (sens de la hauteur) :"
                    value_unite: ""
                    value: "3"
                }

                Ligne_Champs{
                    id:champs_diam
                    value_nom:cfg.isAndro()?"Diam perçages :": "Diamètre de perçage :"
                    value_unite: "mm"
                    value: "14"
                }

                Ligne_Champs{
                    id:champs_verif_percage
                    value_nom: "η ="
                    value_unite: "%"
                    value_isEditable : false

                    value: formule.getVerifPercagePlatFlexion(champs_epaisseur.value,champs_hauteur.value,champs_nbr_percage.value,champs_diam.value,champs_fu.value,champs_fy.value,champs_gammaM0.value,champs_gammaM2.value).toFixed(2)
                    onValue_todoubleChanged: {
                        if (champs_verif_percage.value_todouble<=100.0){
                            champs_verif_percage.value_color="green"
                            champs_message.text = "Le perçage n'est pas impactant"
                        }
                        else {
                            champs_verif_percage.value_color="red"
                            champs_message.text = "Le perçage réduit la résistance"
                        }
                    }
                }
                Style_txt{id : champs_message}

                Style_h3 {
                    text: "Exemple de plaque en flexion :"
                    font.underline: true
                    Layout.fillWidth: true
                }

                Image{
                    source:"qrc:/Images/plat_flexion.jpg"
                    Layout.preferredWidth: 400
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.preferredHeight: 180
                    Layout.fillWidth: true
                    fillMode: Image.PreserveAspectFit
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

                Style_h3 {text: "Coefficents partiels de sécurité :" }

                Ligne_Champs{
                    id:champs_gammaM0
                    value_nom: "γM0 ="
                    value_unite: ""
                    value_isEditable: false
                }

                Ligne_Champs{
                    id:champs_gammaM2
                    value_nom: "γM2 ="
                    value_unite: ""
                    value: "1.25"
                    value_decimales: 2
                    value_isEditable: false
                }

                Style_h3 {text: "Propriétés physiques de l'acier :"}

                Ligne_Champs{
                    id:champs_fy
                    value_nom: "fy ="
                    value_unite: "MPa"
                    value_isEditable: false
                    value:formule.getBDD_Value("Acier","fy","Nuance",combo_acier.currentText)
                }

                Ligne_Champs{
                    id:champs_fu
                    value_nom: "fu ="
                    value_unite: "MPa"
                    value_isEditable: false
                    value:formule.getBDD_Value("Acier","fu","Nuance",combo_acier.currentText)
                }

                Ligne_Champs{
                    id:champs_bw
                    value_nom: "bw ="
                    value_unite: ""
                    value_isEditable: false
                    value:formule.getBDD_Value("Acier","bw","Nuance",combo_acier.currentText)
                }
            }
        }

        GroupBox {
            id: box_verif
            label: Style_h2{text:"Vérification"}
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            ColumnLayout {
                anchors.fill: parent

                Ligne_Champs{
                    id:champs_effort
                    value_nom: "Moment :"
                    value_unite: "kN.m"
                    value: "10"
                }
                Style_h3{text:"Résistance du plat :"}

                Ligne_Champs{
                    id:champs_wely
                    value_nom: "Wely ="
                    value_unite: "mm3"
                    value_isEditable: false
                    value: ((champs_epaisseur.value*Math.pow(champs_hauteur.value,3)/12)/(champs_hauteur.value/2)).toExponential(2)
                }

                Ligne_Champs{
                    id:champs_resistance
                    value_nom: "Résistance plaque :"
                    value_unite: "kN.m"
                    value_isEditable: false
                    value:(Math.pow(10,-6)*formule.getResistancePlatFlexion(champs_wely.value,champs_fy.value,champs_gammaM0.value,champs_verif_percage.value)).toFixed(2)
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
            }

        }
    }

}
/*  Volet_Resultats{
    id:volet_resultats
    visible: cfg.isAndro()? false : true
    Layout.fillWidth: true
    Layout.fillHeight: true
    onClickedChanged: {
        remplir="<H2>Vérification d'un plat en Flexion</H2><BR> "
       // remplir=remplir+"Pente de l'entaille i : "+chef.pente_i+"<BR>";
    }
}*/




