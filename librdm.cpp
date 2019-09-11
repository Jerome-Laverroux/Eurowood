#include "librdm.h"




/// Librairie servant au calcul des réactions d'appuis, des moments, et tout ce qui concerne la RDM



Charge::Charge()
{
    RA=0;RB=0;abcisse=0;moment=0;effort=0;longueurA=0;longueurB=0;barre=0;fl=0;
    Lg_console_G=0;
    Lg_console_D=0;
}

void Charge::ConcentreConsoleG(double LgConsole,double LgA,double F)
{
    setRA(F);
    setRB(0);
    Lg_console_G=LgConsole;
    setMoment(-(F*(Lg_console_G-LgA)),0);
    longueurA=LgA;//compris entre 0 et longueur console

}

void Charge::Concentre(double lgBarre,double LgA,double F)
{
    effort=F;longueurA=LgA;longueurB=lgBarre-LgA;barre=lgBarre;

    if(LgA==0.0){setRA(F);setRB(0);setMoment(0,0);fl=0;}
    else
    {
        if(lgBarre==LgA){setRA(0);setRB(F);setMoment(0,lgBarre);fl=0;}
        else
        {
            setRA((F*longueurB)/lgBarre);
            setRB((F*LgA)/lgBarre);            
            setMoment(((F*LgA*(lgBarre-LgA)))/lgBarre,LgA);

            if(LgA<(lgBarre/2.0)){setFleche((-F*LgA)*((qPow(lgBarre,2)/8.0)-(qPow(LgA,2)/6.0)),false);}
            else{setFleche((-F*longueurB)*((qPow(lgBarre,2)/8.0)-(qPow(longueurB,2)/6.0)),false);}

        }
    }
}

void Charge::ConcentreConsoleD(double Lg,double F)
{
    setRB(F);
    setRA(0);
    setMoment(-(F*Lg),0);
}


void Charge::setRA(double d){if(d<=0.01&&d>=0){d=0;}RA=d;}
void Charge::setRB(double d){if(d<=0.01&&d>=0){d=0;}RB=d;}

void Charge::setMoment(double mom,double ab)
{
    if(moment<=0.01&&moment>=0){moment=0;}
    moment=mom;
    abcisse=ab;
}

void Charge::setFleche(double fleche, bool en_console)
{
    if(en_console){}
    fl=fleche;
}

double Charge::getRA(){return -RA;}
double Charge::getRB(){return -RB;}
double Charge::getMoment_Charge(){return moment;}


double Charge::getAbcisse(){return abcisse;}

double Charge::getFl()
{

    return fl;
}

double Charge::getMomentResultant_ConsoleG(double position_Console_G,double abscisse,double Lg_travee)
{
    double retour=0;
    double abs=abscisse;
    double AE,AC;

    if(abs>=longueurA)// si l'abscisse calculé est à gauche de l'effort il n'y a pas de moment
    {
        if(abs==0.0){retour=0;}
        else
        {
            if(abs<Lg_console_G)
            {
                AE=abs -longueurA;
                AC=Lg_console_G-longueurA;

             retour=getMoment_Charge() * (AE/AC);// sur la console

            }
            else
            {
                    retour=getMoment_Charge() * ((Lg_travee-(abs-Lg_console_G))/Lg_travee);//en travée
            }
        }
    }
    else {retour=0;}

    return retour;
}


double Charge::getMomentResultant_travee(double abs)
{

    bool F_sur_console=false;//si l'effort est sur une console
    bool abs_apres_appuis=false;// si l'abcisse est après un noeud intermédiaire


    double retour=0,AD=0,AB=0;

    //sur 2 appuis

    if(abs<longueurA)//A gauche de la force
    {
        if(abs!=0)
        {
            if(abs==longueurA){retour=0;}
            else
            {
                AD=abs;
                retour=getMoment_Charge()*(AD/longueurA);
            }
        }
        else{retour=0;}
    }
    else// A Droite de la force
    {

        if(abs!=barre)
        {
            AD=barre-abs;            
            retour=getMoment_Charge()*(AD/longueurB);
        }
        else{retour=0;}

    }
    return retour;
}





