#ifndef MODELE_H
#define MODELE_H
#include "libeurocod2.h"
#include "librdm.h"
#include <QVector3D>
#include <QVector2D>
#include <QLineF>
#include <QDebug>
#include <QList>
#include <QMessageBox>









class Barre : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int nbreAppuis READ getNbreAppuis)
  //  Q_PROPERTY(int noeuddebut READ getNoeudDeb WRITE setNoeudDeb)
   // Q_PROPERTY(int noeudfin READ getNoeudFin WRITE setNoeudFin)



public:

    enum NatureChargements{NomCombi=0,G=1,Q=2,S=3,W1=4,W2=5,W3=6,W4=7,Sacc=8};Q_ENUM(NatureChargements)



   // static const int ELS_Inst_Q;
    static const int ELS_Inst;
    static const int ELS_Diff;
    static const int ELS_Fin;

    /// Création d'un objet barre contenant sections, lg, matériau, classe d'emploi
    Barre(int numNoeudDeb,int numNoeudFin, Libeurocod2 *lib=nullptr);
    Barre();

    Q_INVOKABLE void setListePoints(QList<QPointF *> points);
    Q_INVOKABLE QList<QPointF *> getListePoints();
    Q_INVOKABLE std::vector<double> getCoordonnees();
    Q_INVOKABLE void setListeAppuis(QList<QList<bool> > app);
    Q_INVOKABLE QList <int> getListeAppuis();
    Q_INVOKABLE int getNbreAppuis();
    Q_INVOKABLE int getNbreNoeuds();
    Q_INVOKABLE bool isAppuis(int numNoeuds);
    Q_INVOKABLE double getNoeud_X(int numNoeud);
    Q_INVOKABLE double getNoeud_Y(int numNoeud);
    Q_INVOKABLE double getLgConsole_G();
    Q_INVOKABLE double getLgConsole_D();
   // Q_INVOKABLE double getLgConsole_D();
    Q_INVOKABLE QList<double> getLg_Travees();
    Q_INVOKABLE std::vector <int> getNoeuds_Travee(int travee);
    Q_INVOKABLE int getNbreTravees();
    Q_INVOKABLE double getLgEntreNoeuds(int noeudA,int noeudB);


    Q_INVOKABLE void setGeometrie(bool articule_pied, int articule_tete, int atf_Y, int atf_Z, double H_2=0.0, double bande_charge=0.0);

    Q_INVOKABLE int setCaracteristiques(QString materiau, QString classeBois, QString classeService, QString dureeCharge, QString type_element="Elements structuraux",bool ksys=false);

    Q_INVOKABLE double getb();
    Q_INVOKABLE void setb(double b);
    Q_INVOKABLE double getLg();
    Q_INVOKABLE void setLg();
    Q_INVOKABLE double getH();
    Q_INVOKABLE void setH(double H);



    Q_INVOKABLE double getLg_fY();
    Q_INVOKABLE double getLg_fZ();
    Q_INVOKABLE double getH_2();
    Q_INVOKABLE double getH_moy();
    Q_INVOKABLE double getI(bool Iz=false);
    Q_INVOKABLE double getIV(bool IVz=false);
    Q_INVOKABLE double getBande_de_charge();
  //  Q_INVOKABLE double getMfY(QString cas_de_Charge);

    Q_INVOKABLE QString getMateriau();
    Q_INVOKABLE QString getClasseBois();
    Q_INVOKABLE QString getclasseService();
    Q_INVOKABLE QString getDuree_Charge();
   // Q_INVOKABLE QString getTypeCharge();
    //QString getCategorie();
    Q_INVOKABLE QString getTypeElement();
    Q_INVOKABLE double getksys();

    Q_INVOKABLE void addChargementUniforme(int cas_de_Charge, double Q=0, double bande_charge=0,int categorie=Libeurocod2::CategorieCharge::Null, int noeudDeb=0, int noeudFin=0);
    Q_INVOKABLE void addChargementPonctuel(NatureChargements cas_de_Charge, double P=0, int numNoeud=1,int categorie=Libeurocod2::CategorieCharge::Null);
    Q_INVOKABLE void clearChargements();

    ///Deux appuis Iso
    void calcul2App();
    Q_INVOKABLE double getResistanceFlexion(bool G_seul=false);
    Q_INVOKABLE double getContrainteFlexion(int combi, int travee);
    Q_INVOKABLE double getMomentFlexionDelta();
    Q_INVOKABLE QList<double> getTabMoments();

    ///barres hyperstatiques
    ///
    Q_INVOKABLE QList<double> getMomentsHyperS();
    Q_INVOKABLE QList<double> getEffTranchantHyperS();


    Q_INVOKABLE double getTauxtravailFlexion(double contrainte, double resistance);
    Q_INVOKABLE double getDeversement(int combi);
    Q_INVOKABLE double getFleche(ELS typefleche);
    Q_INVOKABLE double getWnet_fin(int travee);
    Q_INVOKABLE double getWinstQ_limite(int travee=1);
    Q_INVOKABLE double getWnet_fin_limite();
    Q_INVOKABLE double getTauxtravailWinstQ(double WinstQ, double WinstQ_lim);
    Q_INVOKABLE double getTauxtravailWnet_fin(double Wnet_fin, double Wnet_fin_lim);

    Q_INVOKABLE double getContrainteCompressionAxiale(double Effort_DaN);
    Q_INVOKABLE double getResistanceCompressionAxiale();
    Q_INVOKABLE double getTauxtravailCompressionAxiale(double contrainte, double resistance);
    Q_INVOKABLE double getFlambement_Y();
    Q_INVOKABLE double getFlambement_Z();
    Q_INVOKABLE double getElancement_Y();
    Q_INVOKABLE double getElancement_Z();
    Q_INVOKABLE double getFlambement_Max();

    Q_INVOKABLE double getResistanceCompressionTransversale();

    Q_INVOKABLE double getContrainteTractionAxiale(double Effort_en_DaN);
    Q_INVOKABLE double getResistanceTractionAxiale();
    Q_INVOKABLE double getTauxtravailTractionAxiale(double contrainte,double resistance);
    Q_INVOKABLE double getkH();

   // void setChargements(double G=0, double Q=0, double S=0, double W1=0, double W2=0, double W3=0, double W4=0, double A=0);
    Q_INVOKABLE void setChargements(bool ELS=true, int num_combi=NatureChargements::G);


    Q_INVOKABLE void setCombi_ELS();
    Q_INVOKABLE void setCombi_ELU();

    Q_INVOKABLE double getCombi_ELS(int num_Combi);
    Q_INVOKABLE int getNombre_combi_ELS();
    Q_INVOKABLE int getNombre_combi_ELU();
    Q_INVOKABLE QStringList getNoms_Combi_ELS();
    Q_INVOKABLE QStringList getNoms_Combi_ELU();
    Q_INVOKABLE QString getType_combi_ELS(int num_Combi);

    Q_INVOKABLE bool getChargement_utilise(int num_cas_charge=NatureChargements::G);
    Q_INVOKABLE int getNbreActionsVariable();




    Q_INVOKABLE Libeurocod2::CategorieCharge getCatQ() const;
    Q_INVOKABLE void setCatQ(const Libeurocod2::CategorieCharge &catQ);

    Q_INVOKABLE Libeurocod2::CategorieCharge getCatS() const;
    Q_INVOKABLE void setCatS(const Libeurocod2::CategorieCharge &catS);

    //Barre hyperstatiques

    Q_INVOKABLE QList<double> set_Matrice_B(QList<QList<double> > tab_force, int compteur_force);
    Q_INVOKABLE double set_lignei_Btab(QList<QList<double>> tab_force, int numero_appui, int compteur_force);
    Q_INVOKABLE QList<QList<double>> set_Matrice_A();
    Q_INVOKABLE QList<QList<double>> set_lignei_Atab(int numero_appui, int compteur_appui);
    Q_INVOKABLE QList<double> getReaction_effort(QList<QList<double> > tab_force, int compteur_force);

    ///récupère les efforts dûs aux moments calculées avec la théorie
    Q_INVOKABLE QList<double> getReaction_moment(QList<double> list_moment_theoreme);
    Q_INVOKABLE QList<double> getReaction_appuis(QList<double> reaction_effort,QList<double> reaction_moment);
    Q_INVOKABLE QList<double> getMoment(QList<double> reaction_appuis, QList<double> effort_positioneffort);
    Q_INVOKABLE QList<double> getTranchant(QList<double> reaction_appuis, QList<double> effort_positioneffort);

    ///Programme maître pour récupérer les efforts dans une barre à plus de deux appuis pour une force ponctuelle
    Q_INVOKABLE QList<QList<double>> get_Effort_Barre_Force_Ponctuelle(QList<QList<double> > tab_force, int compteur_force);

    ///Programme maître du programme maître pour récupérer l'effort de la barre
    Q_INVOKABLE QList<QList<double>> get_Effort_Barre();

    Q_INVOKABLE QList<double> get_Fleche_Barre();

    Q_INVOKABLE QList<QList<double>> getMcharges();

    Q_INVOKABLE QList<double> getEff_pond() const;

    Q_INVOKABLE QList<double> getCoordonees() const;


    Q_INVOKABLE int getPrecision() const;
    Q_INVOKABLE int getNbreSegments();

    Q_INVOKABLE int getNumNoeudDeb() const;
    void setNumNoeudDeb(int numNoeudDeb);

    Q_INVOKABLE int getNumNoeudFin() const;
    void setNumNoeudFin(int numNoeudFin);

