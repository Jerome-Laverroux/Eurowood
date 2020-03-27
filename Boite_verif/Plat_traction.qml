import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.13
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

    GridLayout{
    id:gridbox
    anchors.fill: parent

        Style_h1 {
            text: "Vérification d'un plat en traction"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
            Layout.columnSpan:gridbox.columns>-1?gridbox.columns:1
        }

        GroupBox {
            id: box_geometrie
            label: Style_h2{text:"Géométrie et effort"}
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent

               Ligne_Champs{
                   id:champs_epaisseur
                   value_nom: "Epaisseur :"
                   value_unite: "mm"
                   value: "3"
                   }
               Ligne_Champs{
                   id:champs_hauteur
                   value_nom: "Hauteur :"
                   value_unite: "mm"
                   value: "100"
               }

                RowLayout {
                    Layout.fillWidth: true
                    Style_txt {text:"Type d'acier :"}
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

                Ligne_Champs{
                    id:champs_effort
                    value_nom: "Effort de traction :"
                    value_unite: "kN"
                    value: "10"
                }

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

        }

        GroupBox {
            id: box_chemin_rupture
            label: Style_h2{text:"Géométrie des chemins de rupture"}
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            ColumnLayout {
                anchors.fill: parent
                    Style_h3 {text: "Chemins de rupture possibles :"}
                    Style_txt {text:"An = longueur ligne de rupture n"}

                Ligne_Champs{
                    id:champs_a1
                    value_nom: "A1 ="
                    value_unite: "mm"
                    value: "200"
                }
                Ligne_Champs{
                    id:champs_a2
                    value_nom: "A2 ="
                    value_unite: "mm"
                    value: "200"
                }

                Ligne_Champs{
                    id:champs_anet
                    value_nom: "Anet = min (A1,A2) ="
                    value_unite: "mm"
                    value: Math.min(champs_a1.value,champs_a2.value)
                }

                Style_txt {text: "Exemples de chemins de rupture :"}


                Image{
                    source:"qrc:/Images/rupture_traction.png"
                    Layout.alignment: Qt.AlignTop
                    Layout.maximumWidth:cfg.isAndro()?Screen.desktopAvailableWidth-50 : 350
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: paintedHeight
                    Layout.fillWidth: cfg.isAndro()? true : false
                }
            }
        }

        GroupBox {
            id: box_carac_acier
            label: Style_h2{text:"Caractéristiques, et vérifications"}
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

            ColumnLayout {
                anchors.fill: parent
                Style_h3 {text: "Coefficents partiels de sécurité EC3 :"}

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

                Style_h3 {text: "Propriétés physiques de l'acier"}

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

                Style_h3 {text: "Vérification plaque"}

                Ligne_Champs{
                    id:champs_resistance
                    value_nom: "Résistance plaque :"
                    value_unite: "kN"
                    value_isEditable: false
                    value: (Math.pow(10,-3)*formule.getResistancePlatTraction(champs_epaisseur.value,champs_hauteur.value,champs_anet.value,champs_fu.value,champs_fy.value,champs_gammaM0.value,champs_gammaM2.value)).toFixed(2)
                }

                Ligne_Champs{
                    id:txt_result
                    value:formule.getTauxtravail(champs_effort.value,champs_resistance.value).toFixed(2)
                    value_nom: "Taux de travail : η ="
                    value_unite: "%"
                    value_isEditable: false
                    value_isBold: true
                    onValueChanged: {
                        var cast=0.00;
                        cast=txt_result.value
                        if (cast>=100){value_color="red"}
                        else {value_color="green"}
                    }
                }
            }
        }
        Item{Layout.fillHeight: true}
    }
}

// }


/*   Volet_Resultats{
    id:volet_resultats
    visible: cfg.isAndro()? false : true
    Layout.fillHeight: true
    Layout.fillWidth: true
    onClickedChanged: {
        remplir="<H2>Vérification d'un plat en traction</H2><BR> ";
    /*   remplir=remplir+"Pente de l'entaille i : "+chef.pente_i+"<BR>";
       remplir=remplir+"Rapport de hauteur α : "+chef.alpha+"<BR>";
       remplir=remplir+"Coefficient kn : "+chef.kn+"<BR>";
       remplir=remplir+"Coefficient de réduction dû à l'entaille kv : "+chef.kv+"<BR>";
       remplir=remplir+"Contrainte de cisaillement : "+chef.contrainte+" MPa<BR>";
       remplir=remplir+"Résistance au cisaillement : "+chef.resitance+" MPa<BR>";
       remplir=remplir+"Taux de travail η : "+chef.taux+"%<BR>";*/
  //  }
//  }*/