//std::vector<double> F,position,moment_Total;
//double Lg;

Matrice_Chargement::Matrice_Chargement(){setLg_console_G(0);setLg_console_D(0);}

Matrice_Chargement::Matrice_Chargement(QList<double> efforts, QList<double> coord, double longueur_Barre, QList<QList<bool> > appuis, double Lg_console_G, double Lg_console_D)
{

    setF(efforts);
    setPosition(coord);
    setLg_Barre(longueur_Barre);
    setLg_console_G(Lg_console_G);
    setLg_console_D(Lg_console_D);
    m_couplesAuNoeud.clear();
}

void Matrice_Chargement::calculer()
{
    Charge charge;

    int pos_cons_G=0;
    int pos_cons_D=0;
    m_moment_Total.clear();
    for (int i=0;i<m_tailleTableaux ;i++) {m_moment_Total.append(0);}

    for(int i=0;i<m_position.count();i++)
    {   QLineF test(QPointF(m_position.at(0),0),QPointF(m_position.at(i),0));

        if(test.length()==m_lg_console_G) /// recherche index console gauche
        {
            pos_cons_G=i;
            break;
        }
    }
    if(pos_cons_D==0){pos_cons_D=m_position.count();}


    for(int i=0;i<m_position.count();i++)
    {   QLineF test(QPointF(m_position.at(0),0),QPointF(m_position.at(i),0));// sert à déterminer la longueur ou l'on se trouve dans la barre

        ///Calcul des moments sur la console gauche
        if(test.length()<m_lg_console_G)
        {
            if(qAbs(m_F.at(i))>0)
            {
                charge.ConcentreConsoleG(m_lg_console_G,test.length(),m_F.at(i));
                for(int j=0;j<pos_cons_D;j++)
                {
                    QLineF absconsole(QPointF(m_position.at(0),0),QPointF(m_position.at(j),0));
                    m_moment_Total.replace(j,(m_moment_Total.at(j)+charge.getMomentResultant_ConsoleG(pos_cons_G,absconsole.length(),getLg_Barre()-m_lg_console_G-m_lg_console_D)));
                }
            }
        }
        else
        {
            /// Calcul des moments entre deux appuis
            if(test.length()<(getLg_Barre()-m_lg_console_D))
            {
                charge.Concentre(m_lg_Barre-m_lg_console_G-m_lg_console_D,m_position.at(i),m_F.at(i));

                //on prend une charge

                for(int j=pos_cons_G;j<pos_cons_D;j++)// on va de la fin de la console gauche au début de la console droite
                {

                    //on calcul le moment de la charge à tous les abscisses
                    //puis on l'ajoute au tableau
                    m_moment_Total.replace(j,m_moment_Total.at(j)+charge.getMomentResultant_travee(m_position.at(j)));
                    // qDebug()<<"moment en travée index "<<j<<" | "<<m_moment_Total.at(j);
                }

            }
            ///Calcul des moments sur la console droite
            else
            {
                if(m_lg_console_D>0)/////A FAIRE
                {
                 charge.ConcentreConsoleD(m_position.at(i)- (m_lg_Barre - m_lg_console_D),m_F.at(i));
                }
            }
        }
    }

    ///des calculs des moments
    int noeudDeb=m_noeudDeb-1;
    int noeudFin=m_noeudFin-1;

    /// on cherche le moment max entre les noeuds

    m_MfyTravee=0;
     for(int i=noeudDeb*m_tailleTableaux;i<noeudFin*m_tailleTableaux+1;i++)

    {
         qDebug()<<"i="<<i<<m_moment_Total.at(i);
        if(qAbs(m_MfyTravee)<=qAbs(m_moment_Total.at(i)))
        {
            m_MfyTravee=m_moment_Total.at(i);
        }
    }


}

