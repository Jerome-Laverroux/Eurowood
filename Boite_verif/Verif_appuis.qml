import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../accessoires"

ColumnLayout {
    id : column_verif_appui
    property string lBarre
    property string bBarre
    property string hBarre
    property string materiau
    property string classe_Bois
    property string kmod
    property string gamma_M

    Layout.preferredHeight: 450
        Layout.fillWidth: true

        // Rectangle du titre
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            Text {
                text: qsTr("Vérification de la compression à l'appui")
                anchors.fill: parent
                font.bold: true
                font.underline: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
            }
        }


        //Rectangle des caractéritiques géométriques de l'appui
        Rectangle {
            color: "#ffffff"
            Layout.fillHeight: true
            Layout.preferredHeight: -1
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                GroupBox {
                    font.bold: true
                    font.pointSize: 10
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    title: qsTr("Géométrie de l'appui")

                    ColumnLayout {
                        anchors.fill: parent

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 10
                        }

                        Ligne_Champs{
                            id:champ_l_appui
                            value_nom: "Largeur de l'appui"
                            value_unite: "mm"
                            value: "200"
                        }
                        Ligne_Champs{
                            id:champ_b_appui
                            value_nom: "Largeur de la barre (b) :"
                            value_unite: "mm"
                            value: bBarre
                        }

                        RowLayout {
                            Layout.fillHeight: false

                            Text {
                                text: qsTr("Type d'appui :")
                                font.pixelSize: 12
                            }

                            Item {
                                Layout.preferredHeight: 1
                                Layout.preferredWidth: 20
                                Layout.fillHeight: false
                            }

                            ComboBox {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 200
                                model : ["Appui continu","Appui discret"]
                                onCurrentIndexChanged:{
                                    if(currentIndex==0){
                                        image_appui.source="qrc:/Images/appui_continu.png";
                                        chef.is_appui_continu=true;
                                    }
                                   else{
                                        image_appui.source = "qrc:/Images/appui_discret.png";
                                        chef.is_appui_continu=false
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 1
                            }

                        }

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: qsTr("Distances aux bords de l'appui :")
                                font.bold: true
                                font.pixelSize: 12
                            }
                        }

                        Ligne_Champs{
                            id:champ_a
                            value_nom: "a =:"
                            value_unite: "mm"
                            value: "0"
                        }

                        Ligne_Champs{
                            id:champ_l1
                            value_nom: "l1 =:"
                            value_unite: "mm"
                            value: (lBarre - champ_l_appui.value)
                        }

                        RowLayout {
                            Text {
                                text: qsTr("Effort de compression :")
                                font.pixelSize: 12
                                font.bold: true
                            }
                        }

                        Ligne_Champs{
                            id:champ_effort_compression
                            value_nom: "Fc90 ="
                            value_unite: "kN"
                            value: "10"
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                    }
                }
                // Rectangle des coefficients de calcul pour un appui en compression
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GroupBox {
                        title: "Coefficents de calcul :"
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }

                            Ligne_Champs{
                                id:champ_txt_lef
                                value_isEditable: false
                                value_nom: "Longueur efficace lef = "
                                value_unite: "mm"
                                value: formule.getLongueurEfficaceCompression(champ_l_appui.value,champ_a.value,champ_l1.value)
                            }
                            Ligne_Champs{
                                id:champ_txt_kc90
                                value_isEditable: false
                                value_nom: "kc90 = "
                                value_unite: ""
                                value: formule.getCompression_kc90(materiau,hBarre,champ_l1.value)
                            }

                            Image {
                                id: image_appui
                                Layout.preferredWidth: 450
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                Layout.preferredHeight: 200
                                Layout.fillWidth: true
                                fillMode: Image.PreserveAspectFit
                            }
                        }
                    }

                    GroupBox {
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true
                        title: qsTr("Vérification :")

                        ColumnLayout{
                            anchors.fill: parent
                            RowLayout {
                                Layout.fillWidth: true
                                ColumnLayout {
                                    Ligne_Champs{
                                        id:champ_txt_contrainte_compression
                                        value_isEditable: false
                                        value_nom: "Contrainte de compression"
                                        value: formule.getContrainteCompressionTransversale(champ_effort_compression.value*100,champ_b_appui.value,champ_txt_lef.value).toFixed(2)
                                        value_unite: "MPa"
                                    }
                                    Ligne_Champs{
                                        id:champ_txt_resistance_compression
                                        value_isEditable: false
                                        value_nom: "Résistance à la compression ="
                                        value: formule.getResistanceCompressionTransversale(formule.getfc90k(classe_Bois),kmod,gamma_M).toFixed(2)
                                        value_unite: "MPa"
                                    }
                                }

                                Item{Layout.fillWidth: true}

                                Ligne_Champs{
                                    value_nom: "Taux de travail η :"
                                    value_isEditable: false
                                    value_isBold: true
                                    value: formule.getTauxtravailCompression(champ_txt_contrainte_compression.value,champ_txt_resistance_compression.value,champ_txt_kc90.value).toFixed(2);
                                    value_color: value_todouble >= 100 ? "red" : "green"
                                    value_unite: "%"
                                }
                            }

                        }
                    }

                    Item{
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