private:
    void setFlambement();
    Libeurocod2 *ec5;
    double kmod,gammaM;
    bool art_pied,bois_Massif,Q_is_Entretien;
    Libeurocod2::CategorieCharge m_catQ;
    Libeurocod2::CategorieCharge m_catS;

    int art_tete;
    int m_precision;
    QStringList m_caracteristiques;
    Matrice_Chargement *m_matrice;

    QStringList m_categories_actions_variables,m_noms_cas_Charge;
    QList <int> m_action_variable;
    QList <QStringList> m_combi_ELS,m_combi_ELU;
    QList<QList <double>> m_charges;
    QPointF *m_ndeb,*m_nfin;
    int m_numNoeudDeb,m_numNoeudFin;
    QList<double> m_geometrie;
    QList<QPointF*> m_noeuds;
    QList<QList<bool>> m_appuis;
    QList<double> tabMomFlex,m_eff_pond,m_coordonees,m_fleches;
};



//////////////////////MODELE

class Modele:public QObject
{ Q_OBJECT
public:    
    Modele(Libeurocod2 *lib=nullptr);

    enum Appuis{Encastre=Qt::UserRole,Articule=Qt::UserRole+1,Glissant=Qt::UserRole+2};Q_ENUM(Appuis)

    const static int app_articule;
    const static int app_glissant;
    const static int app_encastre;

