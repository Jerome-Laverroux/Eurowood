import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import enums.Organe 1.0
import "../accessoires"
import "../boites"

Item {
    id:chef
    anchors.fill: parent
    visible: true
    property int h_defaut: 20
    property int l_champs: 50
    property double pk:Math.sqrt(formule.getMasseVolCarac(carac1.classe_bois)*formule.getMasseVolCarac(carac2.classe_bois))

    Reglage_defaut {id : reglages}

    function calc(){
        carac2.classe_bois=combo_classe_bois.currentText
        carac2.materiau=combo_materiau.currentText
    }

    function calc_geometrie(){
        if(combo_organes.currentIndex==0){
            if (champ_position.value_todouble*Math.cos(champ_beta.value*Math.PI/180)>=champ_l_appui.value_todouble){
                champ_t.value=champ_l_appui.value
                if (champ_lg.value_todouble<champ_t.value_todouble){
                    champ_t.value=champ_lg.value
                }
                champ_t1.value=champ_lg.value_todouble-champ_position.value*Math.sin(champ_beta.value*Math.PI/180)
                champ_t2.value=champ_t.value
                txt_vis_visible.visible=true
            }
            else{
                champ_t.value=(champ_position.value*Math.sin(champ_beta.value*Math.PI/180)).toFixed(2)
                if (champ_lg.value_todouble<champ_t.value_todouble){
                    champ_t.value=champ_lg.value
                }
                champ_t1.value=(champ_lg.value_todouble-champ_t2.value_todouble).toFixed(2)
                champ_t2.value=champ_t.value
                txt_vis_visible.visible=false
            }
        }
        else{
            if (champ_position.value/Math.cos(champ_beta.value*Math.PI/180)>champ_l_appui.value_todouble){
                champ_t.value=champ_bp.value
                if (champ_lg.value_todouble<champ_t.value_todouble){
                    champ_t.value=champ_lg.value
                }
                champ_t1.value=champ_lg.value_todouble-champ_position.value_todouble*Math.tan(champ_beta.value*Math.PI/180)
                champ_t2.value=champ_t.value
                txt_vis_visible.visible=true
            }
            else{
                champ_t.value=(champ_position.value_todouble*Math.tan(champ_beta.value*Math.PI/180)).toFixed(2)
                if (champ_lg.value_todouble<champ_t.value_todouble){
                    champ_t.value=champ_lg.value
                }
                champ_t1.value=(champ_lg.value_todouble-champ_t2.value_todouble).toFixed(2)
                champ_t2.value=champ_t.value
                txt_vis_visible.visible=false
            }
        }
    }

    function poteau(){
        if (champ_alpha.value==90){
            box_resistance_compression_piece1.visible=true
            box_resistance_fendage_piece1.visible=false
        }
        else{
            box_resistance_compression_piece1.visible=false
            box_resistance_fendage_piece1.visible=true
        }
    }



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
                    columns: cfg.isAndro()? 1: 2
                    flow: GridLayout.TopToBottom

                    Titre_Verif{txt:"Assemblages par tire-fond vis"}

                    GroupBox{
                        id:box_piece1
                        title:"Pièce 1"
                        Layout.fillWidth: true
                        font.bold:true

                        ColumnLayout{
                            anchors.fill:parent
                            Ligne_Champs{
                                id:champ_ha
                                value_nom: "Hauteur :"
                                value_unite: "mm"
                                value: "240"
                            }

                            Ligne_Champs{
                                id:champ_ba
                                value_nom: "Largeur :"
                                value_unite: "mm"
                                value: "120"
                            }

                            Caracteristiques{
                                id:carac1
                                Layout.preferredHeight: 250
                                Layout.fillWidth: true

                            }

                        }
                    }

                    ///definition pièce 2
                    GroupBox {
                        id:box_piece2
                        font.bold: true
                        Layout.fillWidth: true
                        title: qsTr("Pièce 2")

                        ColumnLayout {
                            anchors.fill: parent


                            Ligne_Champs{
                                id:champ_hp
                                value_nom: "Hauteur :"
                                value_unite: "mm"
                                value: "180"
                            }

                            Ligne_Champs{
                                id:champ_bp
                                value_nom: "Largeur :"
                                value_unite: "mm"
                                value: "120"
                                onValue_todoubleChanged: calc_geometrie()
                            }

                            GroupBox{
                                id:carac2
                                title:"Caractéristiques"
                                font.bold : true
                                Layout.fillWidth: true
                                property string materiau: combo_materiau.currentText
                                property string classe_bois:combo_classe_bois.currentText
                                property double gammaM:txt_gamma_M.text

                                ColumnLayout{
                                    anchors.fill:parent
                                    spacing : 30

                                    //Matériau
                                    RowLayout{
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: reglages.defaut_hauteur_champs
                                        Text {text:"Matériau"}
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
                                        Layout.preferredHeight: reglages.defaut_hauteur_champs
                                        Text {
                                            text:"Classe de bois"
                                        }
                                        Item{Layout.fillWidth: true}

                                        ComboBox{
                                            id:combo_classe_bois
                                            model:{formule.getBDD_Noms("caracBois","Classe","C")}
                                            onCurrentTextChanged: calc()
                                        }
                                    }

                                    //gamma_m
                                    RowLayout{
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: reglages.defaut_hauteur_champs
                                        Text {
                                            text:"gamma M"
                                        }
                                        Item{Layout.fillWidth: true}
                                        Text{
                                            id:txt_gamma_M
                                            horizontalAlignment: Qt.AlignHCenter
                                            text:formule.getgammaM(carac2.materiau)
                                        }
                                    }

                                }

                            }

                        }
                    }

                    GroupBox {
                        id:box_assemblage
                        font.bold: true
                        Layout.fillWidth: true
                        title: "Géométrie de l'assemblage"

                        ColumnLayout {
                            anchors.fill: parent

                            Text{
                                text: "Angles de l'assemblage :"
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Ligne_Champs{
                                id:champ_alpha
                                value_nom: "Angle de la pièce 1 \npar rapport à l'horizontal :"
                                value_unite: "°"
                                value: "30"
                                onValueChanged: poteau()
                            }

                            Ligne_Champs{
                                id:champ_beta
                                value_nom: "Angle de la pièce 2 \npar rapport à la pièce 1 :"
                                value_unite: "°"
                                value: "45"
                                onValue_todoubleChanged: calc_geometrie()
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }

                            Text{
                                text: "Positionnement des organes :"
                                font.bold: true
                            }

                            ComboBox{
                                id:combo_organes
                                model : ["Perpendiculaire pièce 1","Perpendiculaire pièce 2"]
                                Layout.fillWidth: true
                                onCurrentIndexChanged: calc_geometrie()
                            }

                            Ligne_Champs{
                                id:champ_position
                                value_nom: "Position de la vis x :"
                                value_unite: "mm"
                                value:"100"
                                onValue_todoubleChanged: calc_geometrie()
                            }

                            Text{
                                id:txt_vis_visible
                                Layout.fillWidth: true
                                text:"Attention la vis sera apparente"
                                visible:false
                                horizontalAlignment: Qt.AlignHCenter
                                color:"red"
                            }

                            Ligne_Champs{
                                id:champ_l_appui
                                value_nom: "Longueur d'appui \nsur la pièce 1 :"
                                value_unite: "mm"
                                value_isEditable: false
                                value : (champ_bp.value/(Math.sin(champ_beta.value*Math.PI/180))).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_t
                                value_nom: "Longueur d'organe \ndans la pièce 2 :"
                                value_unite: "mm"
                                value_isEditable: false
                                onValue_todoubleChanged: calc_geometrie()
                                }
                            Image{
                                source:"qrc:/Images/Image_position_organe.jpg"
                                Layout.alignment: Qt.AlignTop
                                Layout.maximumWidth:cfg.isAndro()?Screen.desktopAvailableWidth-50 : 350
                                fillMode: Image.PreserveAspectFit
                                Layout.preferredHeight: paintedHeight
                                Layout.fillWidth: cfg.isAndro()? true : false
                            }

                        }
                    }

                    GroupBox {
                        id:box_carac_organes
                        title: "Organes :"
                        font.bold: true
                        //property double largeur: 250
                        Layout.fillWidth: true

                        ColumnLayout{

                            Text{
                                text:"Caractéristiques des organes"
                                font.bold: true
                            }

                            RowLayout{
                                Text{text:"Qualité de l'acier"}
                                ComboBox{
                                    id:combo_qualite
                                    model:["4.6","4.8","5.6","5.8","6.8","8.8","10.9"]
                                }
                            }

                            Ligne_Champs{
                                id:champ_lg
                                value_nom: "Longueur de l'organe :"
                                value_unite: "mm"
                                value: "160"
                                onValue_todoubleChanged: calc_geometrie()
                            }

                            Text{
                                id:txt_erreur_t1
                                color:"red"
                                text:"L'organe est trop court et n'atteint pas la pièce 1"
                                visible: false
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Ligne_Champs{
                                id:champ_d
                                value_nom: "Diamètre de l'organe :"
                                value_unite: "mm"
                                value: "6"
                            }


                            Ligne_Champs{
                                id:champ_fu
                                value_nom: "fu ="
                                value_unite: "MPa"
                                value_isEditable: false
                                value : 100*Math.trunc(combo_qualite.currentText)

                            }

                            Ligne_Champs{
                                id:champ_fy
                                value_nom: "fy ="
                                value_unite: "MPa"
                                value_isEditable: false
                                value : combo_qualite.currentText.split(".")[1]*champ_fu.value/10
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }

                            Text{
                                text:"Disposition des organes :"
                                font.bold: true
                            }

                            Ligne_Champs{
                                id:champ_nbr_ligne
                                value_nom: "Nombre de lignes :"
                                value_unite: ""
                                value: "3"
                            }

                            Ligne_Champs{
                                id:champ_nbr_colonne
                                value_nom: "Nombre de colonnes :"
                                value_unite: ""
                                value: "1"
                            }

                            Ligne_Champs{
                                id:champ_distance_horizontal
                                value_nom: "Distance d1 (\u2265 a1):"
                                value_unite: "mm"
                                value: "80"
                            }

                            Ligne_Champs{
                                id:champ_distance_vertical
                                value_nom: "Distance d2 (\u2265 a2) :"
                                value_unite: "mm"
                                value: "40"
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 10
                            }

                            Text{
                                text:"Conditions de pince :"
                                font.bold: true
                            }
                            GridLayout{
                                id:grid_pinces
                                property double largeur: 10
                                columns: 3
                                rows: 3
                                Ligne_Champs{
                                    id:champ_a1
                                    //value_largeur: grid_pinces.largeur
                                    value_nom: "a1 ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value : formule.getPince_Tirefonds_a1(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois),champ_alpha.value).toFixed(0)
                                    value_color:{
                                        if(champ_a1.value_todouble<=champ_distance_horizontal.value_todouble){
                                            "green"
                                        }
                                        else{"red"}

                                    }
                                }
                                Rectangle{
                                    Layout.rowSpan: 3
                                    color:"black"
                                    Layout.preferredWidth: 2
                                    Layout.fillHeight: true
                                }

                                Ligne_Champs{
                                    id:champ_a2
                                   // value_largeur: grid_pinces.largeur
                                    value_nom: "a2 ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value : formule.getPince_Tirefonds_a2(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois),champ_alpha.value).toFixed(0)
                                    value_color:{
                                        if(champ_a2.value_todouble<=champ_distance_vertical.value_todouble){
                                            "green"
                                        }
                                        else{"red"}
                                    }
                                }

                                Ligne_Champs{
                                    id:champ_a3c
                                    property double d3 : champ_position.value
                                  //  value_largeur: grid_pinces.largeur
                                    value_nom: "a3c ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value : formule.getPince_Tirefonds_a3c(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois),champ_alpha.value).toFixed(0)
                                    value_color:{
                                        if(champ_a3c.value_todouble<=d3){
                                            "green"
                                        }
                                        else{"red"}
                                    }
                                }
                                Ligne_Champs{
                                    id:champ_a4c
                                  //  value_largeur: grid_pinces.largeur
                                    property double d4 : (champ_hp.value_todouble-(champ_nbr_ligne.value_todouble-1)*champ_distance_vertical.value_todouble)/2
                                    value_nom: "a4c ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value : formule.getPince_Tirefonds_a4c(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois)).toFixed(0)
                                    value_color:{
                                        if(champ_a4c.value_todouble<=d4){
                                            "green"
                                        }
                                        else{"red"}
                                    }
                                }

                                Ligne_Champs{
                                    id:champ_a3t
                                  //  value_largeur: grid_pinces.largeur
                                    property double d3 : champ_position.value
                                    value_nom: "a3t ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value : formule.getPince_Tirefonds_a3t(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois),champ_alpha.value).toFixed(0)
                                    value_color:{
                                        if(champ_a3t.value_todouble<=d3){
                                            "green"
                                        }
                                        else{"red"}
                                    }
                                }

                                Ligne_Champs{
                                    id:champ_a4t
                                 //   value_largeur: grid_pinces.largeur
                                    property double d4 : (champ_hp.value_todouble-(champ_nbr_ligne.value_todouble-1)*champ_distance_vertical.value_todouble)/2
                                    value_nom: "a4t ="
                                    value_unite: "mm"
                                    value_isEditable: false
                                    value :formule.getPince_Tirefonds_a4t(champ_d.value,formule.getMasseVolCarac(carac2.classe_bois),champ_alpha.value).toFixed(0)
                                    value_color:{
                                        if(champ_a4t.value_todouble<=d4){
                                            "green"
                                        }
                                        else{"red"}
                                    }
                                }

                            }

                            Image{
                                source:"qrc:/Images/Images_pinces.jpg"
                                Layout.alignment: Qt.AlignTop
                                Layout.maximumWidth:cfg.isAndro()?Screen.desktopAvailableWidth-50 : 350
                                fillMode: Image.PreserveAspectFit
                                Layout.preferredHeight: paintedHeight
                                Layout.fillWidth: cfg.isAndro()? true : false
                            }
                        }
                    }
                }


            }

            ////PAGE 2

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

                    Ligne_Champs{
                        id:champ_effort
                        value_nom:"Effort tranchant :"
                        value_unite:"kN"
                        value: "3.00"
                    }


                    GroupBox{
                        id : box_resistance_organe_cisaillement
                        title :"Vérification des organes dans le bois :"
                        Layout.fillWidth: true
                        font.bold : true
                        ColumnLayout{
                            id:colveriforagne
                            anchors.fill:parent

                            Text{
                                Layout.fillWidth: true
                                text:"Résistance du bois à l'enfoncement "
                                font.bold:true
                            }

                            Ligne_Champs{
                                id:champ_fh1k
                                value_nom: "Pièce 1 : fh1k ="
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getfhkBois(formule.getMasseVolCarac(carac1.classe_bois),champ_d.value,champ_beta.value,carac1.materiau,Organe.Tirefond).toFixed(2)

                            }

                            Ligne_Champs{
                                id:champ_fh2k
                                value_nom: "Pièce 2 : fh2k ="
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getfhkBois(formule.getMasseVolCarac(carac2.classe_bois),champ_d.value,champ_beta.value,carac2.materiau,Organe.Tirefond).toFixed(2)
                            }

                            Separateur{}

                            Text{
                                text:"Moment plastique de l'organe"
                                Layout.fillWidth: true
                                font.bold:true
                            }

                            Ligne_Champs{
                                id:champ_myrk
                                value_nom: "MyRk ="
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getMyrk_Tire_Fonds(champ_fu.value,champ_d.value).toExponential(2)
                            }

                            Separateur{}

                                Text{
                                    text:"Résistance en traction des organes"
                                    Layout.fillWidth: true
                                    font.bold:true
                                }

                                Ligne_Champs{
                                    id:champ_FaxRk
                                    value_nom: "FaxRk ="
                                    value_unite: "N"
                                    value_isEditable: false
                                    value:{if (combo_organes.currentIndex==0){
                                            formule.getFaxRk_Tirefonds(champ_nbr_eff.n,champ_d.value,champ_lg.value,champ_alpha.value,pk).toExponential(2)
                                        }
                                        else{formule.getFaxRk_Tirefonds(champ_nbr_eff.n,champ_d.value,champ_lg.value,90,pk).toExponential(2)}
                                    }
                                }
                            Separateur{}


                            Text{
                                text:"Mode de rupture de Johansen "
                                Layout.fillWidth: true
                                font.bold:true
                            }

                            GroupBox{
                                id:box_johansen
                                Layout.fillWidth: true
                                ColumnLayout{
                                    Ligne_Champs{
                                        id:champ_t1
                                        value_nom: "Pénétration de l'organe\ndans la pièce 1 :"
                                        value_unite: "mm"
                                        value_isEditable: false
                                        value:(champ_lg.value_todouble-champ_t.value_todouble).toFixed(2)
                                        onValue_todoubleChanged: {if(champ_t1.value_todouble<=0){
                                                champ_t1.value_color="red"
                                                txt_erreur_t1.visible=true
                                            }
                                            else{
                                                champ_t1.value_color="black"
                                                txt_erreur_t1.visible=false
                                            }
                                        }
                                    }

                                    Ligne_Champs{
                                        id:champ_t2
                                        value_nom: "Pénétration dans la pièce 2  :"
                                        value_unite: "mm"
                                        value_isEditable: false
                                        value:champ_t.value;
                                    }

                                    Ligne_Champs{
                                        id:champ_FvRk
                                        value_nom: "Résistance caractéristique \ncisaillement organe FvRk :"
                                        value_unite: "kN"
                                        value_isEditable: false
                                        value:((formule.getFvrk_B_Simple_Cisaillement(champ_fh1k.value,champ_fh2k.value,champ_t1.value,champ_t2.value,champ_d.value,champ_myrk.value,Organe.Tirefond,champ_FaxRk.value,true))/1000).toFixed(2)
                                    }

                                    Item{
                                        Layout.preferredHeight: reglages.defaut_hauteur_champs
                                    }

                                    Ligne_Champs{
                                        id:champ_FvRd
                                        value_nom: "Résistance de calcul \ncisaillement organe FvRd :"
                                        value_unite: "kN"
                                        value_isEditable: false
                                        value:(carac1.kmod*champ_FvRk.value/carac2.gammaM).toFixed(2)
                                    }

                                    Ligne_Champs{
                                        id:champ_effort_par_organe
                                        value_nom: "Effort par organe :"
                                        value_unite: "kN"
                                        value_isEditable: false
                                        value:(champ_effort.value/champ_nbr_eff.n).toFixed(2)
                                    }

                                    Ligne_Champs{
                                        id:champ_taux_organes
                                        value_nom: "Taux de travail η :"
                                        value_isEditable: false
                                        value_isBold: true
                                        value: formule.getTauxtravail(champ_effort_par_organe.value,champ_FvRd.value).toFixed(2)
                                        value_color: ((value_todouble >= 100)||(value_todouble<0)) ? "red" : "green"
                                        value_unite: "%"
                                    }
                                }
                            }
                        }
                    }

                    GroupBox{
                        id : box_resistance_fil_organes_cisaillement
                        title :"Vérification des files d'organes dans le bois"
                        Layout.fillWidth: true
                        font.bold : true
                        property double largeur:180
                        ColumnLayout{
                            anchors.fill:parent

                            Ligne_Champs{
                                id:champ_nbr_eff
                                property double n: champ_nbr_colonne.value*champ_nbr_ligne.value;
                             //   value_largeur: box_resistance_fil_organes_cisaillement.largeur
                                value_nom:"Nombre effectifs d'organes :"
                                value_unite:""
                                value_isEditable: false
                                value:formule.getNbrEff_Organe_Cisaillment(Organe.Tirefond,n,champ_d.value,champ_a1.value,true)
                            }

                            Ligne_Champs{
                                id:champ_FveffRk
                             //   value_largeur: box_resistance_fil_organes_cisaillement.largeur
                                value_nom:"Résitance caractéristique \neffective des files de broches \nau cisaillement FeffRk :"
                                value_unite:"N"
                                value_isEditable: false
                                value:(champ_nbr_eff.value*champ_FvRk.value).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_FveffRd
                              //  value_largeur: box_resistance_fil_organes_cisaillement.largeur
                                value_nom:"Résitance de calcul \neffective des files de broches \nau cisaillement FeffRd :"
                                value_unite:"N"
                                value_isEditable: false
                                value:(carac1.kmod*champ_FveffRk.value/carac2.gammaM).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_taux_files_organes
                              //  value_largeur: box_resistance_fil_organes_cisaillement.largeur
                                value_nom: "Taux de travail η :"
                                value_isEditable: false
                                value_isBold: true
                                value: formule.getTauxtravail(champ_effort.value,champ_FveffRd.value).toFixed(2)
                                value_color: ((value_todouble >= 100)||(value_todouble<0)) ? "red" : "green"
                                value_unite: "%"
                            }
                        }
                    }

                    GroupBox{
                        id : box_resistance_fendage_piece1
                        title :"Vérification de la pièce 1 au fendage :"
                        Layout.fillWidth: true
                        font.bold:true
                        property double largeur: 180

                        ColumnLayout{
                            Layout.fillWidth: true

                            Ligne_Champs{
                                id:champ_he1
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Distance entre la rive chargée \net l'organe le plus éloigné :"
                                value_unite: "mm"
                                value_isEditable: false
                                value:(champ_bp.value_todouble*Math.tan(champ_alpha.value*Math.PI/180)/2+champ_a4t.d4+(champ_nbr_ligne.value_todouble-1)*champ_distance_vertical.value_todouble).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_kcr1
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Coefficient de fissuration kcr"
                                value: "0.67"
                            }

                            Ligne_Champs{
                                id:champ_F90Rk1
                             //   value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"F90Rk :"
                                value_unite: "kN"
                                value_isEditable: false
                                value:(formule.getResistanceFendage(champ_t1.value,champ_ha.value,champ_he1.value,true)/1000).toFixed(2)

                            }

                            Ligne_Champs{
                                id:champ_F90Rd1
                             //   value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Résistance au fendage F90Rd :"
                                value_unite: "kN"
                                value_isEditable: false
                                value:(champ_F90Rk1.value*carac1.kmod/carac1.gammaM).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_contrainte_cisaillement_1
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Contrainte de cisaillement :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getContrainteCisaillement(champ_effort.value*100*Math.cos(champ_alpha.value*Math.PI/180),champ_kcr1.value,champ_t1.value,champ_he1.value).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_resistance_cisaillement_1
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Résistance au cisaillement FvRd :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getResistanceCisaillement(formule.getfvk(carac1.classe_bois),carac1.kmod,carac1.gammaM,true).toFixed(2)


                            }

                            Ligne_Champs{
                                id:champ_taux_fendage_piece1
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom: "Taux de travail η :"
                                value_isEditable: false
                                value_isBold: true
                                value: Math.max(formule.getTauxtravail(champ_contrainte_cisaillement_1.value,champ_resistance_cisaillement_1.value),formule.getTauxtravail(champ_effort.value,champ_F90Rd1.value)).toFixed(2)
                                value_color: ((value_todouble >= 100)||(value_todouble<0)) ? "red" : "green"
                                value_unite: "%"
                            }
                        }
                    }

                    GroupBox{
                        id:box_resistance_compression_piece1
                        title :"Vérification de la pièce 1 en compression :"
                        Layout.fillWidth: true
                        font.bold:true
                        visible:false

                        ColumnLayout{
                            Layout.fillWidth: true

                            Ligne_Champs{
                                id:champ_contrainte_compression
                                value_nom:"Contrainte de compression :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getContrainteCompressionAxiale(champ_effort.value*100,champ_ha.value*champ_ba.value).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_resistance_compression
                                value_nom:"Résistance en compression :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getResistanceCompressionAxiale(formule.getfc0k(carac1.classe_bois),carac1.kmod,carac1.gammaM).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_taux_compression
                                value_nom:"Taux de travail η:"
                                value_unite: "%"
                                value_isEditable: false
                                value_color: ((value_todouble >= 100)||(value_todouble<0)) ? "red" : "green"
                                value_isBold: true
                                value:formule.getTauxtravail(champ_contrainte_compression.value,champ_resistance_compression.value).toFixed(2)
                            }
                        }
                    }

                    GroupBox{
                        id : box_resistance_fendage_piece2
                        title :"Vérification de la pièce 2 au fendage :"
                        Layout.fillWidth: true
                        font.bold:true

                        ColumnLayout{
                            Layout.fillWidth: true

                            Ligne_Champs{
                                id:champ_he2
                               // value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Distance entre la rive chargée \net l'organe le plus éloigné :"
                                value_unite: "mm"
                                value_isEditable: false
                                value:champ_a4t.d4+(champ_nbr_ligne.value_todouble-1)*champ_distance_vertical.value_todouble
                            }

                            Ligne_Champs{
                                id:champ_kcr2
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Coefficient de fissuration kcr"
                                value: "0.67"
                            }

                            Ligne_Champs{
                                id:champ_F90Rk2
                            //   value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"F90Rk :"
                                value_unite: "kN"
                                value_isEditable: false
                                value:(formule.getResistanceFendage(champ_t.value,champ_hp.value,champ_he2.value,true)/1000).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_F90Rd2
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Résistance au fendage F90Rd :"
                                value_unite: "kN"
                                value_isEditable: false
                                value:(champ_F90Rk2.value*carac1.kmod/carac2.gammaM).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_contrainte_cisaillement_2
                             //   value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Contrainte de cisaillement :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getContrainteCisaillement(champ_effort.value*100,champ_kcr2.value,champ_t2.value,champ_he2.value).toFixed(2)
                            }

                            Ligne_Champs{
                                id:champ_resistance_cisaillement_2
                             //   value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom:"Résistance au cisaillement FvRd :"
                                value_unite: "MPa"
                                value_isEditable: false
                                value:formule.getResistanceCisaillement(formule.getfvk(carac2.classe_bois),carac1.kmod,carac2.gammaM,true).toFixed(2)


                            }

                            Ligne_Champs{
                                id:champ_taux_fendage_piece2
                              //  value_largeur: box_resistance_fendage_piece1.largeur
                                value_nom: "Taux de travail η :"
                                value_isEditable: false
                                value_isBold: true
                                value: Math.max(formule.getTauxtravail(champ_contrainte_cisaillement_2.value,champ_resistance_cisaillement_2.value),formule.getTauxtravail(champ_effort.value,champ_F90Rd2.value)).toFixed(2)
                                value_color: ((value_todouble >= 100)||(value_todouble<0)) ? "red" : "green"
                                value_unite: "%"
                            }
                        }
                    }
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

        Volet_Resultats{
            id:volet_resultats
            visible: cfg.isAndro()? false : true
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClickedChanged: {
                // Texte de base
                remplir="<H2 align=\"center\">Assemblage par tirefonds ou vis</H2><BR> "
            }
        }
    }


}