double Matrice_Chargement::getMfy(int noeudDeb,int noeudFin,int precision)
{
    Charge charge;

    int pos_cons_G=0;
    int pos_cons_D=0;
    m_moment_Total.clear();
    for (int i=0;i<m_tailleTableaux ;i++) {m_moment_Total.append(0);}

    for(int i=0;i<m_position.count();i++)
    {   QLineF test(QPointF(m_position.at(0),0),QPointF(m_position.at(i),0));

        if(test.length()==m_lg_console_G) /// recherche index console gauche
        {
            pos_cons_G=i;
            break;
        }
    }
    if(pos_cons_D==0){pos_cons_D=m_position.count();}


    for(int i=0;i<m_position.count();i++)
    {   QLineF test(QPointF(m_position.at(0),0),QPointF(m_position.at(i),0));// sert à déterminer la longueur ou l'on se trouve dans la barre

        ///Calcul des moments sur la console gauche
        if(test.length()<m_lg_console_G)
        {
            if(qAbs(m_F.at(i))>0)
            {
                charge.ConcentreConsoleG(m_lg_console_G,test.length(),m_F.at(i));
                for(int j=0;j<pos_cons_D;j++)
                {
                    QLineF absconsole(QPointF(m_position.at(0),0),QPointF(m_position.at(j),0));
                    m_moment_Total.replace(j,(m_moment_Total.at(j)+charge.getMomentResultant_ConsoleG(pos_cons_G,absconsole.length(),getLg_Barre()-m_lg_console_G-m_lg_console_D)));
                }
            }
        }
        else
        {            
            /// Calcul des moments entre deux appuis
            if(test.length()<(getLg_Barre()-m_lg_console_D))
            {
                charge.Concentre(m_lg_Barre-m_lg_console_G-m_lg_console_D,m_position.at(i),m_F.at(i));

                //on prend une charge

                for(int j=pos_cons_G;j<pos_cons_D;j++)// on va de la fin de la console gauche au début de la console droite
                {

                    //on calcul le moment de la charge à tous les abscisses
                    //puis on l'ajoute au tableau
                    m_moment_Total.replace(j,m_moment_Total.at(j)+charge.getMomentResultant_travee(m_position.at(j)));
                    // qDebug()<<"moment en travée index "<<j<<" | "<<m_moment_Total.at(j);
                }

            }
            ///Calcul des moments sur la console droite
            else
            {
                if(m_lg_console_D>0)/////A FAIRE
                {               
                 charge.ConcentreConsoleD(m_position.at(i)- (m_lg_Barre - m_lg_console_D),m_F.at(i));
                }
            }
        }
    }

    ///des calculs des moments

    noeudDeb=noeudDeb-1;noeudFin=noeudFin-1;
    /// on cherche le moment max entre les noeuds

    double d=0;
    for(int i=noeudDeb*precision;i<noeudFin*precision+1;i++)
    {        
        if(qAbs(d)<=qAbs(m_moment_Total.at(i)))
        {
            d=m_moment_Total.at(i);
        }
    }
    return d;

}

double Matrice_Chargement::getFleche()
{
    double resultat=0;
    for(int i=0;i<m_position.count();i++)
    {
        Charge ch;
        ch.Concentre(m_lg_Barre,m_position.at(i),m_F.at(i));
        resultat=resultat+ch.getFl();
      //  qDebug()<<"Fleche a "<<position.at(i)<<" | "<<"F="<<F.at(i)<<" | " <<ch.getFl()/10000000;


    }
    return resultat;
}

QList<double> Matrice_Chargement::getTabMfy(){return m_moment_Total;}



QList<double> Matrice_Chargement::getF() const
{
    return m_F;
}

void Matrice_Chargement::setF(const QList<double> &F)
{
    m_F = F;
}

QList<double> Matrice_Chargement::getPosition() const
{
    return m_position;
}

void Matrice_Chargement::setPosition(const QList<double> &position)
{
    m_position = position;m_tailleTableaux=m_position.count();
    m_moment_Total.clear();
    for (int i=0;i<m_position.count();i++) {m_moment_Total.append(0);}
}

double Matrice_Chargement::getLg_console_G() const
{
    return m_lg_console_G;
}

void Matrice_Chargement::setLg_console_G(double lg_console_G)
{
    m_lg_console_G = lg_console_G;
}

double Matrice_Chargement::getLg_console_D() const
{
    return m_lg_console_D;
}

void Matrice_Chargement::setLg_console_D(double lg_console_D)
{
    m_lg_console_D = lg_console_D;
}

