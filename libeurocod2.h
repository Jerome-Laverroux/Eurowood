#ifndef LIBEUROCOD2_H
#define LIBEUROCOD2_H


#include "QtMath"
#include "QStringList"
#include "QString"
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QDebug"
#include <QVector3D>
//#include "librdm.h"
#include <QObject>


enum ELS{Inst_Q=0,Inst=1,Creep=2,Fin=3,NetFin=4};



class Libeurocod2 : public QObject
{
    Q_OBJECT

public:
    Libeurocod2(bool withBDD=false);
    ~Libeurocod2();

    enum CategorieCharge{
        Null=-1,
        Cat_A=0,
        Cat_B=1,
        Cat_C=2,
        Cat_D=3,
        Cat_E=4,
        Cat_F=5,
        Cat_G=6,
        Cat_H=7,
        Ninf1000m=8,
        Nsup1000m=9,
        Vent=10};Q_ENUM(CategorieCharge)

    static CategorieCharge CategorieFromInt(int value){
        CategorieCharge out;
        switch (value) {
        case -1:
            out=CategorieCharge::Null;
            break;
        case 0:
            out=CategorieCharge::Cat_A;
            break;
        case 1:
            out=CategorieCharge::Cat_B;
            break;
        case 2:
            out=CategorieCharge::Cat_C;
            break;
        case 3:
            out=CategorieCharge::Cat_D;
            break;
        case 4:
            out=CategorieCharge::Cat_E;
            break;
        case 5:
            out=CategorieCharge::Cat_F;
            break;
        case 6:
            out=CategorieCharge::Cat_G;
            break;

        case 7:
            out=CategorieCharge::Cat_H;
            break;
        case 8:
            out=CategorieCharge::Ninf1000m;
            break;
        case 9:
            out=CategorieCharge::Nsup1000m;
            break;
        case 10:
            out=CategorieCharge::Vent;
            break;
        default:
            out=CategorieCharge::Null;
            break;
        }
        return out;
    };

    Q_INVOKABLE double getBDD_Value(QString table="caracBois", QString nom="", QString nom_tag="", QString tag="");
    Q_INVOKABLE QStringList getBDD_Noms(QString table, QString nom,QString filtre="");


    ///Climatique
    Q_INVOKABLE double getVent_vitesse_ref(QString region);
    Q_INVOKABLE QStringList getVent_Liste_rugosites();
    Q_INVOKABLE QString getVent_rugosite_definition(QString rugosite);
    Q_INVOKABLE double getVent_rugosite_Z0(QString rugosite);
    Q_INVOKABLE double getVent_rugosite_Zmin(QString rugosite);
    Q_INVOKABLE double getVent_pression_dyn_pointe(double cez, double z0, double zmin);

    ///RECHERCHE DES VALEURS CARACTERISTIQUES DANS LES TABLEAUX (coeff, caracteristique bois ...);

    Q_INVOKABLE QStringList getlisteClasseService();
    Q_INVOKABLE QStringList getListeClasseDuree();
    Q_INVOKABLE double getkmod(QString classeService, QString classeDuree);

    Q_INVOKABLE QStringList getListeMateriaux_gammaM();
    Q_INVOKABLE double getgammaM(QString materiau);

    Q_INVOKABLE double getkH(bool boisMassif=true,double H_en_mm=0.0);
    Q_INVOKABLE double getksys(int enabled=0);
    Q_INVOKABLE double getkcrit(double elancement_relatif, bool blocage_deversement);
    Q_INVOKABLE QStringList getPsi_Type();
    Q_INVOKABLE QStringList getPsi_Categorie();
    Q_INVOKABLE double getpsi0(CategorieCharge categorie);
    Q_INVOKABLE double getpsi1(CategorieCharge categorie);
    Q_INVOKABLE double getpsi2(CategorieCharge categorie);
    Q_INVOKABLE double getgamma_G(bool G_sup=true,bool G_inf_EQU=false);
    Q_INVOKABLE double getgamma_Q();

