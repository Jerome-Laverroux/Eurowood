import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import enums.Categories 1.0

import "../accessoires"



GroupBox{
    id:chefgroupe
    property string nomCharge
    property string lBarre
    property bool checked: false
    property bool categorie_expl_enabled: false
    property bool categorie_neige_enabled: false
    property string value_unif:"0"
    property string value_ponct:"0"
    property string value_ponct_abs:"0"
    property int value_categorie:Categorie.Null
    property string value_ndc

    label: CheckBox{
        text:nomCharge;
        checked: chefgroupe.checked? true:false
        onCheckedChanged:{
            chck_ponct.checked=false;
            chefgroupe.checked=checked
            if(combo_Categories.count>1){combo_Categories.currentIndex=0}
            if(!checked){
                uniforme.value=""
                ponctuelle.value=""
                ponctuelle_coord.value=""
            }
        }
    }


    function ndc(){

      /*  var out=""
       out="<H4>"+nomCharge+"</H4><BR>";
        var cast=0.0;
        cast=uniforme.value_out;
        if(cast > 0){
            out=out+"Charges uniformes réparties :"+chefgroupe.value_unif+" kN/m²<BR>";
        }
        if(chck_ponct.checked){out=out+"Charge ponctuelle :"+ponctuelle.value_out+" kN à "+ponctuelle_coord.value_out+" mm<BR>"}
        chefgroupe.value_ndc=out;
        console.log("Chargements :"+ out)*/
    }


    title: nomCharge
    Layout.fillWidth: true
    ColumnLayout{

        anchors.fill: parent

        ComboBox{
            id:combo_Categories
            Layout.fillWidth: true
            visible: ((categorie_expl_enabled|| categorie_neige_enabled) && chefgroupe.checked)?true:false
            currentIndex: -1
            model:{

                if(categorie_expl_enabled){formule.getBDD_Noms("Psi","Categorie","Categorie")}
                else if(categorie_neige_enabled){formule.getBDD_Noms("Psi","Categorie","Alt.")}
            }
            ///bourin mais ca marche
            onCurrentIndexChanged: {
                if(model[currentIndex].indexOf("Categorie A")>-1){value_categorie=Categorie.Cat_A;}
                else if(model[currentIndex].indexOf("Categorie B")>-1){value_categorie=Categorie.Cat_B;}
                else if(model[currentIndex].indexOf("Categorie C")>-1){value_categorie=Categorie.Cat_C;}
                else if(model[currentIndex].indexOf("Categorie D")>-1){value_categorie=Categorie.Cat_D;}
                else if(model[currentIndex].indexOf("Categorie E")>-1){value_categorie=Categorie.Cat_E;}
                else if(model[currentIndex].indexOf("Categorie F")>-1){value_categorie=Categorie.Cat_F;}
                else if(model[currentIndex].indexOf("Categorie G")>-1){value_categorie=Categorie.Cat_G;}
                else if(model[currentIndex].indexOf("Categorie H")>-1){value_categorie=Categorie.Cat_H;}
                else if(model[currentIndex].indexOf("inf. 1000m")>-1){value_categorie=Categorie.Ninf1000m;}
                else if(model[currentIndex].indexOf("sup. 1000m")>-1){value_categorie=Categorie.Nsup1000m;}
            }
        }

        Ligne_Champs{
            id:uniforme
            value_largeur: 250
            visible: chefgroupe.checked? true :false
            value_nom: "Charges uniformes réparties"
            value: ""
            value_decimales: 2
            value_unite: "kN/m²"
            onValueChanged:{ value_unif=value;ndc();}
        }
        CheckBox{
            id:chck_ponct
            visible: chefgroupe.checked? true :false
            text: "avec charge ponctuelle"
            checked: false
        }
        Ligne_Champs{
            id:ponctuelle
            visible: chck_ponct.checked ? true :false
            value_nom: "Charge ponctuelle"
            value: ""
            value_unite: "kN"
            onValueChanged:{ value_ponct=value_out;ndc();}
        }
        Ligne_Champs{
            id:ponctuelle_coord
            visible: chck_ponct.checked ? true :false
            value_nom: "postition sur la barre"
            value: ""
            value_unite: "mm"
            /*
              TODO
              vérifier la longueur de la barre suivant la coordonnée indiquée
*/

            onValueChanged:{ value_ponct_abs=value_out;ndc();}
        }
    }
}
