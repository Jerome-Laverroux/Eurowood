#include "ass_gousset.h"
#include "libeurocod2.h"



//Masse volumique des classe de bois Pk en kg/m3

#define C18 320
#define C22 340
#define C24 350

//definition du contreplaqué ép. 10mm
#define CP  660
//#define epCP    10
#define pi 3.14159265

//Definition des pointes
double Dpointe;//diam pointe
double Lpointe;// Lg pointe
double Dhpointe;// Lg tête de pointe
double fu;//pour calcul moment plastique

Libeurocod2 formule;


QString noteCalcul;


Ass_gousset::Ass_gousset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ass_gousset)
{



    ui->setupUi(this);


    ui->lineEdit_B2->setDisabled(true);
    ui->lineEdit_H2->setDisabled(true);
    ui->lineEdit_6->setDisabled(true);
    ui->lineEdit_8->setDisabled(true);

    ui->comboBox->setCurrentIndex(2);
    ui->comboBox_2->setCurrentIndex(1);
    ui->Lg_pointes->setCurrentIndex(1);
    ui->bouton_Imp->setDisabled(true);
}

Ass_gousset::~Ass_gousset()
{
    delete ui;
}

void Ass_gousset::on_Calculer_clicked()
{
    ui->textEdit->clear();
    noteCalcul.clear();
    fu=600;//Resistance en traction de l'acier;
    //if(ui->Lg_pointes->currentText().contains("3x50")){Dpointe=3.0;Lpointe=50.0;Dhpointe=6.8,fu=600;}
    Dpointe=ui->D_pointes->currentText().toDouble();
    Lpointe=ui->Lg_pointes->currentText().toDouble();
    Dhpointe=ui->D_tete_pointe->currentText().toDouble();

    noteCalcul.append("<Body>");
    noteCalcul.append("<center><U><B>"+ui->NomAss->text()+"</B></U></center><BR>");

    noteCalcul.append("<BR><U>Pointes choisies :</U>"+ui->D_pointes->currentText()+" x "+ui->Lg_pointes->currentText()+" "+ui->Type_pointes->currentText()+"<BR>");
    noteCalcul.append("Diamètre D="+cast(Dpointe)+"mm<BR>");
    noteCalcul.append("Longueur L="+cast(Lpointe)+"mm<BR>");
    noteCalcul.append("diamètre de la tête Dh="+cast(Dhpointe)+"mm<BR>");
    noteCalcul.append("Résistance du fil d'acier f<SUB>u</SUB>="+cast(fu)+"N/mm²<BR>");

    noteCalcul.append("<BR><U>Action ELU :efforts sous la combinaison </U>"+ui->comboPonderations->currentText()+"<BR>");
    noteCalcul.append("<BR><U>Classe de service : </U>"+ui->comboBox_2->currentText()+"<BR>");


    double t=ui->lineEdit_ep_agglo->text().toDouble();
    double tpen=Lpointe-t;// longueur de pénétration de la pointe

    double pk1=0,pk2=0;//masses volumiques caractéristique

    if(ui->comboBox->currentText()=="C18"){pk1=C18;}
    if(ui->comboBox->currentText()=="C22"){pk1=C22;}
    if(ui->comboBox->currentText()=="C24"){pk1=C24;}




    pk2=CP;


    noteCalcul.append("<BR><U>Bois Massif </U>"+ui->comboBox->currentText()+"<BR>p<SUB>k</SUB>="+cast(pk1)+"kg/m<SUP>3</SUP><BR>");
    noteCalcul.append("<BR><U>Gousset : contreplaqué ép."+ui->lineEdit_ep_agglo->text()+"mm</U><BR>p<SUB>k</SUB>="+cast(pk2)+"kg/m<SUP>3</SUP><BR>");
    noteCalcul.append("<BR><U>Diagonale/barre "+ui->Nom_diag_d1->text()+" :</U> "+ui->lineEdit_B1->text()+"x"+ui->lineEdit_H1->text()+"mm; sollicitation N= "+ui->lineEdit_5->text()+" DaN; Angle :"+ui->lineEdit_7->text()+" °");
    if(ui->checkBox->isChecked()){noteCalcul.append("<BR><U>Diagonale/barre"+ui->Nom_diag_d2->text()+" :</U> "+ui->lineEdit_B2->text()+"x"+ui->lineEdit_H2->text()+"mm; sollicitation N= "+ui->lineEdit_6->text()+" DaN; Angle :"+ui->lineEdit_8->text()+" °");}
    noteCalcul.append("<BR><U>Entrait ou poiçon "+ui->Nom_barre_entrait->text()+" :</U> "+ui->lineEdit_Bentrait->text()+"x"+ui->lineEdit_Hentrait->text()+"mm;");

    noteCalcul.append("<BR>");
    noteCalcul.append("longueur de pénétration du côté de la pointe t<SUB>pen</SUB> :"+cast(tpen)+" mm<BR>");
    noteCalcul.append("longueur de pénétration du côté de la tête de pointe t :"+cast(t)+" mm<BR>");
    noteCalcul.append("Masse volumique caractéristique p<SUB>k1</SUB> :"+cast(pk1)+" mm<BR>");
    noteCalcul.append("Masse volumique caractéristique p<SUB>k2</SUB> :"+cast(pk2)+" mm<BR>");
    noteCalcul.append("t<SUB>pen</SUB>="+cast(tpen)+"mm, soit"+cast(qRound(tpen/Dpointe))+"d<BR>");
    if(tpen>(12*Dpointe)){noteCalcul.append("t<SUB>pen</SUB> > 12d, pas de minoration de F<SUB>ax,Rk</SUB><BR>");}




    //******************** vérif. condition de pénétration
    noteCalcul.append("<BR><U><B>vérification des conditions de pénétration: 8d pour les pointes lisses</B></U><BR><BR>");

    double dimpointes=0;
    dimpointes=((tpen)/Dpointe);
    noteCalcul.append("Travail en deux simple cisaillements, longueur de pénétration : "+cast(Lpointe)+" - "+cast(t)+" = "+cast(Lpointe-t)+" mm, soit "+cast(Lpointe-t)+"/"+cast(Dpointe)+" = "+cast(qRound(dimpointes))+"d <BR>");
    if((dimpointes*Dpointe)<=(8*Dpointe))
    {QMessageBox::information(this,"Problème",cast(dimpointes)+"d>8d<BR> Les pointes sont sous dimensionnées");}
    else
    {
        noteCalcul.append(cast(qRound(dimpointes))+"d> 8d : Critère vérifié: dimension des pointes correcte vis à vis de l'épaisseur des pièces<BR>");
    }



    //********************Calcul de Fax,RK capacité caractéristique à l'arrachement
    noteCalcul.append("<BR><U><B>Calcul de Fa<SUB>x,Rk</SUB> capacité caractéristique à l'arrachement</B></U><BR><BR>");

    double faxrk=0;

        //résistance caractéristique à l'arrachement en N/mm²
    noteCalcul.append("Résistance caractéristique à l'arrachement en N/mm²<BR>");

    double faxk=0;
    faxk=20*qPow(10,-6)*qPow(pk1,2);
    noteCalcul.append("F<SUB>ax,k</SUB> = 20x10<SUP>-6</SUP> . p<SUB>k1</SUB><SUP>2</SUP> = 20x10<SUP>-6</SUP> x "+cast(qPow(pk1,2))+" = "+cast(faxk)+" N/mm²<BR>");


        //résistance caractéristique à la traversée de la tête en N/mm²
    noteCalcul.append("Résistance caractéristique à la traversée de la tête en N/mm²<BR>");

    double fheadk=0;
    fheadk=70*qPow(10,-6)*qPow(pk2,2);
    noteCalcul.append("F<SUB>head,k</SUB> = 70x10<SUP>-6</SUP> . p<SUB>k2</SUB><SUP>2</SUP> = 70x10<SUP>-6</SUP> x "+cast(qPow(pk2,2))+" = "+cast(fheadk)+" N/mm²<BR>");



    if(ui->Type_pointes->currentText().contains("Li"))
    {
        noteCalcul.append("Pointes lisses<BR>");
        noteCalcul.append("Soit : F<SUB>ax,Rk</SUB> = min<BR>");
        noteCalcul.append("F<SUB>ax,k</SUB> . d . t<SUB>pen</SUB> = "+cast(faxk)+" x "+cast(Dpointe)+" x "+cast(tpen)+" = "+cast(faxk*Dpointe*tpen) +" N <BR>");
        noteCalcul.append("F<SUB>ax,k</SUB> . dt + f<SUB>head,k</SUB> . d<SUB>h</SUB><SUP>2</SUP> = "+cast(faxk)+" x "+cast(Dpointe)+" x "+cast(t)+" + "+cast(fheadk) +" x "+cast(qPow(Dhpointe,2))+" = "+cast((faxk*Dpointe*t)+fheadk*qPow(Dhpointe,2))+" N <BR>");
        faxrk=qMin((faxk*Dpointe*tpen),(faxk*Dpointe*t)+fheadk*qPow(Dhpointe,2));
    }
    if(ui->Type_pointes->currentText().contains("Ann"))
    {
        noteCalcul.append("Pointes annelées<BR>");
        noteCalcul.append("Soit : F<SUB>ax,Rk</SUB> = min<BR>");
        noteCalcul.append("F<SUB>ax,k</SUB> . d . t<SUB>pen</SUB> = "+cast(faxk)+" x "+cast(Dpointe)+" x "+cast(tpen)+" = "+cast(faxk*Dpointe*tpen) +" N <BR>");
        noteCalcul.append("F<SUB>ax,k</SUB> . d<SUB>h</SUB><SUP>2</SUP> = "+cast(faxk)+" x "+cast(qPow(Dhpointe,2))+" = "+cast(faxk*qPow(Dhpointe,2))+" N <BR>");
        faxrk=qMin((faxk*Dpointe*tpen),faxk*qPow(Dhpointe,2));
    }






    noteCalcul.append("Soit : F<SUB>ax,Rk</SUB> = "+cast(faxrk)+" N<BR>");

    //*************************Valeur caractéristique de la capacité résistante Fv,Rk
    noteCalcul.append("<BR><U><B>Valeur caractéristique de la capacité résistante F<SUB>v,Rk</SUB></B></U><BR><BR>");

        //Valeur de pénétration de la tige
    noteCalcul.append("Valeur de pénétration de la tige :<BR>");

    double t1=t;
    double t2=Lpointe-t;
    noteCalcul.append("t<SUB>1</SUB>="+cast(t)+" mm(épaisseur de la pièce sous la tête)<BR>");
    noteCalcul.append("t<SUB>2</SUB>="+cast(Lpointe)+" - "+cast(t)+" = "+cast(Lpointe-t)+" mm(enfoncement côté pointe)<BR>");


        // Portance locale
    noteCalcul.append("Portance locale :<BR>");

    //contreplaqué:
    double fh1k=formule.getfhkBois(pk2,Dpointe,0,"Bois massif",Libeurocod2::Organe::Pointe);
    noteCalcul.append("Contreplaqué: t<SUB>1</SUB>(sans préperçage):<BR>");
    noteCalcul.append("F<SUB>h,1,k</SUB> = 0,11 . p<SUB>k2</SUB> . d<SUP>-0,3</SUP> = 0,11 x "+cast(pk2)+" x "+cast(Dpointe)+"<SUP>-0,3</SUP> = "+cast(fh1k)+" N/mm²<BR>");


    //Bois massif:
    double fh2k=formule.getfhkBois(pk1,Dpointe,0,"Bois massif",Libeurocod2::Organe::Pointe);
    noteCalcul.append("Bois massif: t<SUB>2</SUB>(sans préperçage):<BR>");
    noteCalcul.append("F<SUB>h,2,k</SUB> = 0,082 . p<SUB>k1</SUB> . d<SUP>-0,3</SUP> = 0,082 x "+cast(pk1)+" x "+cast(Dpointe)+"<SUP>-0,3</SUP> = "+cast(fh2k)+" N/mm²<BR>");

        //********************* Moment d'écoulement plastique
    noteCalcul.append("<BR><U><B>Moment d'écoulement plastique</B></U><BR><BR>");
    double Myrk=0.3*fu*qPow(Dpointe,2.6);
    noteCalcul.append("M<SUB>y,Rk</SUB> = 0,3 . f<SUB>u</SUB> . d<SUP>2.6</SUP> = 0.3 x "+cast(fu)+" x "+cast(Dpointe)+"<SUP>2.6</SUP> = "+cast(Myrk)+" N/mm<BR>");


        //*********************Résistance pour chaque mode de rupture
    noteCalcul.append("<BR><U><B>Résistance pour chaque mode de rupture</B></U><BR><BR>");
    double beta=fh2k/fh1k;
    noteCalcul.append("Rapport ß=f<SUB>h,2,k</SUB>/f<SUB>h,1,k</SUB> = "+cast(fh2k)+"/"+cast(fh1k)+" = "+cast(beta)+"<BR>");

        //******************** Calcul de l'effet de corde
    noteCalcul.append("<BR><U><B>Calcul de l'effet de corde</B></U><BR><BR>");


    // soit 15% de la partie de Johansen à  830.5N-> 124.5N ->pointes lisses
    // soit 50% de la partie de Johansen à 830.5N-> 124.5N ->pointes annelées


    double effetCorde=faxrk/4;

    noteCalcul.append("Effet de corde: F<SUB>ax,Rk</SUB>/4 = "+cast(effetCorde)+" N <BR>");

    if(effetCorde>=124.5){QMessageBox::information(this,"Info","L'effet de corde calculé est supérieur à 124.5N");}

    noteCalcul.append("<BR>");
    double a=formule.getFvrk_B_A(fh1k,t1,Dpointe);///TODO A modifier en getFvrkB_A
    double b=formule.getFvrk_B_B(fh2k,t2,Dpointe);
    double c=formule.getFvrk_B_C(fh1k,t1,t2,Dpointe,beta,Libeurocod2::Organe::Pointe,faxrk);
    double d=formule.getFvrk_B_D(fh1k,t1,Dpointe,beta,Myrk,Libeurocod2::Organe::Pointe,faxrk);
    double e=formule.getFvrk_B_E(fh1k,t2,Dpointe,beta,Myrk,Libeurocod2::Organe::Pointe,faxrk);
    double f=formule.getFvrk_B_F(fh1k,Dpointe,beta,Myrk,Libeurocod2::Organe::Pointe,faxrk);


    noteCalcul.append("(a) ="+cast(qRound(a))+" N : <BR>f<SUB>h,1,k</SUB> . t1 . d<BR><BR>");
    noteCalcul.append("(b) ="+cast(qRound(b))+" N : <BR>f<SUB>h,2,k</SUB> . t . d<BR><BR>");
    noteCalcul.append("(c) ="+cast(qRound(c))+" N : <BR><img src=':/c.png'HEIGHT='50'><BR>");
    noteCalcul.append("(d) ="+cast(qRound(d))+" N : <BR><img src=':/d.png'HEIGHT='50'><BR>");
    noteCalcul.append("(e) ="+cast(qRound(e))+" N : <BR><img src=':/e.png'HEIGHT='50'><BR>");
    noteCalcul.append("(f) ="+cast(qRound(f))+" N : <BR><img src=':/f.png'HEIGHT='50'><BR>");

    double fin;

    fin=qMin(a,b);fin=qMin(fin,c);fin=qMin(fin,d);fin=qMin(fin,e);fin=qMin(fin,f);

    noteCalcul.append("Valeur la plus faible :"+cast(qRound(fin))+" N<BR>");
    if(ui->Type_pointes->currentText().contains("Li"))
    {
        noteCalcul.append("Soit 15% de "+cast(qRound(fin))+" = "+cast(qRound(fin*0.15))+" N<BR>");
        if(effetCorde<(fin*0.15)){noteCalcul.append("L'effet de corde calculé est inférieur à "+cast(qRound(fin*0.15))+" N <BR>");}
    }
    if(ui->Type_pointes->currentText().contains("Ann"))
    {
        noteCalcul.append("Soit 50% de "+cast(qRound(fin))+" = "+cast(qRound(fin*0.5))+" N<BR>");
        if(effetCorde<(fin*0.5)){noteCalcul.append("L'effet de corde calculé est inférieur à "+cast(qRound(fin*0.5))+" N <BR>");}
    }


    double fvrk=qRound(fin);

    //***************Définition du nombre de pointes
    noteCalcul.append("<BR><U><B>Définition du nombre de pointes</B></U><BR><BR>");

        //résistance de calcul
    noteCalcul.append("Résistance de calcul F<SUB>V,Rd</SUB> :<BR>");

    //signe gamma en html : &#947;
    noteCalcul.append("F<SUB>V,Rd</SUB>=F<SUB>V,Rk</SUB> . k<SUB>mod</SUB>/&#947;M<BR>");

    double kmod=0.9;// voir pages 50 et 51 pour tableaux
    double gammaM=1.3;

    noteCalcul.append("k<SUB>mod</SUB>="+cast(kmod)+"<BR>");
    noteCalcul.append("&#947;M="+cast(gammaM)+"<BR>");


    double fvrd=fvrk*(kmod/gammaM);fvrd=qRound(fvrd);
    noteCalcul.append("<BR>F<SUB>V,Rd</SUB>="+cast(fvrd)+"<BR>");

        //nombre de pointes
    noteCalcul.append("Nombre de pointes :<BR>");

    double F1=0,F2=0,fved=0,nbpointes=0;

    /*Diagonale d1 sur contreplaqué*/
    noteCalcul.append("<BR>Barre "+ui->Nom_diag_d1->text()+" sur gousset<BR>");

    F1=ui->lineEdit_5->text().toDouble()*10;
    fved=F1;
    nbpointes=fved/fvrd;

    noteCalcul.append("Valeur appliquée: F<SUB>v,Ed</SUB> = "+cast(fved)+" N<BR>");
    noteCalcul.append("Nombre de pointes = F<SUB>v,Ed</SUB> / F<SUB>v,Rd</SUB>= "+cast(fved)+" / "+cast(fvrd)+" = "+cast(qRound(nbpointes))+" pointes -> Soit :<B>"+cast(qRound(nbpointes/2))+" pointes par faces</B><BR>");
    noteCalcul.append("Angle de l'effort par rapport au fil du bois: 0°<BR>");

    if(ui->checkBox->isChecked())
    {
        noteCalcul.append("<BR>Barre "+ui->Nom_diag_d2->text()+" sur gousset<BR>");

        F2=ui->lineEdit_6->text().toDouble()*10;
        fved=F2;
        nbpointes=fved/fvrd;

        noteCalcul.append("Valeur appliquée: F<SUB>v,Ed</SUB> = "+cast(fved)+" N<BR>");
        noteCalcul.append("Nombre de pointes = F<SUB>v,Ed</SUB> / F<SUB>v,Rd</SUB>= "+cast(fved)+" / "+cast(fvrd)+" = "+cast(qRound(nbpointes))+" pointes -> Soit :<B>"+cast(qRound(nbpointes/2))+" pointes par faces</B><BR>");
        noteCalcul.append("Angle de l'effort par rapport au fil du bois: 0°<BR>");
    }




    /*Entrait ou autre sur contreplaqué*/
    noteCalcul.append("<BR>Pièce "+ui->Nom_barre_entrait->text()+" recevant l'assemblage sur gousset<BR>");
    double alpha1=ui->lineEdit_7->text().toDouble();alpha1=pi*alpha1/180;
    double alpha2=ui->lineEdit_8->text().toDouble();alpha2=pi*alpha2/180;

    double fvedh=0,fvedv=0;
    if(!ui->checkBox->isChecked())
    {
        if(ui->comptrac1->currentText().contains("Comp"))
        {
            fvedh=(F1)*qCos(alpha1);
            fvedv=(F1)*qSin(alpha1);
            fved=qPow((qPow(fvedh,2)+qPow(fvedv,2)),0.5);
        }
        else//Traction
        {
            fvedh=(F1)*qCos(alpha1);
            fvedv=(-F1)*qSin(alpha1);
            fved=qPow((qPow(fvedh,2)+qPow(fvedv,2)),0.5);
        }

    }
    else
    {

        if(ui->comptrac1->currentText().contains("Comp")&&ui->comptrac2->currentText().contains("Trac"))
        {
            fvedh=((F1)*qCos(alpha1))+((F2)*qCos(alpha2));
            fvedv=((-F1)*qSin(alpha1))+((F2)*qSin(alpha2));
            fved=qPow((qPow((F2+F1)*qCos(alpha1),2)+qPow((F2-F1)*qSin(alpha1),2)),0.5);
           // QMessageBox::information(this,"Info","Compression traction");
        }
        if(ui->comptrac1->currentText().contains("Comp")&&ui->comptrac2->currentText().contains("Comp"))
        {
            fvedh=((F1)*qCos(alpha1))+((-F2)*qCos(alpha2));
            fvedv=((F1)*qSin(alpha1))+((F2)*qSin(alpha2));
            fved=qPow((qPow((F2+F1)*qCos(alpha1),2)+qPow((F2-F1)*qSin(alpha1),2)),0.5);
           // QMessageBox::information(this,"Info","Compression compression");
        }
        if(ui->comptrac1->currentText().contains("Trac")&&ui->comptrac2->currentText().contains("Trac"))
        {
            fvedh=((-F1)*qCos(alpha1))+((F2)*qCos(alpha2));
            fvedv=((F1)*qSin(alpha1))+((F2)*qSin(alpha2));
            fved=qPow((qPow((F2+F1)*qCos(alpha1),2)+qPow((F2-F1)*qSin(alpha1),2)),0.5);
           // QMessageBox::information(this,"Info","Traction traction");
        }
        if(ui->comptrac1->currentText().contains("Trac")&&ui->comptrac2->currentText().contains("Comp"))
        {
            fvedh=((F1)*qCos(alpha1))+((F2)*qCos(alpha2));
            fvedv=((F1)*qSin(alpha1))+((-F2)*qSin(alpha2));
            fved=qPow((qPow((F2+F1)*qCos(alpha1),2)+qPow((F2-F1)*qSin(alpha1),2)),0.5);
            //QMessageBox::information(this,"Info","Traction Compression");
        }
    }

    nbpointes=fved/fvrd;
    fved=qRound(fved);
    fvedh=qRound(fvedh);fvedh=qAbs(fvedh);
    fvedv=qRound(fvedv);fvedh=qAbs(fvedh);

    noteCalcul.append("Valeur appliquée: F<SUB>v,Ed</SUB> = "+cast(fved)+" N(horizontal:"+cast(fvedh)+" ; vertical:"+cast(fvedv)+")<BR>");
    noteCalcul.append("Nombre de pointes = F<SUB>v,Ed</SUB> / F<SUB>v,Rd</SUB>= "+cast(fved)+" / "+cast(fvrd)+" = "+cast(qRound(nbpointes))+" pointes -> Soit :<B>"+cast(qRound(nbpointes/2))+" pointes par faces</B><BR>");


    double alpha=qAtan(fvedv/fvedh);
    double alpharad=alpha;
    alpha=alpha*180/pi;//alpha=qRound(alpha);

    alpha=arrondi(alpha);
    noteCalcul.append("Angle de l'effort par rapport au fil du bois:<BR>");
    noteCalcul.append("alpha = tan<SUP>-1</SUP>("+cast(fvedv)+" / "+cast(fvedh)+") = "+cast(alpha)+"°<BR>");

    noteCalcul.append("<BR><U><B>Conditions de pince</B></U><BR><BR>");
    double a1=0,a2=0,a3t=0,a3c=0,a4t=0,a4c=0;
    noteCalcul.append("<B>Diagonale(s)</B><BR>");
    noteCalcul.append("<TABLE width=500>");
    a1=(5+5*qCos(0))*Dpointe*0.85;
    noteCalcul.append("<TR>");
    noteCalcul.append("<TD>Pince</TD><TD>Angle</TD><TD>Sans préperçage</TD><TD>Distance mini.</TD><TD>Distance retenue</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("<TD>Espacement parallèle au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a1 </TD><TD>"+cast(0)+"°</TD><TD> d&lt;5mm : (5+5cos a) .d . 0.85</TD><TD> "+cast(a1)+"</TD><TD>"+cast(ceil(a1/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a2=5*Dpointe*0.85;
    noteCalcul.append("<TD>Espacement perpendiculaire au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a2 </TD><TD>"+cast(0)+"°</TD><TD> 5d . 0.85</TD><TD>"+cast(a2)+"</TD><TD>"+cast(ceil(a2/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3t=(10+5*qCos(0))*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3t </TD><TD>"+cast(0)+"°</TD><TD> (10+5cos a) . d</TD><TD>"+cast(a3t)+"</TD><TD>"+cast(ceil(a3t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3c=10*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3c </TD><TD>"+cast(0)+"°</TD><TD>10d</TD><TD>"+cast(a3c)+"</TD><TD>"+cast(ceil(a3c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4t=(5+2*qSin(0))*Dpointe;
    noteCalcul.append("<TD>Distance de rive chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4t </TD><TD>"+cast(0)+"°</TD><TD>(5+2sin a) . d</TD><TD>"+cast(a4t)+"</TD><TD>"+cast(ceil(a4t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4c=5*Dpointe;
    noteCalcul.append("<TD>Distance de rive non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4c </TD><TD>"+cast(0)+"°</TD><TD>5d</TD><TD>"+cast(a4c)+"</TD><TD>"+cast(ceil(a4c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("</TABLE>");



    noteCalcul.append("<B>Pièce recevant l'assemblage</B><BR>");
    noteCalcul.append("<TABLE width=500>");
    a1=(5+5*qCos(alpharad))*Dpointe*0.85;
    noteCalcul.append("<TR>");
    noteCalcul.append("<TD>Pince</TD><TD>Angle</TD><TD>Sans préperçage</TD><TD>Distance mini.</TD><TD>Distance retenue</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("<TD>Espacement parallèle au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a1 </TD><TD>"+cast(alpha)+"°</TD><TD> d&lt;5mm : (5+5cos a) .d . 0.85</TD><TD> "+cast(a1)+"</TD><TD>"+cast(ceil(a1/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a2=5*Dpointe*0.85;
    noteCalcul.append("<TD>Espacement perpendiculaire au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a2 </TD><TD>"+cast(alpha)+"°</TD><TD> 5d . 0.85</TD><TD>"+cast(a2)+"</TD><TD>"+cast(ceil(a2/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3t=(10+5*qCos(alpharad))*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3t </TD><TD>"+cast(alpha)+"°</TD><TD> (10+5cos a) . d</TD><TD>"+cast(a3t)+"</TD><TD>"+cast(ceil(a3t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3c=10*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3c </TD><TD>"+cast(alpha)+"°</TD><TD>10d</TD><TD>"+cast(a3c)+"</TD><TD>"+cast(ceil(a3c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4t=(5+2*qSin(alpharad))*Dpointe;
    noteCalcul.append("<TD>Distance de rive chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4t </TD><TD>"+cast(alpha)+"°</TD><TD>(5+2sin a) . d</TD><TD>"+cast(a4t)+"</TD><TD>"+cast(ceil(a4t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4c=5*Dpointe;
    noteCalcul.append("<TD>Distance de rive non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4c </TD><TD>"+cast(alpha)+"°</TD><TD>5d</TD><TD>"+cast(a4c)+"</TD><TD>"+cast(ceil(a4c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("</TABLE>");




    noteCalcul.append("<B>Gousset</B><BR>");
    noteCalcul.append("<TABLE width=500>");
    a1=(5+5*qCos(0))*Dpointe*0.85;
    noteCalcul.append("<TR>");
    noteCalcul.append("<TD>Pince</TD><TD>Angle</TD><TD>Sans préperçage</TD><TD>Distance mini.</TD><TD>Distance retenue</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("<TD>Espacement parallèle au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a1 </TD><TD>"+cast(0)+"°</TD><TD> d&lt;5mm : (5+5cos a) .d . 0.85</TD><TD> "+cast(a1)+"</TD><TD>"+cast(ceil(a1/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a2=5*Dpointe*0.85;
    noteCalcul.append("<TD>Espacement perpendiculaire au fil</TD></TR><TR>");
    noteCalcul.append("<TD>a2 </TD><TD>"+cast(0)+"°</TD><TD> 5d . 0.85</TD><TD>"+cast(a2)+"</TD><TD>"+cast(ceil(a2/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3t=(3+4*qSin(alpha1))*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3t(d1) </TD><TD>"+cast((alpha1*180)/pi)+"°</TD><TD> (3+4cos a) . d</TD><TD>"+cast(a3t)+"</TD><TD>"+cast(ceil(a3t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3t=(3+4*qSin(alpha2))*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3t(d2) </TD><TD>"+cast((alpha2*180)/pi)+"°</TD><TD> (3+4cos a) . d</TD><TD>"+cast(a3t)+"</TD><TD>"+cast(ceil(a3t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a3c=3*Dpointe;
    noteCalcul.append("<TD>Distance d'extrémité non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a3c </TD><TD>"+cast(0)+"°</TD><TD>3d</TD><TD>"+cast(a3c)+"</TD><TD>"+cast(ceil(a3c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4t=(3+4*qSin(alpha1))*Dpointe;
    noteCalcul.append("<TD>Distance de rive chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4t(d1) </TD><TD>"+cast((alpha1*180)/pi)+"°</TD><TD>(3+4sin a) . d</TD><TD>"+cast(a4t)+"</TD><TD>"+cast(ceil(a4t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4t=(3+4*qSin(alpha2))*Dpointe;
    noteCalcul.append("<TD>Distance de rive chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4t(d2) </TD><TD>"+cast((alpha2*180)/pi)+"°</TD><TD>(3+4sin a) . d</TD><TD>"+cast(a4t)+"</TD><TD>"+cast(ceil(a4t/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    a4c=3*Dpointe;
    noteCalcul.append("<TD>Distance de rive non chargée</TD></TR><TR>");
    noteCalcul.append("<TD>a4c </TD><TD>"+cast(0)+"°</TD><TD>3d</TD><TD>"+cast(a4c)+"</TD><TD>"+cast(ceil(a4c/10)*10)+" mm</TD>");
    noteCalcul.append("</TR><TR>");
    noteCalcul.append("</TABLE>");





    noteCalcul.append("</body>");

    ui->textEdit->toHtml();
    ui->textEdit->setText(noteCalcul);
    ui->bouton_Imp->setEnabled(true);
}/// fin slot Calculer


QString Ass_gousset::cast(double d)
{
    QString t;
    return t.setNum(d);
}

double Ass_gousset::arrondi(double d)
{
    int arr=0;
    arr=d*100;d=arr;d=d/100;
    return d;
}

void Ass_gousset::on_checkBox_toggled(bool checked)
{
    if(ui->checkBox->isChecked())
    {
        ui->lineEdit_B2->setDisabled(false);
        ui->lineEdit_H2->setDisabled(false);
        ui->lineEdit_6->setDisabled(false);
        ui->lineEdit_8->setDisabled(false);

    }
    else
    {
        ui->lineEdit_B2->setDisabled(true);
        ui->lineEdit_H2->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);
        ui->lineEdit_8->setDisabled(true);
    }
}