    Q_INVOKABLE QStringList getListeMateriaux_kdef();
    Q_INVOKABLE double getkdef(QString materiau,QString classeService);

    Q_INVOKABLE QStringList getListeClasseBois(int materiau);
    Q_INVOKABLE double getfmk(QString classeBois);
    Q_INVOKABLE double getft0k(QString classeBois);
    Q_INVOKABLE double getft90k(QString classeBois);
    Q_INVOKABLE double getfc0k(QString classeBois);
    Q_INVOKABLE double getfc90k(QString classeBois);
    Q_INVOKABLE double getfvk(QString classeBois);
    Q_INVOKABLE double getE0mean(QString classeBois);
    Q_INVOKABLE double getE05pourc(QString classeBois);
    Q_INVOKABLE double getE90mean(QString classeBois);
    Q_INVOKABLE double getGmean(QString classeBois);
    Q_INVOKABLE double getMasseVolCarac(QString classeBois);//masse volumique
    Q_INVOKABLE double getMasseVolMoy(QString classeBois);//masse volumique moyenne


    //Calcul des moments quadratiques
    ///Moment quadratique d'une poutre rectangulaire I
    Q_INVOKABLE double getMomentQuadratiqueRect(double b, double h);
    ///Moment quadratique d'une poutre circculaire I
    Q_INVOKABLE double getMomentQuadratiqueCirc(double diam);




    /*-----------------------------------------------------------------VERIFICATION ASSEMBLAGES--------------------------------------------*/
    /*ASEMBLAGES BOIS-BOIS*/

    //Ensemble des organes utilisés
    enum Organe{
        Boulon=Qt::UserRole+1,
        Tirefond=Qt::UserRole+2,
        Pointe=Qt::UserRole+3,
        Broche=Qt::UserRole+4,
        Agraphe=Qt::UserRole+5,
    };Q_ENUM(Organe)

    //Résistance à l'arrachement FaxRk
    ///Résistance à l'arrachement des pointes
    Q_INVOKABLE double getFaxRk_Pointes(double pk, double d_pointe,double d_tete_pointe, double profondeur_clouage_portee, double profondeur_clouage_porteur, bool annelee=true);
    ///Résistance à l'arrachement des boulons
    Q_INVOKABLE double getFaxRk_Boulons(double d_ext_rondelle, double d_int_rondelle, double FtRd, double fc90d);
    ///Résistance à l'arrachement des tire-fonds
    Q_INVOKABLE double getFaxRk_Tirefonds(double nbr_tirefond,double diam, double longueur_penetration_filetage, double angle_axe_fil_bois,double masse_vol_carac);

    ///Résistance à l'arrachement des organes d'assemblage
    Q_INVOKABLE double getFaxRk_Organes(Organe typeElement,double diam, double d_ext_rondelle, double d_int_rondelle, double FtRd, double fc90d,double pk, double d_pointe,double d_tete_pointe, double profondeur_clouage_portee, double profondeur_clouage_porteur, bool annelee=true);


    ///Nombre effectif d'oragnes dans un assemblage
    Q_INVOKABLE double getNbrEff_Organe_Cisaillment(Organe typeElement, double nbr_organes_par_files, double diam, double pince_a1, bool effort_perpendiculaire_au_fil=false, bool prepercage =false);



    //Moment d'écoulement plastique de l'organe MyRk
    ///Moment d'écoulement plastique des pointes
    Q_INVOKABLE double getMyrk_Pointes(double fu, double diam, bool circulaire=true);
    ///Moment d'écoulement plastique des agraphes
    Q_INVOKABLE double getMyrk_Agraphes(double fu, double diam);
    ///Moment d'écoulement plastique des boulons
    Q_INVOKABLE double getMyrk_Boulons_Broches(double fu, double diam);
    ///Moment d'écoulement plastique des tire-fonds
    Q_INVOKABLE double getMyrk_Tire_Fonds(double fu, double diam, bool circulaire=true);