double Matrice_Chargement::getLg_Barre() const
{
    return m_lg_Barre;
}

void Matrice_Chargement::setLg_Barre(double lg_Barre)
{
    m_lg_Barre = lg_Barre;
}

double Matrice_Chargement::getMfyTravee() const
{
    return m_MfyTravee;
}

int Matrice_Chargement::getNoeudDeb() const
{
    return m_noeudDeb;
}

void Matrice_Chargement::setNoeudDeb(int noeudDeb)
{
    m_noeudDeb = noeudDeb;
}

int Matrice_Chargement::getNoeudFin() const
{
    return m_noeudFin;
}

void Matrice_Chargement::setNoeudFin(int noeudFin)
{
    m_noeudFin = noeudFin;
}

int Matrice_Chargement::getPrecision() const
{
    return m_precision;
}

void Matrice_Chargement::setPrecision(int precision)
{
    m_precision = precision;
}


//// SECTION SUR LES TORSEURS

///Laisons : dx,dy,dz,rx,ry,rz
/// va servir à transmetre ou non les efforts à l'appui
const std::vector<int> Torseur::liaisons_glissant{1,0,0,0,0,1};
const std::vector<int> Torseur::liaisons_articule{0,0,0,0,0,1};
const std::vector<int> Torseur::liaisons_encastre{0,0,0,0,0,0};



Torseur::Torseur(double x,double y,double z,double Fx, double Fy, double Fz, double Mx, double My, double Mz,double EIz)
{    
    coord.setX(x);
    coord.setY(y);
    coord.setZ(z);
    F.setX(Fx);
    F.setY(Fy);
    F.setZ(Fz);
    M.setX(Mx);
    M.setY(My);
    M.setZ(Mz);
    EI=EIz;
}

QVector3D Torseur::getCoordonnees(){return coord;}
QVector3D Torseur::getEfforts(){return F;}
QVector3D Torseur::getMoments(){return M;}

void Torseur::translateConsole(QVector3D point)
{
   coord+=(point);
   M= QVector3D::crossProduct(coord,F);
}

void Torseur::translateEntreAppuis(QVector3D point, QVector3D debBarre, QVector3D finBarre)
{
    QVector3D barre(debBarre);
    barre+=(finBarre);
    qDebug()<<"Longueur barre"<<barre.length();
    qDebug()<<"calcul des rotation d'une section";

    QVector3D segment(debBarre);segment+=(point);
    qDebug()<<"Longueur segment"<<segment.length();

    ///théorème des trois moments
    qDebug()<<"calcul des rotation isostatique d'une section";


    double theta_east,theta_west,ai,bi,ci,Mzi_east,Mzi_west;
    double theta_east_0=0;
    double theta_west_0=0;
    //valeur Mzi TEST

    Mzi_east=2500;
    Mzi_west=300;

    ci=ai=barre.length()/(3.0*EI);
    bi=barre.length()/(6.0*EI);
    theta_east=theta_east_0-(ai*Mzi_east)-(bi*Mzi_west);
    theta_west=theta_west_0+(bi*Mzi_east)+(ci*Mzi_west);

    qDebug()<<"Rotation au niveau de l'appuis avec le moment"<<theta_east;
    qDebug()<<"Rotation sur l'appuis suivant"<<theta_west;

}



Trois_appuis::Trois_appuis()
{

}










/////Création d'un tableau vide
QList<double> Maths::init_matrice_ligne(int nbr_colonnes)
{
    int c=nbr_colonnes;
    QList<double> tab;

    for (int i=0;i<c;i++)
    {
        tab.push_back(0.0);
    }
    return tab;
}


/////Création d'un tableau 2D vide
QList<QList<double> > Maths::init_matrice(int nbr_lignes, int nbr_colonnes)
{
    int l= nbr_lignes;
    int c=nbr_colonnes;
    QList<QList<double>> tab;

    for (int i=0;i<l;i++)
    {
        QList <double> lignes;
        for (int j=0;j<c;j++) {
            lignes.push_back(0);
        }
        tab.push_back(lignes);
    }
    return tab;


}

