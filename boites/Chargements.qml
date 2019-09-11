import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import enums.Ch 1.0
import enums.Categories 1.0
import com.modele 1.0
import "../accessoires"

GroupBox {
    id:chefCharg
    title: "Chargements"
    property string lBarre
    property string value_ndc
    property Barre b:null
    property bool chargementOk: false
    property var charge_uniforme:{"categorie":"","q":""}
    property var charge_poncutelle:{"categorie":"","f":"","noeud":""}
    property var tab_charges_uniformes:[{}]
    property var tab_charges_ponctuelles:[]


    onTab_charges_uniformesChanged: {console.log("tableau charge modifié")}

    onCharge_uniformeChanged: {console.log("bla bla")}

    function ndc(){

        value_ndc="<H3>Chargements</H3>"
        if(g.checked){value_ndc=value_ndc+g.value_ndc;}
        if(q.checked){value_ndc=value_ndc+q.value_ndc;}
        if(s.checked){value_ndc=value_ndc+s.value_ndc;}
        if(sAcc.checked){value_ndc=value_ndc+sAcc.value_ndc;}
        if(w1.checked){value_ndc=value_ndc+w1.value_ndc;}
        if(w2.checked){value_ndc=value_ndc+w2.value_ndc;}

        console.log("Chargements :"+ chefCharg.value_ndc)
    }

    ColumnLayout{
        anchors.fill: parent

        Button{
            id:charger
            text: "Charger"
            onClicked:{
                ///G
                b.clearChargements();
                if(g.value_unif !==""){
                    b.addChargementUniforme(NatureCharge.G,g.value_unif,500,Categorie.null,b.getNumNoeudDeb(),b.getNumNoeudFin());
                }
                if(q.value_unif !==""){
                    b.addChargementUniforme(NatureCharge.Q,q.value_unif,500,q.value_categorie,b.getNumNoeudDeb(),b.getNumNoeudFin());
                }
                if(s.value_unif !==""){
                    b.addChargementUniforme(NatureCharge.S,s.value_unif,500,s.value_categorie,b.getNumNoeudDeb(),b.getNumNoeudFin());
                }
                ///TODO gestion neige accidentelle

                if(w1.value_unif !==""){
                    b.addChargementUniforme(NatureCharge.W1,w1.value_unif,500,Categorie.Vent,b.getNumNoeudDeb(),b.getNumNoeudFin());
                }
                if(w2.value_unif !==""){
                    b.addChargementUniforme(NatureCharge.W2,w2.value_unif,500,Categorie.Vent,b.getNumNoeudDeb(),b.getNumNoeudFin());
                }


                chargementOk=true
            }
        }

        BoiteCharges{
            id:g
            nomCharge:"Charges permanentes";
            checked: true;
            lBarre: chefCharg.lBarre
            //onValue_ndcChanged: chefCharg.ndc();
            onValue_unifChanged: {chargementOk=false;

            }
        }
        BoiteCharges{
            id:q
            nomCharge:"Charges d'exploitation";
            categorie_expl_enabled: true
            lBarre: chefCharg.lBarre
            onValue_unifChanged: chargementOk=false;
            onValue_ponctChanged: chargementOk=false;
            onValue_ndcChanged: chefCharg.ndc();
        }
        BoiteCharges{
            id:s
            nomCharge:"Neige Normale"
            categorie_neige_enabled: true
            lBarre: chefCharg.lBarre
            onValue_ndcChanged: chefCharg.ndc();
        }
        BoiteCharges{
            id:sAcc
            nomCharge:"Neige Accidentelle"
            lBarre: chefCharg.lBarre
            onValue_ndcChanged: chefCharg.ndc();
        }
        BoiteCharges{
            id:w1
            nomCharge:"Vent Normal surpression"
            lBarre: chefCharg.lBarre
            onValue_ndcChanged: chefCharg.ndc();
        }
        BoiteCharges{
            id:w2
            nomCharge:"Vent Normal dépression"
            lBarre: chefCharg.lBarre
            onValue_ndcChanged: chefCharg.ndc();
        }
        Item{Layout.fillHeight: true}
    }


}
