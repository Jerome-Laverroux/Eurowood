import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../accessoires"



GroupBox{
    id:box_def
    SystemPalette{id:palette;colorGroup: SystemPalette.Active}
    label: Style_h2{text:"Définition de la barre"}

    property bool flexion: false
    property string hBarre//:champ_hauteur.value
    property string bBarre//:champ_largeur.value
    property string lBarre
    property string bande_chargement:champ_bande_chargement.value
    property bool showAntiDevers: false
    property string ndc:""
    property int nbreappuis: 2
    property bool values_changed: false

    onHBarreChanged: champ_hauteur.value=hBarre
    onBBarreChanged: champ_largeur.value=bBarre

    function getLgBarres(){
        var tableau=[];
        tableau[0]=champ_lg1.value;
        if(champ_lg2.value !== ""){tableau[1]=champ_lg2.value}
        if(champ_lg3.value !== ""){tableau[2]=champ_lg3.value}
        return tableau;
    }

    function calc_ndc(){
        ndc=""
        ndc="<H3>Géométrie de la barre:</H3><BR>";
        ndc=ndc+"Hauteur : " +hBarre+"<BR>";
        ndc=ndc+"Largeur : "+bBarre+"<BR>";
        ndc=ndc+"Longueur : "+lBarre+"<BR>";
        ndc=ndc+"Bande de chargement : "+bande_chargement+"<BR>";
      /* if(avecPaf.checked){ndc=ndc+"Porte à faux droit : "+pafD+"<BR>"
            ndc=ndc+"Porte à faux gauche : "+pafG+"<BR>";}*/

    }

    ColumnLayout{
        anchors.fill: parent

        Ligne_Champs{
            id:champ_largeur
            value_nom: "Largeur"
            value_unite: "mm"
            value_decimales: 0
            onValueChanged: {bBarre=value;calc_ndc();values_changed=true;}
            }

        Ligne_Champs{
            id:champ_hauteur
            value_nom: "Hauteur"
            value_unite: "mm"
            value_decimales: 0
            onValueChanged:{ hBarre=value;calc_ndc();values_changed=true;}
        }

        ColumnLayout{
            Layout.fillWidth: true
            CheckBox{
                id:chck_inertieV
                text: "inertie variable"
                enabled: false
                checked: false
                onCheckStateChanged: {
                    if(checked){champ_hauteur_2.visible=true;}
                    else{champ_hauteur_2.visible=false;}
                }
            }

            Ligne_Champs{
                id:champ_hauteur_2
                visible: false
                Layout.fillWidth: true
                value_nom: "Hauteur h2:"
                value_unite: "mm"
                value: "300"
                value_decimales: 0
                onValueChanged: {h2Barre=value;calc_ndc()}
            }
        }

        Ligne_Champs{
            id:champ_bande_chargement
            value_nom: "Bande de chargement"
            value_unite: "mm"
            value: "500"
            value_decimales: 0
            onValueChanged: {bande_chargement=value;calc_ndc()}
        }

        RowLayout{
            id:row_nbre_appuis
            visible: box_def.flexion? true :false
            Layout.fillWidth: true
            Style_txt{text:"Nombre d'appuis"}
            ComboBox{
                id:combo_nbre_app
                Component.onCompleted:{
                    nbreappuis=model[currentIndex]
                    currentIndex=find(nbreappuis)
                }

                model:["2","3","4"]
                //currentIndex: 2
                onCurrentIndexChanged: {
                    box_def.values_changed=true;
                    nbreappuis=model[currentIndex]
                    switch(currentIndex){
                    case 0:
                        champ_lg2.value=""
                        champ_lg3.value=""
                        break;
                    case 1:
                        champ_lg2.value=""
                        break;
                    }
                }
            }
        }

        Ligne_Champs{
            id:champ_lg1
            value_nom: "Lg barre 1"
            value_unite: "mm"
            value: "6000"
            value_decimales: 0
            onValueChanged: {lBarre=value;calc_ndc();box_def.values_changed=true;}
        }

        Ligne_Champs{
            id:champ_lg2
            visible: combo_nbre_app.currentIndex>0 ? true :false
            value_nom: "Lg barre 2"
            value: "10000"
            value_unite: "mm"
            value_decimales: 0
            onValueChanged: {box_def.values_changed=true;}
        }
        Ligne_Champs{
            id:champ_lg3
            visible: combo_nbre_app.currentIndex>1 ? true :false
            value_nom: "Lg barre 3"
            value: "6000"
            value_unite: "mm"
            value_decimales: 0
            onValueChanged: {box_def.values_changed=true;}
        }

        RowLayout{
            visible: showAntiDevers
            Layout.fillWidth: true
           // Layout.preferredHeight: reglages.defaut_hauteur_champs
            Text {text:"Anti-devers"}
            SpinBox{
                id:spin_devers
                height: 5
            }
            Item{Layout.fillWidth: true}
        }

        /*
        CheckBox{
            id:avecPaf
            checked: false
            text: "Barre avec porte à faux"
            onCheckedChanged: calc_ndc()
        }

        Ligne_Champs{
            id:champ_pafG
            visible: avecPaf.checked ? true : false
            value_nom: "Porte à faux gauche"
            value_unite: "mm"
            value_decimales: 0
            value: "0"
            onValueChanged: {pafG=value;calc_ndc()}
        }
        Ligne_Champs{
            id:champ_pafD
            visible: avecPaf.checked ? true : false
            value_nom: "Porte à faux droite"
            value_unite: "mm"
            value_decimales: 0
            value: "0"
            onValueChanged: {pafD=value;calc_ndc()}
        }


        RowLayout{
            visible: showKsys
        Reglage_defaut{id:reglages}    Layout.fillWidth: true
            CheckBox{id:chck_ksys}
            Text {text:"Plusieurs éléments côte à côte(ksys)"}
            Item{Layout.fillWidth: true}

        }
        RowLayout{
            visible: showAntiDevers
            Layout.fillWidth: true
           // Layout.preferredHeight: reglages.defaut_hauteur_champs
            Text {text:"Anti-devers"}
            SpinBox{
                id:spin_devers
                height: 5
            }

            Item{Layout.fillWidth: true}
        }
*/


    }
}


