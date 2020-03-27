import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.13
import "../accessoires"
import "../"




Item {
    id:chef
    anchors.fill: parent
    visible: true
    property double ratiofenetre: 1.5
    property bool is_entaille_haut: false

    Reglage_defaut {
    id : reglages
    }

    RowLayout{
        id:principal
        anchors.fill: parent
        anchors.margins: 10

        ScrollView{
            id:scroll
            contentWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

            GridLayout{
            id:gridbox
            anchors.fill: parent
            columns: cfg.isAndro()? 1: 2
            flow: GridLayout.TopToBottom

            Text {
                text: "Vérification d'un plat en traction"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredHeight: 30
                Layout.fillWidth: true
                Layout.columnSpan:gridbox.columns
                font.pixelSize: 18
            }

            GroupBox {

                id: box_geometrie
                font.bold: true
                title: "Géométrie et effort"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop


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
                        Text {text:"Type d'acier :"}
                        Item{Layout.fillWidth: true}
                        ComboBox {
                            id: combo_acier
                            model:formule.getBDD_Noms("Acier","Nuance")
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Text {text:"Inox :"}
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
                font.bold: true
                title: "Géométrie des chemins de rupture"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                property double largeur: 120

                ColumnLayout {
                    anchors.fill: parent
                        Text {
                            text: "Chemins de rupture possibles :"
                            font.underline: true
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                        Text {text:"An = longueur ligne de rupture n"}


                    Ligne_Champs{
                        id:champs_a1
                        value_nom: "A1 ="
                        value_unite: "mm"
                        value: "200"
                        //value_largeur: box_chemin_rupture.largeur
                    }
                    Ligne_Champs{
                        id:champs_a2
                        value_nom: "A2 ="
                        value_unite: "mm"
                        value: "200"
                      //  value_largeur: box_chemin_rupture.largeur
                    }

                    Ligne_Champs{
                        id:champs_anet
                        value_nom: "Anet = min (A1,A2) ="
                        value_unite: "mm"
                        value: Math.min(champs_a1.value,champs_a2.value)
                       // value_largeur: box_chemin_rupture.largeur
                    }

                    Text {
                        text: "Exemples de chemins de rupture :"
                        font.underline: true
                    }


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
                font.bold: true
                title: "Caractéristiques, et vérifications"
                Layout.columnSpan: gridbox.columns
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                property double largeur: 100

                ColumnLayout {
                    anchors.fill: parent
                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: "Coefficents partiels de sécurité EC3 :"
                            font.underline: true
                            Layout.fillWidth: false
                            font.pixelSize: 12
                        }
                    }

                    Ligne_Champs{
                        id:champs_gammaM0
                        value_nom: "γM0 ="
                        value_unite: ""
                        value_isEditable: false
                      //  value_largeur: box_carac_acier.largeur
                    }

                    Ligne_Champs{
                        id:champs_gammaM2
                        value_nom: "γM2 ="
                        value_unite: ""
                        value: "1.25"
                        value_decimales: 2
                        value_isEditable: false
                       // value_largeur: box_carac_acier.largeur

                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: "Propriétés physiques de l'acier"
                            font.underline: true
                            Layout.fillWidth: true
                            font.pixelSize: 12
                        }
                    }

                    Ligne_Champs{
                        id:champs_fy
                        value_nom: "fy ="
                        value_unite: "MPa"
                        value_isEditable: false
                        value:formule.getBDD_Value("Acier","fy","Nuance",combo_acier.currentText)
                      //  value_largeur: box_carac_acier.largeur

                    }

                    Ligne_Champs{
                        id:champs_fu
                        value_nom: "fu ="
                        value_unite: "MPa"
                        value_isEditable: false
                        value:formule.getBDD_Value("Acier","fu","Nuance",combo_acier.currentText)
                      //  value_largeur: box_carac_acier.largeur

                    }

                    Ligne_Champs{
                        id:champs_bw
                        value_nom: "bw ="
                        value_unite: ""
                        value_isEditable: false
                        value:formule.getBDD_Value("Acier","bw","Nuance",combo_acier.currentText)
                      //  value_largeur: box_carac_acier.largeur

                    }

                    Text {
                        text: "Vérification plaque"
                        font.underline: true
                        Layout.fillWidth: true
                        font.pixelSize: 12
                    }

                    Ligne_Champs{
                        id:champs_resistance
                        value_nom: "Résistance plaque :"
                        value_unite: "kN"
                        value_isEditable: false
                      //  value_largeur: box_carac_acier.largeur
                        value: (Math.pow(10,-3)*formule.getResistancePlatTraction(champs_epaisseur.value,champs_hauteur.value,champs_anet.value,champs_fu.value,champs_fy.value,champs_gammaM0.value,champs_gammaM2.value)).toFixed(2)

                    }

                    GridLayout {
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
                    }
                }
            }

            Item{Layout.fillHeight: true}
            }

            }

        }
       

        Volet_Resultats{
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
            }
        }    
    }


