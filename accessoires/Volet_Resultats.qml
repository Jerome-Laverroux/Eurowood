import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item{
    id:chef
    property string remplir
    property bool clicked:false

    RowLayout{
        anchors.fill: parent

        Rectangle{
            id:separateur
            Layout.preferredWidth: 2
            Layout.preferredHeight: chef.height - 20
            color: "grey"
        }

        ColumnLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true

            Button{
                Layout.preferredWidth: chef.width -50
                Layout.alignment: Qt.AlignHCenter
                text: "Lancer le calcul"
                onClicked: {
                    chef.clicked=true;
                    resultat.text=remplir
                    chef.clicked=false;
                }
            }
            TextArea{
                id:resultat
                Layout.fillHeight: true
                Layout.fillWidth: true
                textFormat: TextEdit.RichText
                readOnly: true


            }
        }
    }
}

