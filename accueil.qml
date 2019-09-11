import QtQuick 2.12
import QtQml 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "Boite_verif"

ApplicationWindow {



    id:chef
    height:cfg.isAndro()?Screen.desktopAvailableHeight: 600
    width:cfg.isAndro()?Screen.desktopAvailableWidth: 800
    visible: true
    visibility : "Maximized"
    property string application_active: ""
    property string txt_Barre_Flexion: "Barre en flexion"
    property string txt_Barre_Compression: "Barre en compression"
    property string txt_Barre_Traction: "Barre en traction"
    property string txt_plat_traction: "Plat en traction"
    property string txt_cordon_soudure: "Cordons de soudure"
    property string txt_plat_flexion: "Plat en flexion"
    property string txt_assemblage_panne_arbaletrier: "Assemblages par tirefonds vis"


    Component.onCompleted: {
        if(cfg.isAndro()){drawer.open();}
    }

    Loader{
        id:loader_barre_flexion
        active:(application_active==txt_Barre_Flexion)? true : false
        sourceComponent: Barre_Flexion{
            visible: true
            parent: fenetre_principale
            }
    }

    Loader{
        id:loader_barre_compression
        active:(application_active==txt_Barre_Compression)? true : false
        sourceComponent: Barre_Compression{
            visible: true
            parent: fenetre_principale
            }
    }
    Loader{
        id:loader_barre_traction
        active:(application_active==txt_Barre_Traction)? true : false
        sourceComponent: Barre_Traction{
            visible: true
            parent: fenetre_principale
            }
    }




    Loader{
        id : loader_entaille
        active:(application_active == txt_plat_traction)? true :false
        sourceComponent: Plat_traction{
            visible : true
            parent: fenetre_principale
            }
    }
   Loader{
        id: loader_cordon_soudure
        active:(application_active == txt_cordon_soudure)? true :false
        sourceComponent: Cordon_soudure{
            visible:true
            parent:fenetre_principale
        }
    }
   Loader{
        id: loader_plat_flexion
        active:(application_active == txt_plat_flexion)? true :false
        sourceComponent: Verif_plat_flexion{
            visible:true
            parent:fenetre_principale
        }
    }

   Loader{
        id: loader_assemblage_arbaletrier_panne
        active:(application_active == txt_assemblage_panne_arbaletrier)? true :false
        sourceComponent: Verif_assemblage_tire_fond{
            visible:true
            parent:fenetre_principale
        }
    }


    menuBar:MenuBar{
        visible: cfg.isAndro()?false:true
        width: parent.width
        Menu{
            title: "Fichier"
            MenuItem{text: "Quitter";onTriggered:Qt.quit() }
        }

        Menu {
            title: "Vérification bois"

            MenuItem{text: txt_Barre_Flexion
                onClicked: {application_active=txt_Barre_Flexion}
            }
            MenuItem{text: txt_Barre_Compression
                onClicked: {application_active=txt_Barre_Compression}
            }
            MenuItem{text: txt_Barre_Traction
                onClicked: {application_active=txt_Barre_Traction}
            }

            MenuItem{text: "Contreventement ossature"}
        }
        Menu{
            title: "Vérification métal"
            MenuItem{text: txt_plat_traction
                onClicked: {application_active=txt_plat_traction}
            }
            MenuItem{text: txt_plat_flexion
                onClicked: {application_active=txt_plat_flexion}
            }
            MenuItem{text: txt_cordon_soudure
                onClicked: {application_active=txt_cordon_soudure}
            }

        }
        Menu {
            title: "Assemblages"
            MenuItem{text: txt_assemblage_panne_arbaletrier
                onClicked: {application_active=txt_assemblage_panne_arbaletrier}
            }
            MenuItem{text: "Gousset contreplaqué"}
        }
    }

    ///SECTION ANDROID

    Drawer{
        visible: cfg.isAndro()? true :false
        id:drawer
        width: 0.66 * chef.width
        height:chef.height
        ColumnLayout{
            anchors.fill: parent
            anchors.rightMargin: 5
            anchors.leftMargin: 5


            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 20
            }

            GroupBox{
                Layout.fillWidth: true
                label: Label{
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Vérification Bois"
                    font.bold: true
                    font.pointSize: 14
                    background: Rectangle{anchors.fill: parent;color:"#66FFCC"; }
                }

                ColumnLayout{
                    anchors.fill: parent

                  /*  Label{
                        Layout.fillWidth: true
                        text: txt_Barre
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_Barre;drawer.close();}
                        }
                    }*/
                    Label{
                        Layout.fillWidth: true
                        text: txt_Barre_Flexion
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_Barre_Flexion;drawer.close();}
                        }
                    }
                    Label{
                        Layout.fillWidth: true
                        text: txt_Barre_Compression
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_Barre_Compression;drawer.close();}
                        }
                    }
                    Label{
                        Layout.fillWidth: true
                        text: txt_Barre_Traction
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_Barre_Traction;drawer.close();}
                        }
                    }


                    Label{
                        Layout.fillWidth: true
                        text: "Contreventement ossature"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {;drawer.close();}
                        }
                    }
                }
            }

            GroupBox{
                Layout.fillWidth: true
                label: Label{
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Vérification métal"
                    font.bold: true
                    font.pointSize: 14
                    background: Rectangle{anchors.fill: parent;color:"#66FFCC"; }
                }
                ColumnLayout{
                    anchors.fill: parent

                    Label{
                        Layout.fillWidth: true
                        text: txt_plat_traction
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_plat_traction;drawer.close();}
                        }
                    }
                    Label{
                        Layout.fillWidth: true
                        text: txt_plat_flexion
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_plat_flexion;drawer.close();}
                        }
                    }
                    Label{
                        Layout.fillWidth: true
                        text: txt_cordon_soudure
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_cordon_soudure;drawer.close();}
                        }
                    }

                }
            }
            GroupBox{
                Layout.fillWidth: true
                label: Label{
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Assemblages"
                    font.bold: true
                    font.pointSize: 14
                    background: Rectangle{anchors.fill: parent;color:"#66FFCC"; }
                }

                ColumnLayout{
                    anchors.fill: parent

                    Label{
                        Layout.fillWidth: true
                        text: txt_assemblage_panne_arbaletrier
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {application_active=txt_assemblage_panne_arbaletrier;drawer.close();}
                        }
                    }
                    Label{
                        Layout.fillWidth: true
                        text: "Gousset contreplaqué"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {;drawer.close();}
                        }
                    }
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }


    }///fin drawer


    Item{
        id:fenetre_principale
        anchors.fill: parent
    }

}


