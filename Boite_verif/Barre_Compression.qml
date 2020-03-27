import QtQuick 2.0
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import "../boites"
import "../accessoires"

Item{
    id:chef
    anchors.fill: parent
    property string lBarre:"4500"

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
                    columns: cfg.isAndro()? 1: 2
                    flow: GridLayout.TopToBottom

                    Style_h1{text:"Barre en compression"}

                    Caracteristiques{
                        id:carac
                        Layout.fillWidth: true
                    }

                    DefinitionBarre{
                        id:def_barre
                        bBarre:"200"
                        hBarre: "200"
                    }
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
                        lBarre: def_barre.lBarre
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
       /* Volet_Resultats{
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