///théorème des trois moments ou methode de Clapeyron
/// (rotation aux appuis pour une force ponctuelle
double Trois_appuis::getRotationTroisMoment_Ponctuelle(double P, double a, double b, double l)
{
    a=a/1000;
    b=b/1000;
    l=l/1000;
    P=P/1000;


    return -P*a*b*(a+l)/l;
}





///Crée une matrice triangulaire supérieur à partir d'une matrice carrée quelconque
///
QList<QList<double> > Maths::Matrice_Triangularisation(QList<QList<double> > Matrice)
{

    QList<QList<double>> A=Matrice;
    int n=0; //nbr de lignes de la matrice
    int m=0; //nbr de colonne de la matrice
    n=A.size();
    m=A[0].size();
    double p=0;
    double q=0;

    for (int k=0;k<n;k++) {
        p=A[k][k];
        for (int i = k+1; i < n; i++) {
            q=A[i][k];
            A[i][k]=0;

            for (int j=k+1;j<m;j++) {
                A[i][j]=A[i][j]-A[k][j]*q/p;
            }
        }
    }
    return A;
}

////additionne deux matrices carrées
QList<QList<double> > Maths::Matrice_Addition(QList<QList<double> > Matrice_carre1, QList<QList<double> > Matrice_carre2)
{
    QList<QList<double> > A = Matrice_carre1;
    QList<QList<double> > B = Matrice_carre2;
    QList<QList<double> > result;
    int dim_A; //dimension de la matrice A
    int dim_B; //dimension de la matrice B

    dim_A=A.size();
    dim_B=B.size();

    if (dim_A==dim_B){
        result=Maths().init_matrice(dim_A,dim_A);
        for (int i=0;i<dim_A;i++) {
            for (int  j= 0;  j< dim_A; j++) {
                result[i][j]=A[i][j]+B[i][j];
            }
        }

        return result;
    }

    return result;
}


///concatène deux matrices
QList<QList<double> > Maths::Matrice_Assemblage(QList<QList<double> > A, QList<double> b)
{
    int n=A.size();//taille de la mtrice A
    int m=b.size();//taille de la matrice b

    if(n==m){
        for (int i =0;i<n;i++) {
            A[i].append(b[i]);
        }
    }
    return A;
}

///dé-concatène deux matrices haha
void Maths::Matrice_Desassemblage(QList<QList<double> > A)
{
    int n=A.size();
    QList<double> b;
    double val;
    QList<QList<double>> result;

    for (int i =0;i<n;i++) {
        val=A[i].takeLast();
        b.append(val);
    }
    setA(A);
    setB(b);



}

///inverse une matrice carrée
/// Pas de zéro sur la diagonale de la matrice !!!
QList<double> Maths::Matrice_Inversion(QList<QList<double> > Matrice_triangle_A, QList<double> Matrice_b)
{
    QList<QList<double>> A=Matrice_triangle_A;
    QList<double> B=Matrice_b;
    int dim_A;
    dim_A=A.size();
    QList<double> X=init_matrice_ligne(dim_A);
    double somme;

    if (B.size()==dim_A){
        X[dim_A-1]=(B[dim_A-1]/A[dim_A-1][dim_A-1]);
        for (int i=dim_A-2;i>=0;i--) {
            somme=0.0;
            for (int k=i+1;k<=dim_A-1;k++) {
                somme=somme+A[i][k]*X[k];
            }
            X[i]=((B[i]-somme)/A[i][i]);

        }

        return X;
    }

    return X;

}


///// résoud A * X = B
/// en donnant A et B
QList<double> Maths::Matrice_Resolution_Systeme(QList<QList<double> > Matrice_carre_A, QList<double> Matrice_b)
{
    QList<QList<double>> A=Matrice_carre_A;
    QList<double> b=Matrice_b;


    Matrice_Desassemblage(Matrice_Triangularisation(Matrice_Assemblage(A,b)));

    A=getA();
    b=getB();

    return Matrice_Inversion(A,b);

}

QList<QList<double> > Maths::getA() const{return m_A;}

void Maths::setA(const QList<QList<double> > &A){m_A = A;}

QList<double> Maths::getB() const{return m_B;}

void Maths::setB(const QList<double> &B){m_B = B;}


