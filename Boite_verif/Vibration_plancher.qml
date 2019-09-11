import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import "../accessoires"


ColumnLayout {
    id : column_verif_vibration_plancher
    property string lBarre
    property string hBarre
    property string bBarre
    property string bandeChargement
    Layout.preferredHeight: 450
        Layout.fillWidth: true

        Text {
            id:titre_verif_plancher
                text: qsTr("Vérification de la vibration du plancher")
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                font.bold: true
                font.underline: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
            }



        RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                ColumnLayout{
                    id:colonne_gauche_plancher
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    GroupBox {
                        id:box_dimensions_plancher
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true
                        title: qsTr("Géométrie du plancher")

                        ColumnLayout {
                            anchors.fill: parent

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }

                            Ligne_Champs{
                                id:champs_b_plancher
                                value_nom: "Largeur du plancher"
                                value_unite: "mm"
                                value_mask: "999999"
                                value: "6000"
                            }

                            Ligne_Champs{
                                id:champs_b_panneau
                                value_nom: "Largeur des panneaux :"
                                value_unite: "mm"
                                value: "910"
                            }

                            Ligne_Champs{
                                id:champs_e_panneau
                                value_nom: "Epaisseur des panneaux OSB:"
                                value_unite: "mm"
                                value: "18"
                            }
                        }
                    }


                    GroupBox {
                        id:box_frequence_propre
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true
                        title: qsTr("Calcul de la fréquence propre du plancher")

                        ColumnLayout {
                            anchors.fill: parent

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }
                            Ligne_Champs{
                                id:champs_charges_permanentes
                                value_nom: "Charges permanentes ="
                                value_unite: "kg/m²"
                                value:"91"
                            }
                            Ligne_Champs{
                                id:champs_charges_exploitation
                                value_nom: "Charges d'exploitaion ="
                                value_unite: "kg/m²"
                                value:"150"
                            }
                            Ligne_Champs{
                                id:champs_m
                                value_nom: "m ="
                                value_unite: "kg/m²"
                                value_isEditable: false
                                value : champs_charges_permanentes.value_todouble + 0.3*champs_charges_exploitation.value_todouble
                            }
                            Ligne_Champs{
                                id:champs_moment_I
                                value_nom: "Moment quadratique I ="
                                value_unite: "m4"
                                value_isEditable: false
                                value: (Math.pow(hBarre/1000,3)*bBarre/12000).toExponential(2)

                            }
                            Ligne_Champs{
                                id:champs_frequence
                                value_nom: "Fréquence porpre f1 ="
                                value_unite: "Hz"
                                value_isEditable: false
                                value:formule.getPlancherFrequence(lBarre,champs_m.value,champs_EIl.value).toFixed(2)
                            }
                        }
                    }

                    Item{
                        Layout.fillHeight: true
                    }
                }



                ColumnLayout {
                    id:colonne_droite_plancher
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GroupBox {
                        id:box_critere_vitesse
                        title: "Vérification de la vitesse de vibration :"
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true

                        ColumnLayout {
                            anchors.fill: parent

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }
                                Text{
                                    text: "Nombre de fréquence de vibration inférieure à 40 Hz :"
                                    font.underline: true
                                }

                            Ligne_Champs{
                                id:champs_EIl
                                value_isEditable: false
                                value_nom: "(EI)l = "
                                value_unite: "m4"
                                value: formule.getPlancherRigidite(formule.getE0mean(carac.classe_bois),bandeChargement,champs_moment_I.value).toExponential(2)
                            }

                            Ligne_Champs{
                                id:champs_EIb
                                value_isEditable: false
                                value_nom: "(EI)b = "
                                value_unite: "m4"
                                property double moment_I : 1*Math.pow(champs_e_panneau.value/1000,3)/12
                                value: (4930*moment_I*Math.pow(10,12)).toExponential(2)
                            }

                            Ligne_Champs{
                                id:champs_n40
                                value_isEditable: false
                                value_nom: "n40 = "
                                value_unite: ""
                                value: formule.getPlanchern40(champs_frequence.value,lBarre,champs_b_plancher.value,champs_EIl.value,champs_EIb.value).toFixed(2)
                            }
                                Text{
                                    text: "Vibration impulsionnelle du plancher :"
                                    font.underline: true
                                }

                            Ligne_Champs{
                                id:champs_vitesseb
                                value_isEditable: false
                                value_nom: "b= "
                                value_unite: ""
                                function calcb(){
                                    var u=0.0;
                                    var out="";
                                    u=champs_u.value;
                                    if((0.5< u) && (u <= 1.0)){out= (120+((150-120)/0.5)*(1-u)).toFixed(2);}
                                    else {
                                        if((1.0 < u) && (u <= 2.0)){out= (80+((120-80)/1)*(2-u)).toFixed(2);}
                                        else {
                                            if((2.0 < u) &&(u <= 4.0)){out= (50+((80-50)/2)*(4-u)).toFixed(2);}
                                            else{out="N/C";}
                                        }
                                    }
                                    value=out;
                                }
                            }

                            Ligne_Champs{
                                id:champs_epsilon
                                value_isEditable: false
                                value_nom: "ξ ="
                                value_unite: ""
                                value: "0.01"
                            }

                            Ligne_Champs{
                                id:champs_vmax
                                value_isEditable: false
                                value_nom: "vmax ="
                                value_unite: "mm/s"
                                value: formule.getPlancherVitessemax(champs_vitesseb.value,champs_epsilon.value,champs_frequence.value).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champs_v
                                value_isEditable: false
                                value_nom: "v ="
                                value_unite: "mm/s"
                                value: formule.getPlancherVitesse(champs_n40.value,champs_m.value,champs_b_plancher.value,lBarre).toFixed(2)
                            }

                                Text{
                                    id:txt_critere_v
                                    font.bold: true
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 16
                                    Layout.fillWidth: true
                                    text:(champs_v.value_todouble<champs_vmax.value_todouble)? "Validé" :"Non validé (v > vmax)"
                                    color:(champs_v.value_todouble<champs_vmax.value_todouble)?"green":"red"
                                }
                        }
                    }

                    GroupBox {
                        id:box_critere_deplacement
                        font.bold: true
                        font.pointSize: 10
                        Layout.fillWidth: true
                        title: qsTr("Vérification du critère de flèche statique :")

                        ColumnLayout{
                            anchors.fill: parent
                            RowLayout {
                                Layout.fillWidth: true
                                ColumnLayout {
                                    Ligne_Champs{
                                        id:champs_ks
                                        value_isEditable: false
                                        value_nom: "Souplesse d'une solive : ks ="
                                        value: formule.getPlancherSouplesse_solive(lBarre,formule.getE0mean(carac.classe_bois),champs_moment_I.value*Math.pow(10,12)).toExponential(2)
                                        value_unite: "mm/N"
                                    }

                                    Ligne_Champs{
                                        id:champs_kp
                                        value_isEditable: false
                                        value_nom: "Souplesse d'un panneau : kp ="
                                        value: formule.getPlancherSouplesse_panneau(bandeChargement,champs_e_panneau.value,champs_b_panneau.value,4930).toExponential(2)
                                        value_unite: "mm/N"
                                    }

                                    Ligne_Champs{
                                        id:champs_u
                                        value_isEditable: false
                                        value_nom: "Déplacement u ="
                                        value: formule.getPlancherDeplacement(champs_ks.value,champs_kp.value).toFixed(2)
                                        value_unite: "mm/kN"
                                        onValueChanged: {champs_vitesseb.calcb();}
                                    }
                                        Text{
                                            Layout.fillWidth: true
                                            id:txt_critere_u
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            font.pixelSize: 16
                                            text:((champs_u.value_todouble<1.6) && (champs_u.value_todouble>1.0))? "Validé" : "Non validé u compris entre 1 et 1.6"
                                            color:((champs_u.value_todouble<1.6) && (champs_u.value_todouble>1.0))?"green":"red"
                                        }
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
