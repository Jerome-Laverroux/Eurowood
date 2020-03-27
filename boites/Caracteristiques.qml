import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../accessoires"


GroupBox{
    SystemPalette{id:palette;colorGroup: SystemPalette.Active}

    label: Style_h2{text:"Caractéristiques"}

    property double gammaM: txt_gamma_M.value
    property double kmod: txt_kmod.value
    property string materiau: combo_materiau.currentText
    property string classe_bois:combo_classe_bois.currentText
    property string classe_service: combo_classe_service.currentText
    property string classe_duree: combo_classe_duree.currentText
    property string ndc:""
    onGammaMChanged: calc()
    onKmodChanged: calc()
    onMateriauChanged: calc()
    onClasse_boisChanged: calc()
    onClasse_dureeChanged: calc()
    onClasse_serviceChanged: calc()


  function calc(){
      classe_bois=combo_classe_bois.currentText
      classe_service=combo_classe_service.currentText
      classe_duree=combo_classe_duree.currentText
      materiau=combo_materiau.currentText
      kmod=formule.getkmod(classe_service,classe_duree);
      gammaM=formule.getgammaM(materiau);
      txt_gamma_M.value=gammaM;
      txt_kmod.value=kmod;

      // Sortie sur note de calculs

      ndc=""
      ndc="<H3>Caractéristiques de la barre :</H3><BR>";
      ndc=ndc+"Matériau :" +materiau+"<BR>";
      ndc=ndc+"Classe :"+classe_bois+"<BR>";
      ndc=ndc+"Classe de service :"+classe_service+"<BR>";
      ndc=ndc+"Clase de durée du chargement :"+classe_duree+"<BR>";
      ndc=ndc+"kmod = "+kmod+"<BR>";
      ndc=ndc+"gamma M ="+gammaM+"<BR>";
  }

  ColumnLayout{
      anchors.fill: parent

//Matériau
      RowLayout{
          Layout.fillWidth: true
          Style_txt {text:"Matériau"}
          Item{Layout.fillWidth: true}

          ComboBox{
              id:combo_materiau
              spacing: 10.00
           model:{["Bois massif","Lamellé-collé","LVL"]}
           onCurrentTextChanged: {

               switch (currentIndex){
               case 0:
                   combo_classe_bois.model=formule.getBDD_Noms("caracBois","Classe","C")
                   break;
               case 1:
                   combo_classe_bois.model=formule.getBDD_Noms("caracBois","Classe","GL")
                   console.log("index=" + currentIndex)
                   break;
               case 2:
                   combo_classe_bois.model=formule.getBDD_Noms("caracBois","Classe","LVL")
                   break;
               }
               calc();
           }

          }
      }
//classe de bois
      RowLayout{
          Layout.fillWidth: true
          Style_txt {text:"Classe de bois"}
          Item{Layout.fillWidth: true}

          ComboBox{
              id:combo_classe_bois
              model:{formule.getBDD_Noms("caracBois","Classe","C")}
              onCurrentTextChanged: calc();
          }
      }
//classe de service
      RowLayout{
          Layout.fillWidth: true
          Style_txt {text:"Classe de service"}
          Item{Layout.fillWidth: true}
          ComboBox{
              id:combo_classe_service
              model:{["classe_1","classe_2","classe_3"]}
              onCurrentTextChanged: calc();
          }
      }

//classe de durée
      RowLayout{
          Layout.fillWidth: true
          Style_txt {text:"Classe de durée"}
          Item{Layout.fillWidth: true}

          ComboBox{
              Layout.fillWidth: true
              id:combo_classe_duree
              model:{formule.getBDD_Noms("kmod","Classe_duree")}
              onCurrentTextChanged: calc();
          }
      }

      Ligne_Champs{
          id:txt_kmod
          value_nom: "kmod"
          value_isEditable: false
      }

      Ligne_Champs{
          id:txt_gamma_M
          value_nom: "gamma M"
          value_isEditable: false
      }

  }

}