/****************vestige d'un groupbox qui ne marchait pas grrrrrr!!!!!**********************
/*GroupBox{

    Reglage_defaut{id:reglages}
    title: "Définition de la barre" 
    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
    property string hBarre//:champ_hauteur.value
    property string h2Barre
    property string bBarre//:champ_largeur.value
    property string pafG: champ_pafG.value
    property string lBarre
    property string pafD: champ_pafD.value
    property string bande_chargement:champ_bande_chargement.value
    property bool showInertie: false
    property bool showKsys: false
    property bool showAntiDevers: false
    property bool flexion: false
    property string ndc:""



    function calc_ndc(){
        ndc=""
        ndc="<H3>Géométrie de la barre:</H3><BR>";
        ndc=ndc+"Hauteur : " +hBarre+"<BR>";
        ndc=ndc+"Largeur : "+bBarre+"<BR>";
        ndc=ndc+"Longueur : "+lBarre+"<BR>";
        ndc=ndc+"Bande de chargement : "+bande_chargement+"<BR>";
        if(avecPaf.checked){ndc=ndc+"Porte à faux droit : "+pafD+"<BR>"
            ndc=ndc+"Porte à faux gauche : "+pafG+"<BR>";}

    }




    ColumnLayout{
        anchors.fill: parent




/*


        CheckBox{
            id:avecPaf
            checked: false
            text: "Barre avec porte à faux"
            onCheckedChanged: calc_ndc()
        }

        Ligne_Champs{
            id:champ_pafG
            visible: avecPaf.checked ? true : false
            value_nom: "Porte à faux gauche"
            value_unite: "mm"
            value_decimales: 0
            value: "0"
            onValueChanged: {pafG=value;calc_ndc()}
        }
        Ligne_Champs{
            id:champ_pafD
            visible: avecPaf.checked ? true : false
            value_nom: "Porte à faux droite"
            value_unite: "mm"
            value_decimales: 0
            value: "0"
            onValueChanged: {pafD=value;calc_ndc()}
        }


        RowLayout{
            visible: showKsys
        Reglage_defaut{id:reglages}    Layout.fillWidth: true
            CheckBox{id:chck_ksys}
            Text {text:"Plusieurs éléments côte à côte(ksys)"}
            Item{Layout.fillWidth: true}

        }
        RowLayout{
            visible: showAntiDevers
            Layout.fillWidth: true
           // Layout.preferredHeight: reglages.defaut_hauteur_champs
            Text {text:"Anti-devers"}
            SpinBox{
                id:spin_devers
                height: 5
            }

            Item{Layout.fillWidth: true}
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}*/