    Q_INVOKABLE void clearModele();
    Q_INVOKABLE void quickBarre(QList<double> lgtroncons,double bBarre,double hBarre,QString materiau, QString classeBois, QString classeService, QString dureeCharge);///création rapide d'une barre suivant tableau de longueurs
    Q_INVOKABLE void addNoeud(double x, double y=0);
    Q_INVOKABLE QList<int> getNoeuds();
    Q_INVOKABLE void addLiaison(int noeudDebut, int noeudFin, double b_Debut, double H_Debut, double b_Fin=0, double H_Fin=0);
    Q_INVOKABLE void addAppuis(int noeud, Appuis typeApp=Appuis::Encastre);
    Q_INVOKABLE void setRelaxation(int liaison,bool debut,bool fin);
    Q_INVOKABLE void setCombinaisons();
    Q_INVOKABLE void createBarres();
    Q_INVOKABLE int getNbreBarre();
    Q_INVOKABLE bool isAppuis(int noeud);

    Q_INVOKABLE Barre* getBarre(int numBarre);

private:
    Barre *t;
    Libeurocod2 *m_lib;
    double m_angle_beta=0; /// angle de la section ex: panne à dévers
    QList <Barre *> m_Liste_Barres;
    QList <QList<int> > m_liaisons;
    /// DEFINITION des Laisons
    /// 0 noeud début
    /// 1 noeud fin
    ///
    QList <QList<bool> > m_relax;/// relaxation des liaisons entre barres: true si relaxé

    QList <QList<double> > m_sections;
    /// 0 hauteur section noeud départ
    /// 1 largeur section noeud départ
    /// 2 hauteur section noeud départ
    /// 3 largeur section noeud départ
    QList<QPointF*> m_noeuds;/// coordonnées de noeuds en x et y
    QList<QList<bool>> m_appuis;

};

#endif // MODELE_H