    ///Moment d'écoulement plastique des organes d'assemblage
    Q_INVOKABLE double getMyrk_Organe(double fu, double diam,Organe typeElement, bool circulaire=true);



    //Portance Locale fhk
    /// Portance locale Bois massif LC et LVL


    Q_INVOKABLE double getfhkBois(double pk,double diam,double angle_filbois, QString materiau,Organe typeElement=Organe::Tirefond,bool prePercage=false);
    /// Portance locale Contre plaqué
    Q_INVOKABLE double getfhkContrePlaque(double pk, double diam,Organe typeElement=Organe::Tirefond);
    /// Portance locale Fibres durs
    Q_INVOKABLE double getfhkFibreDurs(double t,double diam);
    /// Portance locale OSB et autre panneaux de particules
    Q_INVOKABLE double getfhkOSBParticules(double t,double diam,Organe typeElement=Organe::Tirefond);


    //Rupture Simple cisaillement
    ///ecrasement du bois dans la pièce 1 (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_A(double fh1k,double t1,double diam);
    ///ecrasement du bois dans la pièce 2 (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_B(double fh2k,double t2,double diam);
    ///ecrasement du bois dans la pièce 1 et la pièce 2 (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_C(double fh1k,double t1,double t2,double diam,double beta,Organe typeElement, double FaxRk=0.0);
    ///ecrasement du bois dans la pièce 1 et rotule plastique dans la tige (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_D(double fh1k,double t1,double diam,double beta,double MyRk,Organe typeElement,double FaxRk=0.0);
    ///ecrasement du bois dans la pièce 2 et rotule plastique dans la tige (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_E(double fh1k,double t2,double diam,double beta,double MyRk,Organe typeElement,double FaxRk=0.0);
    ///ecrasement du bois dans la pièce 1 et la pièce 2 et rotule plastique dans la tige (simple cisaillement)
    Q_INVOKABLE double getFvrk_B_F(double fh1k,double diam,double beta,double MyRk,Organe typeElement,double FaxRk=0.0);

    ///assemblages bois-bois oragnes en simple cisaillement
    Q_INVOKABLE double getFvrk_B_Simple_Cisaillement(double fh1k,double fh2k,double t1, double t2, double diam,double MyRk,Organe typeElement,double FaxRk=0.0,bool organe_en_bois_de_bout=false);



    //Rupture Double cisaillement
    ///ecrasement du bois dans les deux pièces 1 (double cisaillement)
    Q_INVOKABLE double getFvrk_B_G(double fh1k,double t1,double diam);
    ///ecrasement du bois dans la pièce centrale 2 (double cisaillement)
    Q_INVOKABLE double getFvrk_B_H(double fh2k,double t2,double diam);
    ///ecrasement du bois dans les deux pièces 1 et rotule plastique dans la tige (double cisaillement)
    Q_INVOKABLE double getFvrk_B_J(double fh1k,double t1,double diam,double beta,double MyRk,Organe typeElement,double FaxRk=0.0);
    ///ecrasement du bois dans les deux pièces 1 et la pièce 2 et rotule plastique dans la tige (double cisaillement)
    Q_INVOKABLE double getFvrk_B_K(double fh1k, double diam, double beta, double MyRk,Organe typeElement, double FaxRk=0.0);


    ///assemblages bois-bois oragnes en double cisaillement
    Q_INVOKABLE double getFvrk_B_Double_Cisaillement(double fh1k,double fh2k,double t1, double t2, double diam,double MyRk,Organe typeElement,double FaxRk=0.0,bool organe_en_bois_de_bout=false);


    ///Pourcentage limite de contribution à la capacité resistante de Johansen
    Q_INVOKABLE double getPourcentJohansen(Organe typeElement);

    /*ASEMBLAGES BOIS-METAL*/

