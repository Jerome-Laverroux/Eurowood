#ifndef LIBRDM_H
#define LIBRDM_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineF>
#include <QPointF>
#include <qmath.h>
#include <math.h>
#include <QDebug>
#include <qvector3d.h>

class Trois_appuis
{
public:
    Trois_appuis();


    double getRotationTroisMoment_Ponctuelle(double P, double a, double b,double l);




private:
    QList<double> m_ltab;
    QList<QList<double>> m_Ftab;
    QList<double> m_Btab;

    QList<QList<double>> m_Atab;
    QList<double> m_Mtab;

    QList<double> m_Rtab;




};


class Charge
{

    public:

    Charge();
    void ConcentreConsoleG(double LgConsole, double LgA, double F);
    void Concentre(double lgBarre,double LgA,double F);    
    void ConcentreConsoleD(double lg,double F);
    double getRA();
    double getRB();
    double getMoment_Charge();
    double getMomentResultant_travee(double abscisse);
    double getMomentResultant_ConsoleG(double position_Console_G, double abscisse, double Lg_travee);
    double getAbcisse();
    double getFl();



    private:

    void setRA(double d);
    void setRB(double d);    
    void setMoment(double mom,double ab);
    void setFleche(double fleche, bool en_console=false);

    double RA,RB,abcisse,moment,effort,longueurA,longueurB,barre,Lg_console_G,Lg_console_D,fl;
};

/// matrice servant à sommer les efforts au noeud
/// permet de calculer les moments résultants et autre données de RDM
/// en utilisant les fonctions précédentes

class Matrice_Chargement
{
public:
    Matrice_Chargement();
    Matrice_Chargement(QList<double> efforts,QList<double> coord, double longueur_Barre, QList<QList <bool> > appuis, double Lg_console_G=0, double Lg_console_D=0);
    double getMfy(int noeudDeb=1, int noeudFin=2, int precision=10);

    double getFleche();
    QList<double> getTabMfy();
    void calculer();


    QList<double> getF() const;
    void setF(const QList<double> &F);

    QList<double> getPosition() const;
    void setPosition(const QList<double> &position);

    double getLg_console_G() const;
    void setLg_console_G(double lg_console_G);

    double getLg_console_D() const;
    void setLg_console_D(double lg_console_D);

    double getLg_Barre() const;
    void setLg_Barre(double lg_Barre);

    double getMfyTravee() const;

    int getNoeudDeb() const;
    void setNoeudDeb(int noeudDeb);

    int getNoeudFin() const;
    void setNoeudFin(int noeudFin);

    int getPrecision() const;
    void setPrecision(int precision);

private:
    int *nbreApp;
    int m_noeudDeb,m_noeudFin;
    QList<double> m_couplesAuNoeud;
    QList<double> m_F,m_position,m_moment_Total;
    double m_lg_console_G,m_lg_console_D,m_lg_Barre;
    double m_MfyTravee,m_MfyConsole_G,m_MfyConsoleD;
    int m_tailleTableaux,m_precision;
};


/// SECTION TORSEURS

class Torseur
{
public:

    static const std::vector<int> liaisons_glissant;
    static const std::vector<int> liaisons_articule;
    static const std::vector<int> liaisons_encastre;

    Torseur(double x=0, double y=0, double z=0, double Fx=0, double Fy=0, double Fz=0, double Mx=0, double My=0, double Mz=0, double EIz=0);

    QVector3D getCoordonnees();
    QVector3D getEfforts();
    QVector3D getMoments();
    double Fx();
    double Fy();
    double Fz();
    double Mx();
    double My();
    double Mz();

    void translateConsole(QVector3D point);
    void translateEntreAppuis(QVector3D point,QVector3D debBarre,QVector3D finBarre);

private:

    QVector3D coord;
    QVector3D F;
    QVector3D M;
    double EI;

};







class Maths
{
public:
    /// arrondi d'un nombre
   static double arr(double val, int decimales)
   {
       double dec=0;
       dec=qPow(10,decimales+2);
       double sortie=qCeil(val*dec)/dec;
       return sortie;
   }

   ///Pour initialiser une matrice à deux dimensions
   QList<double> init_matrice_ligne(int nbr_colonnes);

   ///Pour initialiser une matrice à deux dimensions
   QList<QList<double>> init_matrice(int nbr_lignes, int nbr_colonnes);


   ///On transorme une matrice carrée quelconque en matrice triangulaire supérieure
   QList<QList<double>> Matrice_Triangularisation(QList<QList<double>> Matrice_carre);

   QList<QList<double>> Matrice_Addition(QList<QList<double>> Matrice_carre1,QList<QList<double>> Matrice_carre2);

   ///Formation de Ab pour la résolution de l'équation A*X=b
   QList<QList<double>> Matrice_Assemblage(QList<QList<double>>A,QList<double> b);

   ///Reformation de A et b à partir de Ab triangularisé
   void Matrice_Desassemblage(QList<QList<double>>A);

   ///Résolution du système matriciel où A est une matrice triangulaire supérieure : A*X=b
   QList<double> Matrice_Inversion(QList<QList<double>> Matrice_triangle_A,QList<double> Matrice_b);

   ///Fonction maitresse pour résoudre l'équation matricielle A*X=b
   QList<double> Matrice_Resolution_Systeme(QList<QList<double>> Matrice_carre_A,QList<double> Matrice_b);

   QList<QList<double> > getA() const;
   void setA(const QList<QList<double> > &A);

   QList<double> getB() const;
   void setB(const QList<double> &B);

private:
   QList<QList<double>> m_A;
   QList<double> m_B;
};




#endif // LIBRDM_H
