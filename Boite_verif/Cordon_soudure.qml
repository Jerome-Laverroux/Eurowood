import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../accessoires"
import "../"

Item {
    id:chef
    anchors.fill: parent
    visible: true
    property bool is_entaille_haut: false
    Reglage_defaut {id : reglages}

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

            Text {
                text: "Vérification d'un cordon de soudure"
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
                Layout.fillWidth: true
                title: "Géométrie du cordon"

                ColumnLayout {
                    anchors.fill: parent

                    RowLayout {
                        Layout.fillWidth: true
                        Text {
                            text: qsTr("Type de cordon :")
                        }

                        ComboBox {
                            id: combo_cordon
                            Layout.fillWidth: true
                            model:["Cordon frontal","Cordon latéral","Cordon oblique"]
                            onCurrentIndexChanged: {switch (currentIndex){
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
                        }
                    }

                    Image{
                        id:image_cordon
                        Layout.preferredWidth: 200
                        Layout.preferredHeight : 200
                        verticalAlignment: Qt.AlignVCenter
                    }

                    Ligne_Champs{
                        id:champs_longueur
                        value: "100"
                        value_nom: "Longueur de cordon :"
                        value_unite: "mm"
                    }

                    Ligne_Champs{
                        id : champs_gorge
                        value: "3"
                        value_nom: "Gorge :"
                        value_unite : "mm"
                    }

                    Ligne_Champs{
                        id:champs_angle
                        value: "30"
                        value_nom: "Angle :"
                        value_unite: "°"
                    }

                    RowLayout {
                        Text {text: "Type d'acier :"}
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
                font.bold: true
                Layout.fillWidth: true
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
                        id:champ_gammaM2
                        value_isEditable: false
                        value_nom: "γM2 ="
                        value: "1.25"
                    }

                    Text {
                        text: "Propriétés physiques de l'acier :"
                        font.underline: true
                        Layout.fillWidth: true
                    }

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
                font.bold: true
                Layout.preferredHeight: 200
                Layout.fillWidth: true
                title: qsTr("Vérification")

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