    //Rupture Simple cisaillement plaque mince
    ///ecrasement du bois dans la pièce (simple cisaillement plaque mince)
    Q_INVOKABLE double getFvrk_M_A(double fhk,double t1,double diam);
    ///rotule plastique de la tige (simple cisaillement plaque mince)
    Q_INVOKABLE double getFvrk_M_B(double fhk,double diam, double MyRk,Organe typeElement, double FaxRk=0.0);

    ///Simple cisaillement assemblage bois-métal
    Q_INVOKABLE double getFvrk_M_SimpleCisaillement(double fhk,double t1, double diam, double MyRk, double epaisseur_plaque,Organe typeElement, double FaxRk=0.0);

    //Rupture Simple cisaillement plaque épaisse
    ///ecrasement du bois dans la pièce (simple cisaillement plaque épaisse)
    Q_INVOKABLE double getFvrk_M_C(double fhk,double t1,double diam);
    ///ecrasement du bois dans la pièce et rotule plastique dans la tige (simple cisaillement plaque épaisse)
    Q_INVOKABLE double getFvrk_M_D(double fhk,double t1,double diam,double MyRk, Organe typeElement, double FaxRk=0.0);
    ///rotule plastique dans la tige (simple cisaillement plaque épaisse)
    Q_INVOKABLE double getFvrk_M_E(double fhk,double diam,double MyRk, Organe typeElement, double FaxRk=0.0);

    //Rupture Double cisaillement avec plaque métallique centrale
    ///ecrasement du bois dans la pièce 1 (double cisaillement plaque métallique centrale)
    Q_INVOKABLE double getFvrk_M_F(double fh1k,double t1, double diam);
    ///ecrasement du bois et rotule plastique dans la tige (double cisaillement plaque métallique centrale)
    Q_INVOKABLE double getFvrk_M_G(double fh1k,double t1,double diam, double MyRk,Organe typeElement, double FaxRk=0.0);
    ///rotule plastique dans la tige (double cisaillement plaque métallique centrale)
    Q_INVOKABLE double getFvrk_M_H(double fh1k, double diam, double MyRk,Organe typeElement, double FaxRk=0.0);

    ///Double cisaillement plaque centrale assemblage bois-métal
    Q_INVOKABLE double getFvrk_M_DoubleCisaillement_PlaqueCentrale(double fh1k,double t1, double diam, double MyRk, Organe typeElement, double FaxRk=0.0);



    //Rupture Double cisaillement avec deux plaques métalliques minces externes
    ///écrasement du bois dans la pièce centrale (double cisaillement plaques métalliques minces externes)
    Q_INVOKABLE double getFvrk_M_J(double fh2k,double t2,double diam);
    ///rotule plastique dans la tige (double cisaillement plaques métalliques minces externes)
    Q_INVOKABLE double getFvrk_M_K(double fh2k, double diam, double MyRk,Organe typeElement, double FaxRk=0.0);

    //Rupture Double cisaillement avec deux plaques métalliques épaisses externes
    ///écrasement du bois dans la pièce centrale (double cisaillement plaques métalliques épaisses externes)
    Q_INVOKABLE double getFvrk_M_L(double fh2k,double t2,double diam);
    ///rotule plastique dans la tige (double cisaillement plaques métalliques épaisses externes)
    Q_INVOKABLE double getFvrk_M_M(double fh2k, double diam, double MyRk, Organe typeElement, double FaxRk=0.0);

    ///Double cisaillement plaques externes assemblage bois-métal
    Q_INVOKABLE double getFvrk_M_DoubleCisaillement_PlaquesExternes(double fh2k,double t2, double diam, double MyRk, double epaisseur_plaque,Organe typeElement, double FaxRk=0.0);


    //---------------------------------------------------------FENDAGE--------------------------------------------------
    ///Résistance d'un assemblage au fendage
    Q_INVOKABLE double getResistanceFendage(double largeur, double hauteur, double distance_rive_charge_organe_leplus_eloigne, bool assemblage_en_bois_de_bout=false, bool plaque_emboutie=false, double largeur_plaque=0.0);



