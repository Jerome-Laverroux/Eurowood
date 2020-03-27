import QtQuick 2.13
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
//import Qt3D.Core 2.12
//import Qt3D.Render 2.12
//import Qt3D.Input 2.12
//import Qt3D.Extras 2.12
import QtQuick.Scene3D 2.0
import enums.Ch 1.0
import enums.Categories 1.0
import com.modele 1.0
import "../boites"
import "../accessoires"

Item{
    id:chef
    anchors.fill: parent

    onWidthChanged:{
        if(width > 550){
            gridbox1.columns=2}
        else{gridbox1.columns=1}
    }
    property string lBarre:"4500"
    property bool modelecree: false
    property int nbreapp:2
    property Barre barre: null
    property var charge_uniforme:{"categorie":"","q":""}




    onModelecreeChanged: {
        rendu.requestPaint();
        //rendu3d.barre=[def_barre.bBarre,def_barre.hBarre,modele.getLgBarre(1)]
        def_barre.values_changed=false;
    }

    function creerModele(){
        modele.quickBarre(def_barre.getLgBarres(),def_barre.bBarre,def_barre.hBarre,carac.materiau,carac.classe_bois,carac.classe_service,carac.classe_duree);               
        modelecree=true;
        barre=modele.getBarre(1);
        rendu.lgtotalebarre=barre.getLg();
        barre.clearChargements();
        charges.b=barre;
    }

    function chargerBarre(){
        modele.setCombinaisons();
        noms_combis_ELU.model=barre.getNoms_Combi_ELU();
    }

    function calculer(){
        barre.setChargements(false,noms_combis_ELU.currentIndex);///calculs aux ELU
      //  barre.setChargements(true,noms_combis_ELS.currentIndex);///calculs aux ELS
        barre.getContrainteFlexion(1,1);
        rendu.requestPaint();
    }

    Reglage_defaut {id : reglages}
    RowLayout{
        id:principal
        anchors.fill: parent
        anchors.margins: 10

        SwipeView{
            id:swipe_saisies
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView{
                id:scroll1
                contentWidth: -1
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

                GridLayout{
                    id:gridbox1
                    anchors.fill: parent


                   Style_h1{
                       Layout.fillWidth: true
                       Layout.columnSpan:gridbox1.columns>-1?gridbox1.columns:1
                       horizontalAlignment: Text.AlignHCenter
                       text: "Barre en flexion"
                   }

                   CheckBox{
                       id:chckAppuis
                       Layout.columnSpan: gridbox1.columns>-1?gridbox1.columns:1
                       text: "Vérification des appuis"
                       checked: false
                   }

                   Caracteristiques{
                        id:carac
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop
                    }

                    DefinitionBarre{
                        id:def_barre
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop
                        bBarre: "100"
                        hBarre: "220"
                        flexion:true

                        onNbreappuisChanged: chef.nbreapp=def_barre.nbreappuis
                        onValues_changedChanged: if(values_changed){ modelecree=false;}
                    }
                    RowLayout{
                        Layout.fillWidth: true
                        Layout.columnSpan: gridbox1.columns>-1?gridbox1.columns:1
                        Button{
                            text: "Créer le modèle"
                            onClicked: {creerModele();}
                        }
                        Text {
                            id: txt_mod_cree
                            text: modelecree ?"Modèle prêt": "Modèle non créé"
                            color: modelecree ?"green" :"red"
                        }

                    }

    /////////VERIFICATION APPUIS

                    Verif_appuis{
                        id:boite_verif_appuis
                        visible: chckAppuis.checked
                        bBarre:def_barre.bBarre
                        hBarre:def_barre.hBarre
                        materiau:carac.materiau
                        classe_Bois:carac.classe_bois
                        kmod:carac.kmod
                        gamma_M:carac.gammaM
                    }

    ////VERIFICATION ENTAILLE

                    Entaille{
                        id:boite_entaille
                        visible: chckAppuis.checked
                       // Layout.topMargin: 3
                        hBarre: def_barre.hBarre
                        bBarre: def_barre.bBarre
                        materiau: carac.materiau
                        classe_bois:carac.classe_bois
                        kmod: carac.kmod
                        gamma_m: carac.gammaM
                    }



                    Item{Layout.fillHeight: true}
                }
            }
            ScrollView{
                id:scroll2
                contentWidth: -1
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

                GridLayout{
                    id:gridbox2
                    anchors.fill: parent
                    columns: cfg.isAndro()? 1: 2
                    flow: GridLayout.TopToBottom


                    Chargements{
                        id:charges
                        enabled: modelecree ? true : false
                        onChargementOkChanged:{
                            if(chargementOk){
                                console.log("Chargements terminés")
                               barre=b;
                               chargerBarre();
                           }
                        }
                    }
                }
            }
            ScrollView{
                id:scroll3
                contentWidth: -1
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

                GridLayout{
                    id:gridbox3
                    anchors.fill: parent
                    columns: cfg.isAndro()? 1: 2
                  //  flow: GridLayout.TopToBottom
                    ColumnLayout{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 600
                        Button{
                            Layout.preferredHeight: 30
                            Layout.preferredWidth: 100
                            text:"Actualiser"
                            onClicked: {rendu.requestPaint();}
                        }
                        Ligne_Champs{
                            id:val_ech_moments
                            value_nom:"ech moments x"
                            value:"1"
                            onValueChanged: rendu.requestPaint();
                        }
                        Ligne_Champs{
                            id:val_ech_tranchants
                            value_nom:"ech efforts tranchants x"
                            value:"1"
                            onValueChanged: rendu.requestPaint();
                        }
                        ComboBox{
                           id: noms_combis_ELU
                           Layout.fillWidth: true
                           onCurrentIndexChanged: {calculer();

                              // console.log("sigma m,d",modele.getcontrainteFlexion(1,currentIndex).toFixed(2), "MPa")
                           }
                        }

                      /*  Scene3D {
                                    id: scene3d
                                    Layout.fillWidth:true
                                    Layout.preferredHeight: 250
                                    focus: true
                                    aspects: ["input", "logic"]
                                    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

                                    Rendu {
                                        id:rendu3d
                                    }
                                }*/


                        Canvas{
                            id:rendu
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            property double lgtotalebarre:1
                            renderStrategy: Canvas.Cooperative
                            renderTarget: Canvas.FramebufferObject
                            onWidthChanged:{ markDirty();}
                            onCanvasSizeChanged: markDirty()
                            property variant ctx

                            onPainted:{console.log("paint terminé")}

                            onPaint: {

                                console.log("painting");                               

                                ctx= rendu.getContext("2d");
                                ctx.reset();
                                ctx.fillStyle = 'white'
                                ctx.fillRect(0, 0, width, height)



                                var largeur=rendu.width;
                                var hauteur=rendu.height;

                                //définition des marges et de l'échelle
                                var hAppuis=10;
                                var zeroX=25;
                                var zeroY=hauteur/2;
                                var echelle=1/(lgtotalebarre/(rendu.width-(zeroX*2)));
                                var echeff=100*val_ech_tranchants.value*echelle;
                               // var mutl=0;mult=val_ech_moments
                                var echmom=(1/5000)*val_ech_moments.value*echelle;
                                var lgbarre=lgtotalebarre*echelle;

                                if(modelecree){

                                    ///dessin de la barre
                                    ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);
                                    ctx.lineWidth = 1;
                                   // ctx.beginPath();
                                    ctx.moveTo(zeroX,zeroY);
                                    ctx.lineTo(zeroX+lgbarre,zeroY);
                                    ctx.stroke();

                                    ///dimension de la barre
                                    ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);
                                    ctx.moveTo(zeroX,zeroY);
                                    var txtlg="";
                                    txtlg=lgtotalebarre+"mm";
                                    ctx.text(txtlg,zeroX+(lgbarre/2),zeroY-20);
                                    ctx.stroke();



                                    var numbarre=1;
                                    var nbrenoeuds=barre.getNbreNoeuds()
                                    var coordX=0;
                                    var coordY=0;

                                    for(var i=0 ; i<nbrenoeuds;i++)
                                    {
                                        var txt="";txt=i+1;///cast de i en texte
                                        coordX=zeroX+(barre.getNoeud_X(i+1))*echelle;
                                        coordY=zeroY+barre.getNoeud_Y(i+1)*echelle;

                                        ///numérotation des noeuds
                                        ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);
                                        ctx.moveTo(zeroX,zeroY);
                                        ctx.text(txt,coordX,coordY-10);
                                        ctx.stroke();

                                        ///dessin des appuis
                                        ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);
                                        ctx.moveTo(zeroX,zeroY);
                                        ctx.lineWidth = 1;
                                        ctx.beginPath();
                                        ctx.moveTo(coordX,coordY);
                                        ctx.lineTo(coordX-hAppuis,coordY+hAppuis);
                                        ctx.lineTo(coordX+hAppuis,coordY+hAppuis);
                                        ctx.lineTo(coordX,coordY);
                                        ctx.closePath();                                       
                                        ctx.stroke();
                                    }


                                    ///dessin des efforts tranchants
                                  /////////standby 3 appuis/////////
                                    var segments=0;
                                    var valmoment;
                                    var valefftran;
                                    if(chef.nbreapp==2)///barre sur 2 appuis
                                    {
                                        segments=barre.getPrecision();
                                        valmoment=barre.getTabMoments();
                                    }
                                    else{
                                        segments=barre.getNbreSegments();
                                        valmoment=barre.getMomentsHyperS();
                                        echmom=echmom*1000000;
                                        valefftran=barre.getEffTranchantHyperS();
                                    }



                                    var lgSegment=lgbarre/segments;


                                    coordX=zeroX;
                                    coordY=zeroY;

                                    if(chef.nbreapp!==2)
                                    {

                                        ctx.strokeStyle = Qt.rgba(1, 0, 0, 0.5);
                                        ctx.lineWidth = 1;
                                        ctx.moveTo(coordX,coordY);
                                        ctx.beginPath();
                                        //traçage effort tranchant
                                        for(var i=0 ; i<segments;i++)
                                        {

                                            ctx.lineTo(coordX,coordY+(valefftran[i]*echeff));
                                           // console.log(valefftran[i]*echeff)
                                            coordX=coordX+lgSegment;
                                        }
                                        ctx.lineTo(zeroX+lgbarre,zeroY);
                                        ctx.closePath();
                                        ctx.fillStyle = Qt.rgba(1,0, 0, 0.5);
                                        ctx.fill();
                                        ctx.stroke();

                                    }



                                    coordX=zeroX;
                                    coordY=zeroY;
                                    ctx.strokeStyle = Qt.rgba(0,1, 0, 0.5);
                                    ctx.lineWidth = 1;
                                    ctx.moveTo(coordX,coordY);
                                    ctx.beginPath();

                                    //traçage des moments
                                    for(var i=0 ; i<segments;i++)
                                    {
                                        ctx.lineTo(coordX,coordY+(valmoment[i]*echmom));
                                        coordX=coordX+lgSegment;
                                    }
                                    ctx.lineTo(zeroX+lgbarre,zeroY);

                                    ctx.clip();
                                    ctx.closePath();
                                    ctx.fillStyle = Qt.rgba(0,1, 0, 0.5);
                                    ctx.fill();
                                    ctx.stroke();

                                }
                            }
                        }
                    }
                }
            }


         /*   ScrollView{
                id:scroll4
                contentWidth: -1
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

                ColumnLayout{
                    anchors.fill: parent

    /////////VERIFICATION APPUIS

                    Verif_appuis{
                        id:boite_verif_appuis
                        bBarre:def_barre.bBarre
                        hBarre:def_barre.hBarre
                        materiau:carac.materiau
                        classe_Bois:carac.classe_bois
                        kmod:carac.kmod
                        gamma_M:carac.gammaM
                    }

    ////VERIFICATION ENTAILLE

                    Entaille{
                        id:boite_entaille
                        Layout.topMargin: 3
                        hBarre: def_barre.hBarre
                        bBarre: def_barre.bBarre
                        materiau: carac.materiau
                        classe_bois:carac.classe_bois
                        kmod: carac.kmod
                        gamma_m: carac.gammaM
                    }
                }
            }*/

        ScrollView{
            id:scroll5
            contentWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy:ScrollBar.AlwaysOff

                ////VERIFICATION Vibration plancher

            Vibration_plancher{
                bBarre: def_barre.bBarre
                hBarre: def_barre.hBarre
                lBarre: def_barre.lBarre
                bandeChargement:def_barre.bande_chargement
            }
        }

    }
        PageIndicator {
            id: indicator
            parent:swipe_saisies
            count: swipe_saisies.count
               currentIndex: swipe_saisies.currentIndex
               anchors.bottom: parent.bottom
               anchors.horizontalCenter: parent.horizontalCenter
        }
      /*  Volet_Resultats{
            id:volet_resultats
            visible: cfg.isAndro()? false : true
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClickedChanged: {
                // Texte de base
                remplir="<H2 align=\"center\">Vérification d'une barre en flexion</H2><BR> "
            }
        }*/
    }
}
