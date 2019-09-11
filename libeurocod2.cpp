#include "libeurocod2.h"
#define pi 3.141592654
#include <QResource>

/*
Bienvenue à toi étranger, tu te trouves dans une bibliothède de calcul EC3 et EC5,
tous les résultats qui te seront données seront en mm, N et MPa.

Tous les résultats soit disant vérifiés l'ont été par un stagiaire
extrêmement malintentionné et j'insiste sur le extrêmement !

Merci de respecter le silence attendu dans toute bibliothèque et de ne pas tout casser.
Bonne découverte !

Ledit Stagiaire
*/


QSqlQueryModel requete;

Libeurocod2::Libeurocod2(bool withBDD)
{
    if(withBDD)
    {
        qDebug()<<"ouverture BDD...";
        bdd=QSqlDatabase::addDatabase("QSQLITE","BASE_EC");
        Q_INIT_RESOURCE(Eurowood);


#ifdef Q_OS_ANDROID
        QFile dfile("assets:/db/base.db");
        if(dfile.exists()){

            dfile.copy("./base.db");
        }
            QFile::setPermissions("./base.db",QFile::WriteOwner |     QFile::ReadOwner);
        bdd.setDatabaseName("./base.db");
#else
        bdd.setDatabaseName("./base.db");
#endif

        if (!bdd.open()){qDebug()<<"erreur bdd introuvable";}

    }

    ///categories retrouvées suivant enum CategorieCharge
    m_categoriesCharges<<"Caregorie A Habitations résidentielles";
    m_categoriesCharges<<"Categorie B Bureaux";
    m_categoriesCharges<<"Categorie C Lieux de réunion";
    m_categoriesCharges<<"Categorie D Commerce";
    m_categoriesCharges<<"Categorie E Stockage";
    m_categoriesCharges<<"Categorie F";
    m_categoriesCharges<<"Categorie G";
    m_categoriesCharges<<"Categorie H Toits";
    m_categoriesCharges<<"Alt. inf. 1000m";
    m_categoriesCharges<<"Alt. sup. 1000m";
    m_categoriesCharges<<"Action du vent";
}

Libeurocod2::~Libeurocod2(){
    bdd.close();
    bdd.removeDatabase("BASE_EC");
}

double Libeurocod2::getBDD_Value(QString table, QString nom, QString nom_tag,QString tag)
{
    double sortie=0;
        QString chaine("SELECT \""+nom+"\" FROM \""+table+"\" WHERE \""+nom_tag+"\"='"+tag+"'");
//qDebug()<<chaine;
        QSqlQuery requete(chaine,bdd);
        while (requete.next()){
            sortie=requete.value(0).toDouble();
        }
        return sortie;
}

QStringList Libeurocod2::getBDD_Noms(QString table, QString nom,QString filtre)
{
    QStringList sortie;
    QString chaine;
        if(filtre.isEmpty())
        {
         chaine= chaine=("SELECT \""+nom+"\" FROM \""+table+"\"");
        }
        else {chaine=("SELECT \""+nom+"\" FROM \""+table+"\" WHERE \""+nom+"\" LIKE '%"+filtre+ "%'");}


       // qDebug()<<chaine;
        QSqlQuery requete(chaine,bdd);
        while (requete.next()){
            sortie<<requete.value(0).toString();
        }
        return sortie;

}







///recherches des valeurs dans les tableau EC5 depuis la BDD
///
///
///