    //Conditions de pince dans le bois
    //Pointes
    ///a1 pointes
    Q_INVOKABLE double getPince_Pointes_a1(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a2 pointes
    Q_INVOKABLE double getPince_Pointes_a2(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a3c pointes
    Q_INVOKABLE double getPince_Pointes_a3c(double diam,double pk_bois, bool prepercage=false);
    ///a3t pointes
    Q_INVOKABLE double getPince_Pointes_a3t(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a4c pointes
    Q_INVOKABLE double getPince_Pointes_a4c(double diam,double pk_bois, bool prepercage=false);
    ///a4t pointes
    Q_INVOKABLE double getPince_Pointes_a4t(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);

    //Agraphes
    ///a1 agraphes
    Q_INVOKABLE double getPince_Agraphes_a1(double diam, double angle_effort_filbois, double angle_agraphes_filbois);
    ///a2 agraphes
    Q_INVOKABLE double getPince_Agraphes_a2(double diam);
    ///a3c agraphes
    Q_INVOKABLE double getPince_Agraphes_a3c(double diam);
    ///a3t agraphes
    Q_INVOKABLE double getPince_Agraphes_a3t(double diam, double angle_effort_filbois);
    ///a4c agraphes
    Q_INVOKABLE double getPince_Agraphes_a4c(double diam);
    ///a4t agraphes
    Q_INVOKABLE double getPince_Agraphes_a4t(double diam, double angle_effort_filbois);


    //Boulons
    ///a1 boulons
    Q_INVOKABLE double getPince_Boulons_a1(double diam, double angle_effort_filbois);
    ///a2 boulons
    Q_INVOKABLE double getPince_Boulons_a2(double diam);
    ///a3c boulons
    Q_INVOKABLE double getPince_Boulons_a3c(double diam, double angle_effort_filbois);
    ///a3t boulons
    Q_INVOKABLE double getPince_Boulons_a3t(double diam);
    ///a4c boulons
    Q_INVOKABLE double getPince_Boulons_a4c(double diam);
    ///a4t boulons
    Q_INVOKABLE double getPince_Boulons_a4t(double diam, double angle_effort_filbois);

    //Broches
    ///a1 broches
    Q_INVOKABLE double getPince_Broches_a1(double diam, double angle_effort_filbois);
    ///a2 broches
    Q_INVOKABLE double getPince_Broches_a2(double diam);
    ///a3c broches
    Q_INVOKABLE double getPince_Broches_a3c(double diam, double angle_effort_filbois);
    ///a3t broches
    Q_INVOKABLE double getPince_Broches_a3t(double diam);
    ///a4c broches
    Q_INVOKABLE double getPince_Broches_a4c(double diam);
    ///a4t broches
    Q_INVOKABLE double getPince_Broches_a4t(double diam, double angle_effort_filbois);


    //Tire_fonds
    ///a1 tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a1(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a2 tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a2(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a3c tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a3c(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a3t tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a3t(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);
    ///a4c tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a4c(double diam,double pk_bois, bool prepercage=false);
    ///a4t tire-fonds
    Q_INVOKABLE double getPince_Tirefonds_a4t(double diam, double pk_bois,double angle_effort_filbois, bool prepercage=false);


    //Conditions de pince dans le métal
    //Pinces maximales
    ///Pince longitudinal max e1
    Q_INVOKABLE double getPinceMetalMax_e1(double epaisseur_plaque, bool en_exterieur=false);
    ///Pince transversale max e2
    Q_INVOKABLE double getPinceMetalMax_e2(double epaisseur_plaque, bool en_exterieur=false);
    ///Entraxe en file longitudinale max p1
    Q_INVOKABLE double getPinceMetalMax_p1(double epaisseur_plaque, bool en_exterieur=false);
    ///Entraxe en file transversale p2
    Q_INVOKABLE double getPinceMetalMax_p2(double epaisseur_plaque, bool en_exterieur=false);
    //Pinces minimales
    ///Pince longitudinal min e1
    Q_INVOKABLE double getPinceMetalMin_e1(double diam_percage);
    ///Pince transversale min e2
    Q_INVOKABLE double getPinceMetalMin_e2(double diam_percage);
    ///Entraxe en file longitudinale min p1
    Q_INVOKABLE double getPinceMetalMin_p1(double diam_percage);
    ///Entraxe en file transversale min p2
    Q_INVOKABLE double getPinceMetalMin_p2(double diam_percage);



    //-----------------------------------------------------------VERIFICATION DES PIECES METALLIQUES---------------------------------------
    /// Résistance en traction des boulons
    Q_INVOKABLE double getFtrd_Boulon(double fu, double diam, double  gammaM2);

    //Vérification des plats métalliques
    ///Résistance en traction d'un plat métallique
    Q_INVOKABLE double getResistancePlatTraction(double epaisseur, double hauteur, double Anet, double fu, double fy, double gammaM0, double gammaM2);

    ///Vérification de l'impact des perçages sur la résistance en flexxion d'un plat métallique
    Q_INVOKABLE double getVerifPercagePlatFlexion(double epaisseur_plat, double hauteur_plat, double nbr_percage, double diam_percage, double fu, double fy, double gammaM0, double gammaM2);
    ///Résistance en flexion d'un plat métallique
    Q_INVOKABLE double getResistancePlatFlexion(double wely, double fy, double gammaM0, double importance_percage);


    //Vérification des soudures
    ///Résistance d'un cordon frontal
    Q_INVOKABLE double getResistanceCordonFrontal(double gorge, double longueur, double fu, double betaw, double gammaM2);
    /// Résistance d'un cordon latéral
    Q_INVOKABLE double getResistanceCordonLateral(double gorge, double longueur, double fu, double betaw, double gammaM2);
    /// Résistance d'un cordon oblique
    Q_INVOKABLE double getResistanceCordonOblique(double gorge, double longueur, double fu, double betaw, double gammaM2, double angle);







    //-----------------------------------------------------------------VERIFICATION DES POUTRES--------------------------------------------
    //Vérification d'un plancher
    ///Calcul de la fréquence propre d'un plancher
    Q_INVOKABLE double getPlancherFrequence(double longueur_solive_mm, double m, double EIl);
    ///Calcul des rigidités equivalentes plancher
    Q_INVOKABLE double getPlancherRigidite(double E, double bande_chargement, double I);
    ///Calcul du n40
    Q_INVOKABLE double getPlanchern40(double frequence, double longueur_solive, double largeur_plancher, double EIl, double EIb);
    ///Calcul de la vitesse vibratoire du plancher
    Q_INVOKABLE double getPlancherVitesse(double n40, double m, double largeur_plancher, double longueur_solive);
    ///Calcul de la vitesse vibratoire limite du plancher
    Q_INVOKABLE double getPlancherVitessemax(double b, double epsilon, double frequence);
    ///Calcul de la souplesse d'une solive
    Q_INVOKABLE double getPlancherSouplesse_solive(double longueur_solive, double E, double I);
    ///Calcul de la souplesse d'un panneau de plancher
    Q_INVOKABLE double getPlancherSouplesse_panneau(double bande_chargement, double epaisseur_panneau, double largeur_panneau, double E);
    ///Calcul du déplacement d'un plancher
    Q_INVOKABLE double getPlancherDeplacement(double ks, double kp);

    //Calcul ELU
    ///Calcul de la résistance en flexion fmd
    Q_INVOKABLE double getResistanceFlexion(double fmK, double kmod, double gammaM, double ksys, double kh);
    ///Calcul de la contrainte en flexion
    Q_INVOKABLE double getContrainteFlexion(double P, double coord_P, double Lg, double IVy);

    ///coefficent pour calculer kv : kn
    Q_INVOKABLE double getEntaille_kn(QString type_bois);
    ///coefficient pour calculer kv : i
    Q_INVOKABLE double getEntaillePente_i(double h_entaille, double b_entaille);
    ///coefficient pour calculer kv : α
    Q_INVOKABLE double getEntaille_alpha(double h_barre, double h_entaille);
    ///coefficient d'entaille kv
    Q_INVOKABLE double getEntaille_kv(double hauteur_poutre, double pente_entaille_i,double rapport_hauteur_alpha, double kn, double distance_appui_x, bool entaille_opposee_appui=false);
    ///Resitance cisaillement fvd
    Q_INVOKABLE double getResistanceCisaillement(double fvk,double kmod,double gammaM, bool assemblage_en_bois_de_bout=false);
    ///Contrainte cisaillement
    Q_INVOKABLE double getContrainteCisaillement(double effort_tranchant_en_DaN, double kcr, double b, double hef);
    ///Taux de travail cisaillement
    //Q_INVOKABLE double getTauxtravailCisaillement(double contrainte,double resistance,double kv);

    ///coefficient de résistance à la compression kc90
    Q_INVOKABLE double getCompression_kc90(QString type_bois, double h_barre, double l1, bool appui_continu=false);
    ///Resitance compression axiale fc0d
    Q_INVOKABLE double getResistanceCompressionAxiale(double fc0k,double kmod,double gammaM);
    ///Contrainte compression axiale
    Q_INVOKABLE double getContrainteCompressionAxiale(double effort_en_DaN, double aire_mm2);

    ///Calcul contrainte traction axiale
    Q_INVOKABLE double getContrainteTractionAxiale(double effort_en_DaN, double aire_mm2);
    ///Calcul résistance traction axiale ft0d
    Q_INVOKABLE double getResistanceTractionAxiale(double ft0k,double kmod,double gammaM,double kH);

    ///longueur efficace compression transversale
    Q_INVOKABLE double getLongueurEfficaceCompression(double l_appui,double a, double l1);
    ///Calcul de résistance en compression transversale fc90d
    Q_INVOKABLE double getResistanceCompressionTransversale(double fc90k,double kmod,double gammaM);
    ///Calcul de contrainte en compression trnasversale
    Q_INVOKABLE double getContrainteCompressionTransversale(double fc90d,double b,double l_eff);


    ///Taux de travail de toute vérification
    Q_INVOKABLE double getTauxtravail(double contrainte,double resistance);
    Q_INVOKABLE double getTauxtravailCompression(double contrainte,double resistance,double kc90);

    //Calcul ELS
    Q_INVOKABLE double getW_nodale(double F,double coord,double Lg_barre, double E0mean, double I);
    Q_INVOKABLE double getWinstQ(double Lg, double E0mean, double I, double bande_charge, double Q_inst);
    Q_INVOKABLE double getWinst(double Lg, double E0mean, double I, double bande_charge, double G, double Q=0, double S=0, double psi0=0);
    Q_INVOKABLE double getWcreep(double Lg, double E0mean, double I, double bande_charge,double G, double psi2, double kdef, double Q=0);

    Q_INVOKABLE double getWinstQ_lim(double Lg, QString element, bool Agricole=false);
    Q_INVOKABLE double getWnetfin_lim(double Lg, QString element,bool Agricole=false);

    Q_INVOKABLE double getContrainteCritique(double E005, double b, double H, double Lg, bool charge_repartie, bool porte_a_faux, bool dessus_fibre_neutre);

    Q_INVOKABLE double getElancement_relatif_flexion(double sigma_m_crit,double fmk);

    //tableau des coefficients de flambements et des élancements y et z
    Q_INVOKABLE std::vector<double> getElancement_relatif(double Lg_barre_y,double Lg_barre_z,double b,double H,double fc0k, double E005, int articule_tete, bool articule_pied,bool bois_Massif);





private:
    QSqlDatabase bdd;

    //accès à la BDD
    QString getInfo(QString classeBois,QString carac);
    QStringList m_categoriesCharges;

    //tableau des coeffs m et Lf(élancment, flambement)
    std::vector<double> getElancement_m_Lf(int articule_tete,bool articule_pied);
};

#endif // LIBEUROCOD2_H
