import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import "../accessoires"


ColumnLayout {

        id: verif_entaille
        Layout.preferredHeight: 400
        Layout.fillWidth: true
        property string hBarre
        property string bBarre
        property string materiau
        property string classe_bois
        property string kmod
        property string gamma_m
        property bool is_entaille_haut: false
        property string ndc:""

        function calc_ndc(){
            ndc=""
            ndc="<H3>Vérification en cisaillement :</H3><BR>";
            ndc=ndc+combo_type_entaille[combo_type_entaille.currentIndex]+"<BR>";
            if (combo_type_entaille.currentIndex==0){
                ndc=ndc+"coefficient de fissuration kcr : "+champ_input_kcr.value+"<BR>";
                ndc=ndc+"Contrainte de cisaillement : "+champ_txt_contrainte_cisaillement.value+"<BR>";
                ndc=ndc+"Résistance au cisaillement : "+champ_txt_resistance_cisaillement.value+"<BR>";
            }
            else{
                ndc=ndc+"Hauteur de l'entaille : "+champ_input_he.value+"<BR>";
                ndc=ndc+"Largeur de l'entaille : "+champ_input_be.value+"<BR>";
                ndc=ndc+"Distance à l'appui : "+champ_input_x.value+"<BR>";
                ndc=ndc+"Coefficient kn : "+champ_txt_kn.value+"<BR>";
                ndc=ndc+"Pente i : "+champ_txt_i.value+"<BR>";
                ndc=ndc+"Rapport des hauteurs α : "+champ_txt_alpha.value+"<BR>";
                ndc=ndc+"Coefficient d'entaille kv : "+champ_txt_kv.value+"<BR>";
                ndc=ndc+"Coefficient de fissuration kcr : "+champ_input_kcr.value+"<BR>";
                ndc=ndc+"Contrainte de cisaillement : "+champ_txt_contrainte_cisaillement.value+"<BR>";
                ndc=ndc+"Résistance au cisaillement : "+champ_txt_resistance_cisaillement.value+"<BR>";
            }

        }

        RowLayout {
            Layout.preferredHeight: 30

            Rectangle {
                Layout.preferredHeight: 30
                Layout.fillWidth: true
                Layout.fillHeight: false
                Text {
                    text: "Vérification du cisaillement aux droit des appuis"
                    font.bold: true
                    font.underline: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    font.pixelSize: 18
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {//colonne de saisie des données géométriques de l'entaille
                Layout.fillWidth: true

                // Rectangle de saisie des données géométriques de l'entaille
                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    GroupBox {

                        title: "Dimensions de l'entaille"
                        font.bold: true
                        Layout.fillWidth: true
                        anchors.fill: parent
                        Layout.fillHeight: true
                        ColumnLayout {
                            anchors.fill: parent
                            Item {
                                Layout.preferredHeight: 10
                                Layout.fillWidth: true
                            }

                            RowLayout {
                                Text {text: "Type d'entaille :"}
//Combo de choix du type d'entaille
                                ComboBox {
                                    id:combo_type_entaille
                                    font.pointSize: 8
                                    Layout.preferredWidth: 250
                                    Layout.fillWidth: false
                                    model : ["Pas d'entaille","Entaille sur la face d'appui","Entaille sur la face opposée à l'appui"]
                                    onCurrentIndexChanged:{
                                        switch(currentIndex){
                                        case 0:
                                            verif_entaille.is_entaille_haut=true;
                                            break;
                                        case 1:
                                            verif_entaille.is_entaille_haut=false;
                                            image_entaille.source="qrc:/Images/entaille_basse.png"
                                            break;
                                        case 2:
                                            verif_entaille.is_entaille_haut=true;
                                            image_entaille.source="qrc:/Images/entaille_haute.png";
                                            break;
                                        }
                                        calc_ndc()
                                    }
                                }
                            }

//Image de l'entaille suivant le cas
                            Image {
                                id: image_entaille
                                visible: combo_type_entaille.currentIndex == 0 ? false :true
                                Layout.preferredWidth: 200
                                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                                Layout.fillWidth: true
                                Layout.preferredHeight: 200
                                fillMode: Image.PreserveAspectFit
                            }                            

                            Ligne_Champs{
                                id:champ_input_he
                                visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                value_nom: "Hauteur de l'entaille he :"
                                value:(combo_type_entaille.currentIndex) ==0 ? "0" :"40"
                                value_unite: "mm"
                            }

                            Ligne_Champs{
                                id:champ_input_be
                                visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                value_nom: "Largeur de l'entaille be :"
                                value:(combo_type_entaille.currentIndex) ==0 ? "0" :"40"
                                value_unite: "mm"
                            }

                            Ligne_Champs{
                                id:champ_input_x
                                visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                value_nom: "Distance à l'appui x :"
                                value:(combo_type_entaille.currentIndex) ==0 ? "0" :"40"
                                value_unite: "mm"
                            }

                            Ligne_Champs{
                                id:champ_input_effort_cisaillement
                               // visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                value_nom: "Effort tranchant Vd :"
                                value:"10"
                                value_unite: "kN"
                            }
                            Item{Layout.fillHeight: true}                            
                        }
                    }
                }
            }

///COLONNE RESULTATS


            ColumnLayout {//colonne comprenant les coeff de calcul
                Layout.fillWidth: true

                Rectangle {
                    color: "#ffffff"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent

                        GroupBox {//Groupbox de coeff de cisaillement et d'entaille
                            font.bold: true
                            title: "Vérification de l'entaille au cisaillement :"
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                            Layout.preferredHeight: 250
                            Layout.fillWidth: true
                            ColumnLayout {
                                 anchors.fill: parent
                                spacing: 8
                                Item {
                                    Layout.preferredHeight: 5
                                    Layout.fillWidth: true
                                }

                                Ligne_Champs{
                                    id:champ_txt_i
                                    visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                    value_nom: "Pente de l'entaille i(rapport b/h) :"
                                    value_isEditable: false
                                    value: formule.getEntaillePente_i(champ_input_he.value,champ_input_be.value).toFixed(2)

                                }

                                Ligne_Champs{
                                    id:champ_txt_alpha
                                    visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                    value_nom: "Rapport des hauteurs α :"
                                    value_isEditable: false
                                    value: formule.getEntaille_alpha(hBarre,champ_input_he.value).toFixed(2)

                                }

                                Ligne_Champs{
                                    id:champ_txt_kn
                                    visible: (combo_type_entaille.currentIndex) ==0 ? false : true
                                    value_nom: "kn(coeff on sait pas ce que c'est) :"
                                    value_isEditable: false
                                    value: formule.getEntaille_kn(materiau)
                                }

                                Ligne_Champs{
                                    id:champ_txt_kv
                                    value_nom: "Facteur de réduction kv :"
                                    value_isEditable: false
                                    value:(combo_type_entaille.currentIndex) == 1 ? formule.getEntaille_kv(hBarre,champ_txt_i.value,champ_txt_alpha.value,champ_txt_kn.value,champ_input_x.value,verif_entaille.is_entaille_haut).toFixed(2) : 1
                                }

                                Ligne_Champs{
                                    id:champ_input_kcr
                                    value_mask: "9.99"
                                    value_nom: "Facteur de fissuration kcr :"
                                    value: "0.67"
                                }
                                Item {
                                    Layout.preferredHeight: 20
                                    Layout.fillWidth: true
                                }

                            }
                        }

                        ColumnLayout{
                            GroupBox {//Groupbox de vérification

                                font.bold: true
                                title: "Vérification :"
                                Layout.fillWidth: true
                                RowLayout {
                                    anchors.fill: parent
                                    ColumnLayout {
                                        Layout.fillHeight: true

                                        Ligne_Champs{
                                            id:champ_txt_contrainte_cisaillement
                                            value_nom: "Contrainte de cisaillement :"
                                            value_unite: "MPa"
                                            value_isEditable: false
                                            value: formule.getContrainteCisaillement(champ_input_effort_cisaillement.value * 100.0,champ_input_kcr.value,verif_entaille.bBarre,verif_entaille.hBarre-champ_input_he.value).toFixed(2)
                                        }

                                        Ligne_Champs{
                                            id:champ_txt_resistance_cisaillement
                                            value_nom: "Résistance au cisaillement:"
                                            value_unite: "MPa"
                                            value_isEditable: false
                                            value: formule.getResistanceCisaillement(formule.getfvk(classe_bois),kmod,gamma_m).toFixed(2)
                                        }
                                        Ligne_Champs{
                                            id:champ_txt_taux_cisaillement
                                            value_nom: "Taux de travail η :"
                                            value_isEditable: false
                                            value_isBold: true
                                            value: (formule.getTauxtravail(champ_txt_contrainte_cisaillement.value,champ_txt_resistance_cisaillement.value)/champ_txt_kv.value).toFixed(2)
                                            value_unite: "%"
                                            onValue_todoubleChanged:   {
                                                if (value_todouble >=100){value_color="red";}
                                                else {value_color="green";}
                                                calc_ndc()
                                            }
                                        }
                                    }                                  
                                }
                            }
                            Item{Layout.fillHeight: true}
                        }
                    }
                }
            }
        }
    }