/// Partie CLIMATIQUE
///
///
double Libeurocod2::getVent_vitesse_ref(QString region)
{
    double d=0;
    QSqlQuery query("SELECT Vitesse FROM Vent_V_ref WHERE Région LIKE '" + region+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

QStringList Libeurocod2::getVent_Liste_rugosites()
{
    QStringList t;
    QSqlQuery query("SELECT Categorie FROM Vent_rugosite" ,bdd);
    while(query.next()){t.append(query.value(0).toString());}
    return t;
}

QString Libeurocod2::getVent_rugosite_definition(QString rugosite)
{
    QString t;
    QSqlQuery query("SELECT Z0 FROM Vent_rugosite WHERE Categorie LIKE '" + rugosite+"'" ,bdd);
    while(query.next()){t.setNum(query.value(0).toDouble());}
    return t;
}

double Libeurocod2::getVent_rugosite_Z0(QString rugosite)
{
    double d=0;
    QSqlQuery query("SELECT Z0 FROM Vent_rugosite WHERE Categorie LIKE '" + rugosite+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

double Libeurocod2::getVent_rugosite_Zmin(QString rugosite)
{
    double d=0;
    QSqlQuery query("SELECT Zmin FROM Vent_rugosite WHERE Categorie LIKE '" + rugosite+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

double Libeurocod2::getVent_pression_dyn_pointe(double vb,double z0,double zmin)
{
    //vb vitesse de base de référence
    //z0
    //zmin hauteur minimal


    double  p=1.225;//p masse volumique de l'air
    //cez coeff d'exposition
    // qb pression de base dynamique






    //kl coeff de turbulence


    double kl=1.0;//annexe nationale


    double sigma_v=0;//ecart type de la turbulence
    double kr=0;// coefficient de terrain
    double vm=0;// vent moyen
    double cr=0;// facteur de rugosité
    double co=1.0;//facteur d'orographie

    kr=0.19 * qPow((z0/zmin),0.07);/// A VERIFIER
    cr=kr * qLn(zmin/z0);
    double qb;
    qb=0.5 * p * qPow(vb,2);
    sigma_v=kr * vb * kl;
    vm=cr * co* vb;

    double Iv=0;/// intensité de turbulence à la hauteur z
    Iv=sigma_v/vm;

    double qpz=(1+(7*Iv))* 0.5 * p * qPow(vm,2);
    return qpz;
}





//---------------------------------------------------Partie sur les classe de bois et autres coeff ....-----------------------------------

QStringList Libeurocod2::getListeClasseBois(int materiau)
{

    QString t;

    switch(materiau)
    {
    case 0://0= bois massif
        t.append("C");
        break;
    case 1://1= Lamellé collé
        t.append("GL");
        break;
    case 2://2= feuillu
        t.append("D");
        break;
    default:
        t.append("C");
        break;
    }

    QStringList liste;
    QSqlQuery query("SELECT Classe FROM caracBois WHERE Classe LIKE '"+t+"%'" ,bdd);
    while(query.next()){
        liste.append(query.value(0).toString());        
    }

    return liste;
}

QStringList Libeurocod2::getlisteClasseService()
{
    QStringList liste;
    liste<<"classe_1"<<"classe_2"<<"classe_3";
    return liste;
}

QStringList Libeurocod2::getListeClasseDuree()
{
    QStringList liste;
    QSqlQuery query("SELECT Classe_duree FROM kmod" );
    while(query.next()){liste.append(query.value(0).toString()),bdd;}
    return liste;
}

double Libeurocod2::getkmod(QString classeService, QString classeDuree)
{    
    return getBDD_Value("kmod",classeService,"Classe_duree",classeDuree);
}

QStringList Libeurocod2::getListeMateriaux_gammaM()
{
    QStringList liste;
    QSqlQuery query("SELECT Materiaux FROM gammaM" ,bdd);
    while(query.next()){liste.append(query.value(0).toString());}
    return liste;
}

double Libeurocod2::getgammaM(QString materiau)
{
    return getBDD_Value("gammaM","valeur","Materiaux",materiau);
}

double Libeurocod2::getkH(bool boisMassif, double H_en_mm)
{
    double d=0;
    if(boisMassif)
    {
        if(H_en_mm>=150.0){d=1.0;}
        else{d=qMin(1.3,qPow(150.0/H_en_mm,0.2));}

    }
    else//lamellé collé
    {
        if(H_en_mm>=600.0){d=1;}
        else{d=qMin(1.1,qPow(600.0/H_en_mm,0.1));}
    }
    return d;
}

double Libeurocod2::getksys(int enabled)
{
    double d;
    if(enabled==1){d=1.1;}
    else{d=1.0;}
    return d;
}

double Libeurocod2::getkcrit(double elancement_relatif, bool blocage_deversement)
{
    double kcrit=0;
    if(!blocage_deversement)
    {
    if(elancement_relatif<=0.75){kcrit=1;}
    if(elancement_relatif>0.75 && elancement_relatif<=1.4){kcrit=1.56-(0.75 * elancement_relatif);}
    if(elancement_relatif>1.4){kcrit=1/qPow(elancement_relatif,2);}
    }else{kcrit=1;}

    return kcrit;
}

QStringList Libeurocod2::getPsi_Type()
{
    QStringList liste;
    QSqlQuery query("SELECT Type FROM Psi" );
    while(query.next()){liste.append(query.value(0).toString()),bdd;}
    return liste;
}

QStringList Libeurocod2::getPsi_Categorie()
{
    QStringList liste;
    QSqlQuery query("SELECT Categorie FROM Psi" ,bdd);
    while(query.next()){liste.append(query.value(0).toString());}
    return liste;
}

double Libeurocod2::getpsi0(CategorieCharge categorie)
{

    double d=0;
   // qDebug()<<"Libeurocod2::getpsi0"<<m_categoriesCharges.at(categorie);
    QSqlQuery query("SELECT Psi0 FROM psi WHERE Categorie LIKE '" + m_categoriesCharges.at(categorie)+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

double Libeurocod2::getpsi1(CategorieCharge categorie)
{
    QStringList cat=getPsi_Categorie();
    double d=0;
    QSqlQuery query("SELECT Psi1 FROM psi WHERE Categorie LIKE '" + m_categoriesCharges.at(categorie)+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

double Libeurocod2::getpsi2(CategorieCharge categorie)
{
    double d=0;
    QSqlQuery query("SELECT Psi2 FROM psi WHERE Categorie LIKE '" + m_categoriesCharges.at(categorie)+"'" ,bdd);
    while(query.next()){d=query.value(0).toDouble();}
    return d;
}

double Libeurocod2::getgamma_G(bool G_sup, bool G_inf_EQU)
{
    double d=1;
    if(G_sup && !G_inf_EQU){d=1.35;}
    if(!G_sup && !G_inf_EQU){d=1;}
    if(!G_sup && G_inf_EQU){d=0.9;}
    return d;
}

double Libeurocod2::getgamma_Q()
{
    //1.5 Q (surcharges)
    return 1.5;
}
//page 81

QStringList Libeurocod2::getListeMateriaux_kdef()
{
    QStringList liste;
    QSqlQuery query("SELECT Materiau FROM kdef" ,bdd);
    while(query.next()){liste.append(query.value(0).toString());}
    return liste;
}

double Libeurocod2::getkdef(QString materiau, QString classeService)
{
    QString t;
    QSqlQuery query("SELECT "+classeService+" FROM kdef WHERE Materiau='" + materiau+"'" ,bdd);
    while(query.next()){t.append(query.value(0).toString());}    
    return t.toDouble();
}



QString Libeurocod2::getInfo(QString classeBois, QString carac)// recherche globale dans la bdd des résineux
{
    QString t;
    QSqlQuery query("SELECT "+carac+ " FROM caracBois WHERE Classe='" + classeBois+"'" ,bdd);
    while(query.next()){t.append(query.value(0).toString());}
    return t;

}
//Valeurs normatives bois
double Libeurocod2::getfmk(QString classeBois){return getInfo(classeBois,"fmk").toDouble();}
double Libeurocod2::getft0k(QString classeBois){return getInfo(classeBois,"ft0k").toDouble();}
double Libeurocod2::getft90k(QString classeBois){return getInfo(classeBois,"ft90k").toDouble();}
double Libeurocod2::getfc0k(QString classeBois){return getInfo(classeBois,"fc0k").toDouble();}
double Libeurocod2::getfc90k(QString classeBois){return getInfo(classeBois,"fc90k").toDouble();}
double Libeurocod2::getfvk(QString classeBois){return getInfo(classeBois,"fvk").toDouble();}
double Libeurocod2::getE0mean(QString classeBois){return getInfo(classeBois,"E0mean").toDouble();}
double Libeurocod2::getE05pourc(QString classeBois){return getInfo(classeBois,"E005").toDouble();}
double Libeurocod2::getE90mean(QString classeBois){return getInfo(classeBois,"E90mean").toDouble();}
double Libeurocod2::getGmean(QString classeBois){return getInfo(classeBois,"Gmean").toDouble();}
double Libeurocod2::getMasseVolCarac(QString classeBois){return getInfo(classeBois,"Pk").toDouble();}
double Libeurocod2::getMasseVolMoy(QString classeBois){return getInfo(classeBois,"Pmean").toDouble();}


//Vérifié le 15/07/19
double Libeurocod2::getMomentQuadratiqueRect(double b, double h)
{
    return b*qPow(h,3)/12;
}

//Vérifié le 15/07/19
double Libeurocod2::getMomentQuadratiqueCirc(double diam)
{
    return pi*qPow(diam,4)/64;
}




//------------------------------------------------------------Valeurs normatives acier à faire-------------------------------------------

/// FIN RECHERCHE VALEURS







//-------------------------------------------------------------CALCUL ASSEMBLAGES-----------------------------------------------------------


//Résistance à l'arrachement
//vérifié le 10/07/19
double Libeurocod2::getFaxRk_Pointes(double pk, double d_pointe, double d_tete_pointe, double profondeur_clouage_portee, double profondeur_clouage_porteur, bool annelee)
{
    double result;
    double t;
    double tpene;
    t=profondeur_clouage_porteur;
    tpene=profondeur_clouage_portee;

    if (annelee==true){
        result=qMin(20*qPow(10,-6)*qPow(pk,2)*d_pointe*tpene,70*qPow(10,-6)*qPow(pk,2)*qPow(d_tete_pointe,2));
    }
    else {
        result=qMin(20*qPow(10,-6)*qPow(pk,2)*d_pointe*tpene,70*qPow(10,-6)*qPow(pk,2)*qPow(d_tete_pointe,2)+20*qPow(10,-6)*qPow(pk,2)*d_pointe*t);
    }

    return result;
}

//Vérifié le 10/07/19
double Libeurocod2::getFaxRk_Boulons(double d_ext_rondelle, double d_int_rondelle, double FtRd, double fc90d)
{
    return qMin(FtRd,3*fc90d*pi*(qPow(d_ext_rondelle,2)-qPow(d_int_rondelle,2))/4);
}

//Vérifié le 30/07/2019
double Libeurocod2::getFaxRk_Tirefonds(double nbr_tirefond, double diam, double longueur_penetration_filetage, double angle_axe_fil_bois, double masse_vol_carac)
{
    double lef=longueur_penetration_filetage;
    double kd=qMin(diam/8,1.0);
    double pk=masse_vol_carac;
    double faxk=0.52*qPow(diam,-0.5)*qPow(lef,-0.1)*qPow(pk,0.8);
    double alpha=angle_axe_fil_bois;

    if ((diam>=6)&&(diam<=12)){
        return qPow(nbr_tirefond,0.9)*faxk*diam*lef*kd/(qPow(qSin(alpha*pi/180),2)+1.2*qPow(qCos(alpha*pi/180),2));
    }
    else {
        return qPow(nbr_tirefond,0.9)*faxk*diam*lef/(qPow(qSin(alpha*pi/180),2)+1.2*qPow(qCos(alpha*pi/180),2));
    }

}


//Vérifié le 26/07/2019
double Libeurocod2::getFaxRk_Organes(Libeurocod2::Organe typeElement, double diam, double d_ext_rondelle, double d_int_rondelle, double FtRd, double fc90d, double pk, double d_pointe, double d_tete_pointe, double profondeur_clouage_portee, double profondeur_clouage_porteur, bool annelee)
{
    double result=0.0;

    double t;
    double tpene;
    t=profondeur_clouage_porteur;
    tpene=profondeur_clouage_portee;

    switch(typeElement)
    {
    case Organe::Boulon:
        result=getFaxRk_Boulons(d_ext_rondelle,d_int_rondelle,FtRd,fc90d);
        break;

    case Organe::Agraphe:
        result=0.0;
        break;

    case Organe::Broche:
        result=0.0;
        break;

    case Organe::Tirefond:
        if (diam>6){
            result=getFaxRk_Boulons(d_ext_rondelle,d_int_rondelle,FtRd,fc90d);
        }
        else{
            result=getFaxRk_Pointes(pk,d_pointe,d_tete_pointe,tpene,t,annelee);
        }
        break;


    case Organe::Pointe:
        result=getFaxRk_Pointes(pk,d_pointe,d_tete_pointe,tpene,t,annelee);
        break;

    }

    return result;
}

//Vérifié le 30/07/2019
double Libeurocod2::getNbrEff_Organe_Cisaillment(Libeurocod2::Organe typeElement, double nbr_organes_par_files, double diam, double pince_a1, bool effort_perpendiculaire_au_fil, bool prepercage)
{
    double result_pointe=0.0;
    double result_boulon=0.0;
    double result_agraphe=1.0;
    double a1=pince_a1;
    double n=nbr_organes_par_files;
    double kef=0.0;
    double result=0.0;

    if (a1==4.0*diam){
        if (prepercage){kef=0.5;}
        else{kef=1.0;}
    }
    else {
        if ((a1>4*diam)&&(a1<7*diam)){
            kef=0.5+(0.2/(3*diam))*(a1-4*diam);
        }
        else{
            if((a1>=7*diam)&&(a1<10*diam)){
                kef=0.7+(0.15/(3*diam))*(a1-7*diam);
            }
            else {
                if((a1>=10*diam)&&(a1<14*diam)){
                    kef=0.85+(0.15/(3*diam))*(a1-10*diam);
                }
                else {
                    kef=1.0;
                }
            }
        }
    }


    result_pointe=qPow(n,kef);
    result_boulon=qMin(n,qPow(n,0.9)*qPow(a1/(13*diam),0.25));

    switch(typeElement)
    {
    case Organe::Boulon:
        result=result_boulon;
        break;

    case Organe::Agraphe:
        result=result_agraphe;
        break;

    case Organe::Broche:
        result=result_boulon;
        break;

    case Organe::Tirefond:
        if (diam>6){
            result=result_boulon;
        }
        else{
            result=result_pointe;
        }
        break;


    case Organe::Pointe:
        result=result_pointe;
        break;

    }

    if (effort_perpendiculaire_au_fil){
        return n;
    }
    else{return result;}

}



//Moment d'écoulement plastique

//Vérifié le 10/07/19
double Libeurocod2::getMyrk_Pointes(double fu, double diam, bool circulaire)
{
    double result;
    if(circulaire ==true){
        result = 0.3*fu*qPow(diam,2.6);
    }
    else {
        result = 0.45*fu*qPow(diam,2.6);
    }
    return result;
}

//Vérifié le 10/07/19
double Libeurocod2::getMyrk_Agraphes(double fu, double diam)
{
    return 0.3*fu*qPow(diam,2.6);
}

//Vérifié le 10/07/19
double Libeurocod2::getMyrk_Boulons_Broches(double fu, double diam)
{
    return 0.3*fu*qPow(diam,2.6);
}

//Vérifié le 25/07/19
double Libeurocod2::getMyrk_Tire_Fonds(double fu, double diam, bool circulaire)
{
    if (diam>6){
        return getMyrk_Boulons_Broches(fu,diam);
    }
    else{
        return getMyrk_Pointes(fu,diam,circulaire);
    }
}

//Vérifié le 26/07/19
double Libeurocod2::getMyrk_Organe(double fu, double diam, Libeurocod2::Organe typeElement, bool circulaire)
{

    double result=0.0;

    switch(typeElement)
    {
    case Organe::Boulon:
        result=0.3*fu*qPow(diam,2.6);
        break;

    case Organe::Agraphe:
        result=0.3*fu*qPow(diam,2.6);
        break;

    case Organe::Broche:
        result=0.3*fu*qPow(diam,2.6);
        break;

    case Organe::Tirefond:
        if (diam>6){
            result=0.3*fu*qPow(diam,2.6);
        }
        else{
            if(circulaire ==true){
                result = 0.3*fu*qPow(diam,2.6);
            }
            else {
                result = 0.45*fu*qPow(diam,2.6);
            }
        }
        break;


    case Organe::Pointe:
        if(circulaire ==true){
            result = 0.3*fu*qPow(diam,2.6);
        }
        else {
            result = 0.45*fu*qPow(diam,2.6);
        }
        break;

    }

    return result;
}






//Portance locale

//Vérifié le 26/07/19
double Libeurocod2::getfhkBois(double pk, double diam,double angle_filbois, QString materiau,Organe typeElement, bool prePercage)
{
    double result=0.0;
    double alpha=angle_filbois;
    double k90;

    //obtention de k90
    if ((materiau == "Bois massif") || (materiau =="Lamellé-collé")){
        k90=1.35+0.015*diam;
    }
    else{
        k90=1.30+0.015*diam;
    }

    switch (typeElement)
    {
    case Organe::Boulon:
        result=(0.082*(1-0.01*diam)*pk)/(k90*qPow(qSin(alpha*pi/180),2)+qPow(qCos(alpha*pi/180),2));
        break;

    case Organe::Broche:
        result=(0.082*(1-0.01*diam)*pk)/(k90*qPow(qSin(alpha*pi/180),2)+qPow(qCos(alpha*pi/180),2));
        break;

    case Organe::Tirefond:
        if(diam>6){
            result=(0.082*(1-0.01*diam)*pk)/(k90*qPow(qSin(alpha*pi/180),2)+qPow(qCos(alpha*pi/180),2));
        }
        else{
            if(prePercage)
            {
                result=0.082*(1-0.01*diam)*pk;
            }
            else
            {
                result=0.082*pk*qPow(diam,-0.3);
            }
        }
        break;


    case Organe::Pointe:
        if(diam<=8){
            if(prePercage)
            {
                result=0.082*(1-0.01*diam)*pk;
            }
            else
            {
                result=0.082*pk*qPow(diam,-0.3);
            }
        }
        else {
            result=(0.082*(1-0.01*diam)*pk)/(k90*qPow(qSin(alpha*pi/180),2)+qPow(qCos(alpha*pi/180),2));
        }

        break;

    case Organe::Agraphe:
        result=0.0;
        break;

    }

    return result;

}

//Vérifié le 30/07/2019
double Libeurocod2::getfhkContrePlaque(double pk, double diam, Organe typeElement)
{
    double result=0.0;


    switch(typeElement)
    {
    case Organe::Boulon:
        result=0.11*(1-0.01*diam)*pk;
        break;

    case Organe::Broche:
        result=0.11*(1-0.01*diam)*pk;
        break;

    case Organe::Tirefond:
        if(diam>6){
            result=0.11*(1-0.01*diam)*pk;
        }
        else{
            result= 0.11*pk*qPow(diam,-0.3);
        }
        break;


    case Organe::Pointe:
        result= 0.11*pk*qPow(diam,-0.3);
        break;

    case Organe::Agraphe:
        result=0.0;
        break;
    }
    return result;
}

//Vérifié le 10/07/19
double Libeurocod2::getfhkFibreDurs(double t, double diam)
{
    return 30*qPow(diam,-0.3)*qPow(t,0.6);
}


double Libeurocod2::getfhkOSBParticules(double t, double diam,Organe typeElement)
{
    double result = 0.0;

    /*if (boulon)
    {
        result = 50*qPow(diam,-0.6)*qPow(t,0.2);
    }
    else {
        result = 65*qPow(diam,-0.7)*qPow(t,0.1);
    }*/

    switch(typeElement)
    {
    case Organe::Boulon:
        result = 50*qPow(diam,-0.6)*qPow(t,0.2);
        break;

    case Organe::Broche:
        result = 50*qPow(diam,-0.6)*qPow(t,0.2);
        break;

    case Organe::Tirefond:
        if(diam>6){
            result = 50*qPow(diam,-0.6)*qPow(t,0.2);
        }
        else{
            result = 65*qPow(diam,-0.7)*qPow(t,0.1);
        }
        break;


    case Organe::Pointe:
        result = 65*qPow(diam,-0.7)*qPow(t,0.1);
        break;

    case Organe::Agraphe:
        result=0.0;
        break;
    }

    return result;
}


//CISAILLEMENT ASSEMBLAGES BOIS-BOIS

//Vérifié le 26/07/19
double Libeurocod2::getPourcentJohansen(Libeurocod2::Organe typeElement)
{
    double pourcent=0.0;
    switch(typeElement)
    {
    case Organe::Boulon:
        pourcent=0.25;
        break;

    case Organe::Broche:
        pourcent=0.00;
        break;

    case Organe::Tirefond:
            pourcent=1.0;
    break;

    case Organe::Pointe:
        pourcent=0.15;
        break;

    case Organe::Agraphe:
        pourcent=0.0;
        break;
    }

    return pourcent;
}






// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_A(double fh1k, double t1, double diam)
{
    double d=0;
    d=fh1k*t1*diam;
    return qRound(d);
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_B(double fh2k, double t2, double diam)
{
    double d=0;
    d=fh2k*t2*diam;
    return qRound(d);
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_C(double fh1k, double t1, double t2, double diam, double beta,Organe typeElement, double FaxRk)
{

    double result=0;
    result=(((fh1k*t1*diam)/(1+beta))*(qPow((beta+((2*qPow(beta,2)))*(1+(t2/t1)+qPow((t2/t1),2)))+(qPow(beta,3)*qPow((t2/t1),2)),0.5)-(beta*(1+(t2/t1)))));

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_D(double fh1k, double t1, double diam, double beta, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0;
    result=((1.05*((fh1k*t1*diam)/(2+beta)))*(qPow(2*beta*(1+beta)+(4*beta*(2+beta)*MyRk)/(fh1k*diam*qPow(t1,2)),0.5)-beta));

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result + qMin(pourcent*result,FaxRk/4));
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_E(double fh1k, double t2, double diam, double beta, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0;
    result=((1.05*((fh1k*t2*diam)/(1+(2*beta))))*(qPow(2*qPow(beta,2)*(1+beta)+(4*beta*(1+(2*beta))*MyRk)/(fh1k*diam*qPow(t2,2)),0.5)-beta));

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_F(double fh1k, double diam, double beta, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0;
    result=((1.15*qPow((2*beta)/(1+beta),0.5)*qPow(2*MyRk*fh1k*diam,0.5)));

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

//Vérifé le 26/07/19
double Libeurocod2::getFvrk_B_Simple_Cisaillement(double fh1k, double fh2k, double t1, double t2, double diam, double MyRk, Organe typeElement, double FaxRk, bool organe_en_bois_de_bout)
{
    double beta =fh2k/fh1k;
    double result=0.0;

    if (organe_en_bois_de_bout){
        result= (1.0/3.0)*qMin(getFvrk_B_A(fh1k,t1,diam),qMin(getFvrk_B_B(fh2k,t2,diam),qMin(getFvrk_B_C(fh1k,t1,t2,diam,beta,typeElement,FaxRk),qMin(getFvrk_B_D(fh1k,t1,diam,beta,MyRk,typeElement,FaxRk),qMin(getFvrk_B_E(fh1k,t2,diam,beta,MyRk,typeElement,FaxRk),getFvrk_B_F(fh1k,diam,beta,MyRk,typeElement,FaxRk))))));

    }
    else {
        result= qMin(getFvrk_B_A(fh1k,t1,diam),qMin(getFvrk_B_B(fh2k,t2,diam),qMin(getFvrk_B_C(fh1k,t1,t2,diam,beta,typeElement,FaxRk),qMin(getFvrk_B_D(fh1k,t1,diam,beta,MyRk,typeElement,FaxRk),qMin(getFvrk_B_E(fh1k,t2,diam,beta,MyRk,typeElement,FaxRk),getFvrk_B_F(fh1k,diam,beta,MyRk,typeElement,FaxRk))))));
    }

    return result;
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_G(double fh1k, double t1, double diam)
{
    return qRound(fh1k*t1*diam);
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_H(double fh2k, double t2, double diam)
{
    return qRound(0.5*fh2k*t2*diam);
}



// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_J(double fh1k, double t1, double diam, double beta, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=(1.05*fh1k*t1*diam/(2+beta))*(qSqrt(2*beta*(1+beta)+(4*beta*(2+beta)*MyRk)/(fh1k*qPow(t1,2)*diam))-beta);

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_B_K(double fh1k, double diam, double beta, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=1.15*qSqrt((2*beta/(1+beta)))*qSqrt(2*MyRk*fh1k*diam);

    double pourcent = 0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

//Vérifié le 26/07/19
double Libeurocod2::getFvrk_B_Double_Cisaillement(double fh1k, double fh2k, double t1, double t2, double diam, double MyRk, Organe typeElement, double FaxRk, bool organe_en_bois_de_bout)
{
    double beta = fh2k/fh1k;

    if(organe_en_bois_de_bout){
        return (1.0/3.0)*qMin(getFvrk_B_G(fh1k,t1,diam),qMin(getFvrk_B_H(fh2k,t2,diam),qMin(getFvrk_B_J(fh1k,t1,diam,beta,MyRk,typeElement,FaxRk),getFvrk_B_H(fh2k,t2,diam))));

    }
    else {
        return qMin(getFvrk_B_G(fh1k,t1,diam),qMin(getFvrk_B_H(fh2k,t2,diam),qMin(getFvrk_B_J(fh1k,t1,diam,beta,MyRk,typeElement,FaxRk),getFvrk_B_H(fh2k,t2,diam))));
    }
}





//CISAILLEMENT ASSEMBLAGES BOIS-METAL

// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_A(double fhk, double t1, double diam)
{
    return qRound(0.4*fhk*t1*diam);
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_B(double fhk, double diam, double MyRk, Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=1.15*qSqrt(2*MyRk*fhk*diam);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);


    return qRound(result+qMin(pourcent*result,FaxRk/4));
}


//Vérfié le 26/07/19
double Libeurocod2::getFvrk_M_SimpleCisaillement(double fhk, double t1, double diam, double MyRk, double epaisseur_plaque, Organe typeElement, double FaxRk)
{
    double e=epaisseur_plaque;
    double result_plaque_mince;
    double result_plaque_epaisse;

    result_plaque_mince=qMin(getFvrk_M_A(fhk,t1,diam),getFvrk_M_B(fhk,diam,MyRk,typeElement, FaxRk));
    result_plaque_epaisse=qMin(getFvrk_M_C(fhk,t1,diam),qMin(getFvrk_M_D(fhk,t1,diam,MyRk,typeElement, FaxRk),getFvrk_M_E(fhk,diam,MyRk,typeElement, FaxRk)));

    if (e<=0.5*diam){
        return result_plaque_mince;
    }
    else{
        if (e>=diam){
            return result_plaque_epaisse;
        }
        else{
            return result_plaque_mince+((result_plaque_epaisse-result_plaque_mince)/(0.5*diam))*(e-0.5*diam);
        }
    }
}


// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_C(double fhk, double t1, double diam)
{
    return qRound(fhk*t1*diam);
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_D(double fhk, double t1, double diam, double MyRk,Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=fhk*t1*diam*(qSqrt(2+4*MyRk/(fhk*diam*qPow(t1,2)))-1);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_E(double fhk, double diam, double MyRk,Organe typeElement, double FaxRk)
{

    double result=0.0;
    result=2.3*qSqrt(MyRk*diam*fhk);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}


// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_F(double fh1k, double t1, double diam)
{
    return qRound(fh1k*t1*diam);
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_G(double fh1k, double t1, double diam, double MyRk,Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=fh1k*t1*diam*(qSqrt(2+4*MyRk/(fh1k*diam*qPow(t1,2)))-1);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_H(double fh1k, double diam, double MyRk,Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=2.3*qSqrt(MyRk*diam*fh1k);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}

//Vérifié le 26/07/2019
double Libeurocod2::getFvrk_M_DoubleCisaillement_PlaqueCentrale(double fh1k, double t1, double diam, double MyRk,Organe typeElement, double FaxRk)
{   
    return qMin(getFvrk_M_F(fh1k,t1,diam),qMin(getFvrk_M_G(fh1k,t1,diam,MyRk,typeElement, FaxRk),getFvrk_M_H(fh1k,diam,MyRk,typeElement, FaxRk)));
}


// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_J(double fh2k, double t2, double diam)
{
    return qRound(0.5*fh2k*t2*diam);
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_K(double fh2k, double diam, double MyRk,Organe typeElement, double FaxRk)
{
    double result=0.0;
    result=1.15*qSqrt(2*MyRk*fh2k*diam);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_L(double fh2k, double t2, double diam)
{
    return qRound(0.5*fh2k*t2*diam);
}
// Vérifié le 10/07/19
double Libeurocod2::getFvrk_M_M(double fh2k, double diam, double MyRk,Organe typeElement, double FaxRk)
{

    double result=0.0;
    result=2.3*qSqrt(MyRk*fh2k*diam);

    double pourcent=0.0;
    pourcent = getPourcentJohansen(typeElement);

    return qRound(result+qMin(pourcent*result,FaxRk/4));
}


//Vérifié le 26/07/19
double Libeurocod2::getFvrk_M_DoubleCisaillement_PlaquesExternes(double fh2k, double t2, double diam, double MyRk, double epaisseur_plaque,Organe typeElement, double FaxRk)
{
    double e=epaisseur_plaque;
    double result_plaque_mince;
    double result_plaque_epaisse;

    result_plaque_mince=qMin(getFvrk_M_J(fh2k,t2,diam),getFvrk_M_K(fh2k,diam,MyRk,typeElement, FaxRk));
    result_plaque_epaisse=qMin(getFvrk_M_L(fh2k,t2,diam),getFvrk_M_M(fh2k,diam,MyRk,typeElement, FaxRk));

    if (e<=0.5*diam){
        return result_plaque_mince;
    }
    else{
        if (e>=diam){
            return result_plaque_epaisse;
        }
        else{
            return result_plaque_mince+((result_plaque_epaisse-result_plaque_mince)/(0.5*diam))*(e-0.5*diam);
        }
    }
}

//Vérifié le 30/07/2019
double Libeurocod2::getResistanceFendage(double largeur, double hauteur, double distance_rive_charge_organe_leplus_eloigne,bool assemblage_en_bois_de_bout, bool plaque_emboutie, double largeur_plaque)
{
    double b=largeur;
    double h = hauteur;
    double he=distance_rive_charge_organe_leplus_eloigne;
    double w=0.0;

    if (plaque_emboutie){
        w=qMax(qPow(largeur_plaque/100,0.35),1.0);
    }
    else{w=1.0;}

    if (assemblage_en_bois_de_bout){
        return (1.0/3.0)*14*b*w*qSqrt(he/(1-he/h));
    }
    else{
        return 14*b*w*qSqrt(he/(1-he/h));
    }

}

/*--------------------------------------------------CONDITIONS DE PINCES-------------------------------------------------*/
//Conditions de pinces dans le bois


//Pointes
//Toutes vérifiées le 26/07/19
double Libeurocod2::getPince_Pointes_a1(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk = pk_bois;
    double result;

    if (prepercage){
        result =(4+qAbs(qCos(alpha*pi/180)))*diam;
    }
    else{
        if(pk<=420){
            if(diam<5){
                result=(5+5*qAbs(qCos(alpha*pi/180)))*diam;
            }
            else{
                result =(5+7*qAbs(qCos(alpha*pi/180)))*diam;
            }
        }
        else{
            result=(7+8*qAbs(qCos(alpha*pi/180)))*diam;
        }
    }

    return result;
}

double Libeurocod2::getPince_Pointes_a2(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk = pk_bois;
    double result;

    if (prepercage){
        result =(3+qAbs(qSin(alpha*pi/180)))*diam;
    }
    else{
        if(pk<=420){
            result=5*diam;
        }
        else{
            result = 7*diam;
        }
    }

    return result;
}

double Libeurocod2::getPince_Pointes_a3c(double diam, double pk_bois, bool prepercage)
{
    double pk = pk_bois;
    double result;

    if (prepercage){
        result =7*diam;
    }
    else{
        if(pk<=420){
            result=10*diam;
        }
        else{
            result = 15*diam;
        }
    }

    return result;
}

double Libeurocod2::getPince_Pointes_a3t(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk = pk_bois;
    double result;

    if (prepercage){
        result =(7+5*qAbs(qCos(alpha*pi/180)))*diam;
    }
    else{
        if(pk<=420){
            result=(10+5*qAbs(qCos(alpha*pi/180)))*diam;
        }
        else{
            result =(15+5*qAbs(qCos(alpha*pi/180)))*diam;
        }
    }

    return result;
}

double Libeurocod2::getPince_Pointes_a4c(double diam, double pk_bois, bool prepercage)
{

    double pk = pk_bois;
    double result;

    if (prepercage){
        result =3*diam;qDebug()<<"prepercage = true";
    }
    else{
        if(pk<=420){
            result=5*diam;
        }
        else{
            result = 7*diam;
        }
    }

    return result;
}


double Libeurocod2::getPince_Pointes_a4t(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk = pk_bois;
    double result;

    if (prepercage){
        if (diam<5){
            result =(3+2*qAbs(qSin(alpha*pi/180)))*diam;
        }
        else{
            result=(3+4*qAbs(qSin(alpha*pi/180)))*diam;
        }

    }
    else{
        if(pk<=420){
            if(diam<5){
                result=(5+2*qAbs(qSin(alpha*pi/180)))*diam;
            }
            else{
                result =(5+5*qAbs(qSin(alpha*pi/180)))*diam;
            }
        }
        else{
            if(diam<5){
                result=(7+2*qAbs(qSin(alpha*pi/180)))*diam;
            }
            else{
                result =(7+5*qAbs(qSin(alpha*pi/180)))*diam;
            }
        }
    }

    return result;
}

//Toutes vérifiées le 26/07/19
//Agraphes
double Libeurocod2::getPince_Agraphes_a1(double diam, double angle_effort_filbois, double angle_agraphes_filbois)
{
    double alpha=angle_effort_filbois;
    double theta = angle_agraphes_filbois;

    if (theta>=30){
        return (10+5*qAbs(qCos(alpha*pi/180)))*diam;
    }
    else{
        return (15+5*qAbs(qCos(alpha*pi/180)))*diam;
    }

}

double Libeurocod2::getPince_Agraphes_a2(double diam)
{

    return 15*diam;
}

double Libeurocod2::getPince_Agraphes_a3c(double diam)
{

    return 15*diam;
}

double Libeurocod2::getPince_Agraphes_a3t(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return (15+5*qAbs(qCos(alpha*pi/180)))*diam;
}

double Libeurocod2::getPince_Agraphes_a4c(double diam)
{

    return 10*diam;
}


double Libeurocod2::getPince_Agraphes_a4t(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return (15+5*qAbs(qSin(alpha*pi/180)))*diam;
}


//Boulons
//Toutes verifiées le 26/07/19
double Libeurocod2::getPince_Boulons_a1(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return (4+qAbs(qCos(alpha*pi/180)))*diam;
}

double Libeurocod2::getPince_Boulons_a2(double diam)
{

    return 4*diam;
}

double Libeurocod2::getPince_Boulons_a3c(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;
    double result;

    if(alpha>=90 && alpha<150){
        result=(1+6*qSin(alpha*pi/180))*diam;
    }
    else{
        if(alpha>=150 && alpha<210){
            result = 4*diam;
        }
        else{
            result=(1+6*qSin(alpha*pi/180))*diam;
        }
    }

    return result;
}


double Libeurocod2::getPince_Boulons_a3t(double diam)
{

    return qMax(7*diam,80.0);
}

double Libeurocod2::getPince_Boulons_a4c(double diam)
{

    return 3*diam;
}

double Libeurocod2::getPince_Boulons_a4t(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return qMax((2+2*qSin(alpha*pi/180))*diam,3*diam);
}

//Toutes vérifiées le 26/07/19
//Broches
double Libeurocod2::getPince_Broches_a1(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return (3+2*qAbs(qCos(alpha*pi/180)))*diam;
}

double Libeurocod2::getPince_Broches_a2(double diam)
{

    return 3*diam;
}

double Libeurocod2::getPince_Broches_a3c(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;
    double result;

    if(alpha>=90 && alpha<150){
        result=getPince_Broches_a3t(diam)*qAbs(qSin(alpha*pi/180));
    }
    else{
        if(alpha>=150 && alpha<210){
            result = qMax(3.5*diam,40.0);
        }
        else{
            result=getPince_Broches_a3t(diam)*qAbs(qSin(alpha*pi/180));
        }
    }

    return result;
}

double Libeurocod2::getPince_Broches_a3t(double diam)
{
    return qMax(7*diam,80.0);
}

double Libeurocod2::getPince_Broches_a4c(double diam)
{

    return 3*diam;
}

double Libeurocod2::getPince_Broches_a4t(double diam, double angle_effort_filbois)
{
    double alpha=angle_effort_filbois;

    return qMax((2+2*qSin(alpha*pi/180))*diam,3*diam);
}

//Toutes vérifiées le 26/07/19
//Tire-fonds
double Libeurocod2::getPince_Tirefonds_a1(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a1(diam,alpha);
    }
    else{
        return getPince_Pointes_a1(diam,pk,alpha,prepercage);
    }

}

double Libeurocod2::getPince_Tirefonds_a2(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a2(diam);
    }
    else{
        return getPince_Pointes_a2(diam,pk,alpha,prepercage);
    }

}

double Libeurocod2::getPince_Tirefonds_a3c(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a3c(diam,alpha);
    }
    else{
        return getPince_Pointes_a3c(diam,pk,prepercage);
    }

}

double Libeurocod2::getPince_Tirefonds_a3t(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a3t(diam);
    }
    else{
        return getPince_Pointes_a3t(diam,pk,alpha,prepercage);
    }

}

double Libeurocod2::getPince_Tirefonds_a4c(double diam, double pk_bois, bool prepercage)
{
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a4c(diam);
    }
    else{
        return getPince_Pointes_a4c(diam,pk,prepercage);
    }

}

double Libeurocod2::getPince_Tirefonds_a4t(double diam, double pk_bois, double angle_effort_filbois, bool prepercage)
{
    double alpha=angle_effort_filbois;
    double pk =pk_bois;

    if (diam>6){
        return getPince_Boulons_a4t(diam,alpha);
    }
    else{
        return getPince_Pointes_a4t(diam,pk,alpha,prepercage);
    }

}

//Conditions de pince dans le métal
//Toutes les pinces métales vérifiées le 26/07/19
//Pinces max
double Libeurocod2::getPinceMetalMax_e1( double epaisseur_plaque, bool en_exterieur)
{
    double t=epaisseur_plaque;

    if (en_exterieur){
        return 40+4*t;
    }
    else{
        return qMin(8*t,125.0);
    }

}

double Libeurocod2::getPinceMetalMax_e2( double epaisseur_plaque, bool en_exterieur)
{
    double t=epaisseur_plaque;

    if (en_exterieur){
        return 40+4*t;
    }
    else{
        return qMin(8*t,125.0);
    }

}

double Libeurocod2::getPinceMetalMax_p1( double epaisseur_plaque, bool en_exterieur)
{
    double t=epaisseur_plaque;

    if (en_exterieur){
        return qMin(14*t,200.0);
    }
    else{
        return qMin(14*t,175.0);
    }

}


double Libeurocod2::getPinceMetalMax_p2( double epaisseur_plaque, bool en_exterieur)
{
    double t=epaisseur_plaque;

    if (en_exterieur){
        return qMin(14*t,200.0);
    }
    else{
        return qMin(14*t,175.0);
    }

}

//Pinces min


double Libeurocod2::getPinceMetalMin_e1(double diam_percage)
{
    double d=diam_percage;

    return 1.2*d;
}

double Libeurocod2::getPinceMetalMin_e2(double diam_percage)
{
    double d=diam_percage;

    return 1.2*d;
}


double Libeurocod2::getPinceMetalMin_p1(double diam_percage)
{
    double d=diam_percage;

    return 2.2*d;
}

double Libeurocod2::getPinceMetalMin_p2(double diam_percage)
{
    double d=diam_percage;

    return 2.4*d;
}




//-----------------------------------------------------VERIFICATION DES PIECES METALLIQUES-------------------------------------------------
//Vérifié le 11/07/19
double Libeurocod2::getFtrd_Boulon(double fu, double diam, double gammaM2)
{
    return 0.9*fu*0.78*pi*qPow(diam/2,2)/gammaM2;
}

//Vérifié le 11/07/19
double Libeurocod2::getResistancePlatTraction(double epaisseur, double hauteur, double Anet, double fu, double fy, double gammaM0, double gammaM2)
{
    return qMin(0.9*Anet*epaisseur*fu/gammaM2,epaisseur*hauteur*fy/gammaM0);
}

//érifié le 30/07/2019
double Libeurocod2::getVerifPercagePlatFlexion(double epaisseur_plat, double hauteur_plat, double nbr_percage, double diam_percage, double fu, double fy, double gammaM0, double gammaM2)
{
    double e =epaisseur_plat;
    double h = hauteur_plat;
    double d=diam_percage;
    double n =nbr_percage;
    double anet=e*(h-n*d);
    double a =e*h;

    return ((a*fy/gammaM0)*100/(0.9*anet*fu/gammaM2));
}

//Vérifié le 30/07/2019
double Libeurocod2::getResistancePlatFlexion(double wely, double fy, double gammaM0, double importance_percage)
{
    double result;
    if (importance_percage <100){
        result = wely*fy/gammaM0;
    }
    else {
        result=0.4*wely*fy/gammaM0;
    }

    return result;
}



//Vérifié le 11/07/19
double Libeurocod2::getResistanceCordonFrontal(double gorge, double longueur, double fu, double betaw, double gammaM2)
{
    return gorge*longueur*fu/(qSqrt(2)*betaw*gammaM2);
}

//Vérifié le 11/07/19
double Libeurocod2::getResistanceCordonLateral(double gorge, double longueur, double fu, double betaw, double gammaM2)
{
    return gorge*longueur*fu/(qSqrt(3)*betaw*gammaM2);
}

//Vérifié le 11/07/19
double Libeurocod2::getResistanceCordonOblique(double gorge, double longueur, double fu, double betaw, double gammaM2, double angle)
{
    return gorge*longueur*fu/(qSqrt(3-qPow(qSin(angle*pi/180),2))*betaw*gammaM2);
}


//----------------------------------------------------------------------Vérification du plancher---------------------------------------
//Vérifié le 19/07/19
double Libeurocod2::getPlancherFrequence(double longueur_solive_mm, double m, double EIl)
{
    return  (pi/(2*qPow(longueur_solive_mm/1000,2)))*qSqrt(EIl*qPow(10,-6)/m);
}

double Libeurocod2::getPlancherRigidite(double E, double bande_chargement, double I)
{
    return E*I*qPow(10,15)/bande_chargement;
}

//Vérifié le 19/07/19
double Libeurocod2::getPlanchern40(double frequence, double longueur_solive, double largeur_plancher, double EIl, double EIb)
{
    return qPow((qPow(40/frequence,2)-1)*qPow(largeur_plancher/longueur_solive,4)*EIl/EIb,0.25);
}

//Vérifié le 19/07/19
double Libeurocod2::getPlancherVitesse(double n40, double m, double largeur_plancher, double longueur_solive)
{
    return ((4*(0.4+0.6*n40))/(m*largeur_plancher/1000*longueur_solive/1000+200))*1000;
}

//Vérifié le 19/07/19
double Libeurocod2::getPlancherVitessemax(double b, double epsilon, double frequence)
{
    return qPow(b,frequence*epsilon-1)*1000;
}

double Libeurocod2::getPlancherSouplesse_solive(double longueur_solive, double E, double I)
{
    return qPow(longueur_solive,3)/(48*E*I);
}

double Libeurocod2::getPlancherSouplesse_panneau(double bande_chargement, double epaisseur_panneau, double largeur_panneau, double E)
{
    return qPow(bande_chargement,3)*12/(E*largeur_panneau*qPow(epaisseur_panneau,3));
}

//Vérifié le 19/07/19
double Libeurocod2::getPlancherDeplacement(double ks, double kp)
{
    return ((ks*(36*qPow(ks,2)+108*ks*kp+7*qPow(kp,2)))/(180*qPow(ks,2)+204*ks*kp+7*qPow(kp,2)))*1000;
}





//---------------------------------------------------------VERIFICATION DES POUTRES--------------------------------------------------------

//Vérification de la Flexion
double Libeurocod2::getResistanceFlexion(double fmK, double kmod, double gammaM, double ksys,double kh)
{
    double fmd=0;
    fmd=fmK*(kmod/gammaM)*ksys*kh;
    return fmd;
}

double Libeurocod2::getContrainteFlexion(double P,double coord_P,double Lg,double IVy)
{
    ///Anciennement pour une charge uniforme répartie
    /*
    double sigma_md=0;double moment_Fy=0;    
    moment_Fy= (Q*qPow(L,2))/8.0; ///Charge uniforme    
    if(P>0){moment_Fy=moment_Fy + ((P *coord_P*(L-coord_P))/L);} /// charge ponctuelle
    sigma_md=(6.0/(b*qPow(H,2))) * moment_Fy;
    return sigma_md;
    */
    double Mfy=0;double Lga=0;double Lgb=0;double sigma_md=0;
    Lga=coord_P;Lgb= Lg-coord_P;
    Mfy=(P *Lga * Lgb)/Lg;
    sigma_md = Mfy / IVy ;
    return sigma_md;
}

//Vérifié le 15/07/19
double Libeurocod2::getEntaille_kn(QString type_bois)
{
    double result;

    if (type_bois=="Bois massif")
    {
        result=5;
    }
    else
    {
        if (type_bois=="Lamellé-collé")
        {
            result = 6.5;
        }
        else
        {
            result = 4.5;
        }
    }

    return result;
}

//Vérifié le 15/07/19
double Libeurocod2::getEntaillePente_i(double h_entaille, double b_entaille)
{
    return b_entaille/h_entaille;
}

//Vérifié le 15/07/19
double Libeurocod2::getEntaille_alpha(double h_barre, double h_entaille)
{
    return (h_barre-h_entaille)/h_barre;
}



//Vérifié le 12/07/19
double Libeurocod2::getEntaille_kv(double hauteur_poutre, double pente_entaille_i, double rapport_hauteur_alpha, double kn, double distance_appui_x, bool entaille_opposee_appui)
{
    double alpha=rapport_hauteur_alpha;
    double i = pente_entaille_i;
    double x =distance_appui_x;
    double h = hauteur_poutre;
    double kv;

    if (entaille_opposee_appui==true){
        kv=1;
    }
    else
    {

        kv=kn*(1+((1.1*qPow(i,1.5))/qSqrt(h)))/(qSqrt(h)*(qSqrt(alpha*(1-alpha))+0.8*(x/h)*qSqrt((1/alpha)-qPow(alpha,2))));
    };

    return kv;
}

//Vérifié le 16/07/19
double Libeurocod2::getResistanceCisaillement(double fvk, double kmod, double gammaM, bool assemblage_en_bois_de_bout)
{
    if(assemblage_en_bois_de_bout){
        return (1.0/3.0)*kmod*fvk/gammaM;
    }
    else {
        return kmod*fvk/gammaM;
    }
}

//Vérifié le 16/07/19
double Libeurocod2::getContrainteCisaillement(double effort_tranchant_en_DaN, double kcr, double b, double hef)
{
    /*qDebug()<<"Libeurocod2::getContrainte Cisaillement";
    qDebug()<<"F:"<<effort_tranchant_en_DaN<<" , kcr:"<<kcr<<" b:"<<b<<" , hef:"<<hef;*/
    double F=effort_tranchant_en_DaN;
            F=F*10;
    return 1.5*F/(kcr*b*hef);
}

//Vérifié le 15/07/19
double Libeurocod2::getCompression_kc90(QString type_bois, double h_barre, double l1, bool appui_continu)
{
    double result;
    double h =h_barre;
    if (h<300)
    {
        if (l1<2*h)
        {
           result =1 ;
        }
        else
        {
            if (appui_continu)
            {
                if (type_bois=="Bois massif")
                {
                    result=1.25;
                }
                else
                { if (type_bois=="Lamellé-collé")
                    {result=1.5;}
                    else
                    {
                        result=1;
                    }

                }

            }
            else
            {
                if (type_bois=="Bois massif")
                {
                    result=1.5;
                }
                else
                { if (type_bois=="Lamellé-collé")
                    {result=1.75;}
                    else
                    {
                        result=1;
                    }

                }
            }

        }
    }
    else
    {
        if (type_bois=="Bois massif")
        {
            result=1.5;
        }
        else
        {
            result=1.75;
        }
    }
    return result;
}





//Vérification de la compression transversale
//Vérifié le 16/07/19
double Libeurocod2::getContrainteCompressionTransversale(double Effort_en_DaN, double lef_mm, double largeur)///page 58
{
    double d=0;
    Effort_en_DaN=Effort_en_DaN*10.0;
    d=Effort_en_DaN/(lef_mm*largeur);
    return d;
}

//Vérifié le 16/07/19
double Libeurocod2::getResistanceCompressionTransversale(double fc90k, double kmod, double gammaM)
{
    double fc90d=0;
    fc90d=fc90k*(kmod/gammaM);
    return fc90d;
}



// vérifiaction de la Traction
double Libeurocod2::getContrainteTractionAxiale(double effort_en_DaN, double aire_mm2)//EC5 page 54
{
    double d=0;
    effort_en_DaN=effort_en_DaN*10.0;
    d=effort_en_DaN/aire_mm2;
    return d;
}

double Libeurocod2::getResistanceTractionAxiale(double ft0k, double kmod, double gammaM, double kH)
{
    double ft0d=0;
    ft0d=ft0k*(kmod/gammaM)*kH;
    return ft0d;
}

double Libeurocod2::getLongueurEfficaceCompression(double l_appui, double a, double l1)
{
    return l_appui+qMin(l_appui,qMin(l1/2,30.0))+qMin(l_appui,qMin(a,30.0));
}



double Libeurocod2::getTauxtravail(double contrainte, double resistance)
{
    return (contrainte/resistance)*100.0;
}

double Libeurocod2::getTauxtravailCompression(double contrainte, double resistance, double kc90)
{
    return (contrainte/(resistance * kc90))*100.0;
}

//Vérification de la compression axiale
double Libeurocod2::getContrainteCompressionAxiale(double effort_en_DaN, double aire_mm2)///page 58
{
    double d=0;
    effort_en_DaN=effort_en_DaN*10.0;
    d=effort_en_DaN/aire_mm2;
    return d;
}

double Libeurocod2::getResistanceCompressionAxiale(double fc0k, double kmod, double gammaM)
{
    double fc0d=0;
    fc0d=fc0k*(kmod/gammaM);
    return fc0d;
}




//CALCUL DE FLECHES
double Libeurocod2::getWinstQ(double Lg, double E0mean, double I, double bande_charge, double Q_inst)///cacul pour une charge uniforme
{
    bande_charge=bande_charge/1000.0;
    double effort=bande_charge*Q_inst;
    effort=effort/100.0;//conversion en N/mm

    return ((5 * effort * qPow(Lg,4)) / (384 * E0mean * I));
}

double Libeurocod2::getW_nodale(double F, double coord, double Lg_barre, double E0mean, double I)///cacul pour une charge nodale
{
    if(coord> Lg_barre/2.0){coord=Lg_barre-coord;}
    return ((F * coord)/(2.0 * E0mean * I)) * ((qPow(Lg_barre,2)/8.0)-(qPow(coord,2)/6.0));
}

double Libeurocod2::getWinst(double Lg, double E0mean, double I, double bande_charge, double G, double Q, double S, double psi0)
{
    double action_variable=0;
   // si on a 2 actions variables
    if (S>0 && Q>0)/// Pour l'instant surcharges prioritaire sur neige
    {
        action_variable=Q + (S * psi0);
    }
    else
    {
        if(Q>0){action_variable=Q;}
        else{if(S>0){action_variable=S;}}
    }

    bande_charge=bande_charge/1000.0;


    double effort=bande_charge*(G+action_variable);
    effort=effort/100.0;//conversion en N/mm
    return ((5 * effort * qPow(Lg,4)) / (384 * E0mean * I));
}

double Libeurocod2::getWcreep(double Lg, double E0mean, double I, double bande_charge, double G, double psi2, double kdef, double Q)
{

    double action_variable=0;
    action_variable=Q * psi2;
    bande_charge=bande_charge/1000.0;
    double effort=bande_charge*kdef*(G+action_variable);
    effort=effort/100.0;//conversion en N/mm
    return ((5 * effort * qPow(Lg,4)) / (384 * E0mean * I));
}

double Libeurocod2::getWinstQ_lim(double Lg,QString element,bool Agricole)
{
    QString t;double fleche=0;
    QString requete;
    if(!Agricole){requete.append("SELECT Winst FROM Fleches WHERE Element='" + element+"' AND Element NOT LIKE 'Agricole%'");}
    else{requete.append("SELECT Winst FROM Fleches WHERE Element='Agricole_" + element+"'");}
    QSqlQuery query(requete,bdd);
    while(query.next()){t.append(query.value(0).toString());}
    fleche=Lg / t.toDouble();
    return fleche;
}

double Libeurocod2::getWnetfin_lim(double Lg, QString element, bool Agricole)
{
    QString t;double fleche=0;
    QString requete;
    if(!Agricole){requete.append("SELECT Wnetfin FROM Fleches WHERE Element='" + element+"' AND Element NOT LIKE 'Agricole%'");}
    else{requete.append("SELECT Wnetfin FROM Fleches WHERE Element='Agricole_" + element+"'");}

    QSqlQuery query(requete,bdd);
    while(query.next()){t.append(query.value(0).toString());}
    fleche=Lg / t.toDouble();
    return fleche;
}




double Libeurocod2::getContrainteCritique(double E005, double b, double H, double Lg,bool charge_repartie,bool porte_a_faux,bool dessus_fibre_neutre)
{
    double L_eff,klef;
    //page 82
    if(!porte_a_faux)//Sur appuis simples
    {
        if(charge_repartie){klef=0.9;}//charge répartie
        else{klef=0.8;}//charge ponctuelle
    }
    else// sur un porte à faux
    {
        if(charge_repartie){klef=0.5;}//charge répartie
        else{klef=0.8;}//charge ponctuelle
    }
    L_eff=Lg*klef;
    // chargement sur fibre comprimé(haut de poutre)
    if(dessus_fibre_neutre){L_eff=L_eff +(2.0*H);}
    //chargement sur fibre tendu (bas de poutre)
    else{L_eff=L_eff -(0.5*H);}


    double d=0;
    d=(0.78*E005*qPow(b,2))/(H*L_eff);
    return d;
}

double Libeurocod2::getElancement_relatif_flexion(double sigma_m_crit, double fmk)
{
    return qPow((fmk/sigma_m_crit),0.5);
}



std::vector<double> Libeurocod2::getElancement_m_Lf(int articule_tete, bool articule_pied)
{
    std::vector <double> sortie(2);
    //Page 59
    // index 0: m, coefficient permettant de définir la longueur de flambement en fonction des liaisons
    // index 1: Lf, longueur de flambement suivant la longueur de la barre


    /// influence des assemblages sur la longueur de flambement
    if(!articule_pied)
    {
        switch (articule_tete)
        {
            case 0:///encastré en pied et en tête
            sortie.at(0)=0.5;sortie.at(1)=0.5;
            break;

        case 1:///encastré en pied articulé en tête
           sortie.at(0)=0.7;sortie.at(1)=0.7;
            break;
        case 2:/// encastré en pied libre en tête(ex:poteau isolé)
            sortie.at(0)=2.0;sortie.at(1)=2.0;

        default :
            break;
            sortie.at(0)=0.7;sortie.at(1)=0.7;
        }
    }
    else///articulé en pied et en tête
    {sortie.at(0)=1;sortie.at(1)=1;}

    return sortie;

}




// calcul de l'elancement d'une barre

std::vector<double> Libeurocod2::getElancement_relatif(double Lg_barre_y,double Lg_barre_z, double b, double H, double fc0k, double E005, int articule_tete, bool articule_pied,bool bois_Massif)
{
    std::vector<double> lambda(2);
    double el_meca_y,el_meca_z;


    //elancement mécanique selon Y
    el_meca_y=(getElancement_m_Lf(articule_tete,articule_pied).at(0)*Lg_barre_y*qPow(12,0.5))/H;
    //elancement mécanique selon Z
    el_meca_z=(getElancement_m_Lf(articule_tete,articule_pied).at(0)*Lg_barre_z*qPow(12,0.5))/b;


    //elancement relatif selon Y
    lambda.at(0)=(el_meca_y/pi)*qPow(fc0k/E005,0.5);

    //lambda.at(0)=((getElancement_m_Lf(articule_tete,articule_pied).at(0) * Lg_barre*qPow(12.0,0.5))/(H * 3.14))*qPow(fc0k/E005,0.5);

    //elancement relatif selon Z
    lambda.at(1)=(el_meca_z/pi)*qPow(fc0k/E005,0.5);

    //lambda.at(1)=((getElancement_m_Lf(articule_tete,articule_pied).at(0) * Lg_barre*qPow(12.0,0.5))/(b * 3.14))*qPow(fc0k/E005,0.5);



    std::vector<double> kc(5);
    // index 0 kcmax
    // index 1 kcY
    // index 2 kcZ
    // index 3 elancement selon Y
    // index 4 elancement selon Z


    //coeff betaC 0.2 bois massif,0.1 lamellé collé
    double betaC=0.2;if(!bois_Massif){betaC=0.1;}//page 59

 double ky=0;
    if(lambda.at(0)<0.3){kc.at(1)=1;}//kcY
    else
    {
        ky=0.5*(1+betaC*(lambda.at(0)-0.3)+qPow(lambda.at(0),2));
        kc.at(1)=1.0/(ky+qPow(qPow(ky,2)-qPow(lambda.at(0),2),0.5));
    }


double kz=0;
    if(lambda.at(1)<0.3){kc.at(2)=1.0;}//kcZ

    else
    {
        kz=0.5*(1.0+betaC*(lambda.at(1)-0.3)+qPow(lambda.at(1),2));
        kc.at(2)=1.0/(kz+qPow(qPow(kz,2)-qPow(lambda.at(1),2),0.5));
    }


    ///kcmax

    kc.at(0)=qMin(kc.at(1),kc.at(2));
    kc.at(3)=lambda.at(0);
    kc.at(4)=lambda.at(1);

    return kc;
}







