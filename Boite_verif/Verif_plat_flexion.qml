import QtQuick 2.12
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../accessoires"

Item {
    id:chef
    anchors.fill: parent
    visible: true
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
                        text: "Vérification d'un plat en flexion"
                        font.underline: true
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        Layout.columnSpan:gridbox.columns
                        font.pixelSize: 18
                    }
                    GroupBox {
                        id: box_geometrie
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        title: "Géométrie du plat"

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
                                Text {text: "Type d'acier :"}
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

                            Item{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }

                    }

                    GroupBox {
                        id: box_verif_percage
                        font.bold: true
                        title: "Vérification des perçages"

                        ColumnLayout {
                            anchors.fill: parent

                                Text {
                                    text: "Caractéristiques des perçages :"
                                    font.underline: true
                                    font.pixelSize: 12
                                    Layout.fillWidth: true
                                }

                            Ligne_Champs{
                                id:champs_nbr_percage
                                value_nom: cfg.isAndro()?"Nbre perçages(suivant h)":"Nombre de perçage (sens de la hauteur) :"
                                value_unite: ""
                                value: "3"
                                //value_largeur: box_verif_percage.largeur

                            }

                            Ligne_Champs{
                                id:champs_diam
                                value_nom:cfg.isAndro()?"Diam perçages :": "Diamètre de perçage :"
                                value_unite: "mm"
                                value: "14"
                              //  value_largeur: box_verif_percage.largeur

                            }

                            Ligne_Champs{
                                id:champs_verif_percage
                                value_nom: "η ="
                                value_unite: "%"
                                value_isEditable : false
                               // value_largeur: box_verif_percage.largeur
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
                            Text{id : champs_message}


                                Text {
                                    text: "Exemple de plaque en flexion :"
                                    font.underline: true
                                    Layout.fillWidth: true
                                }



                            Image{
                                source:"qrc:/Images/plat_flexion.jpg"
                                Layout.alignment: Qt.AlignTop |Qt.AlignLeft
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
                        Layout.fillWidth: true
                      //  Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        title: "Caractéristiques de l'acier "

                        ColumnLayout {
                            anchors.fill: parent

                                Text {
                                    text: "Coefficents partiels de sécurité :"
                                    font.underline: true
                                    Layout.fillWidth: true
                                    font.pixelSize: 12
                                }


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
                                Text {
                                    text: "Propriétés physiques de l'acier :"
                                    font.underline: true
                                    Layout.fillWidth: true
                                    font.pixelSize: 12
                                }

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
                        font.bold: true
                      //  Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        title: "Vérification"

                        ColumnLayout {
                            anchors.fill: parent


                            Ligne_Champs{
                                id:champs_effort
                                value_nom: "Moment :"
                                value_unite: "kN.m"
                                value: "10"
                            }
                            Text{
                                text:"Résistance du plat :"
                                font.underline: true
                                Layout.fillWidth: true
                            }

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
                }

        }
            Volet_Resultats{
                id:volet_resultats
                visible: cfg.isAndro()? false : true
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClickedChanged: {
                    remplir="<H2>Vérification d'un plat en Flexion</H2><BR> "
                   // remplir=remplir+"Pente de l'entaille i : "+chef.pente_i+"<BR>";
                }
            }

    }
}

