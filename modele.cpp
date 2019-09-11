#include "modele.h"



Modele::Modele(Libeurocod2 *lib)
{
    m_lib=new Libeurocod2();
    if(m_lib != nullptr){m_lib=lib;}
}

void Modele::clearModele()
{
    m_noeuds.clear();//noeuds.resize(0);
    m_liaisons.clear();//liaisons.resize(0);
    m_relax.clear();//relax.resize(0);
    m_sections.clear();//sections.resize(0);
    m_Liste_Barres.clear();
}

void Modele::quickBarre(QList<double> lgtroncons,double bBarre,double hBarre,QString materiau, QString classeBois, QString classeService, QString dureeCharge)
{
    ///fonction servant à créeer une barre horizontale sur x appuis
    clearModele();
    addNoeud(0,0);
    addAppuis(1,Appuis::Articule);
    double coord=0;
    for (int i=0;i<lgtroncons.count();i++) {

        coord=coord+lgtroncons.at(i);
        addNoeud(coord,0);

        addLiaison(i+1,i+2,bBarre,hBarre);
        addAppuis(i+2,Appuis::Articule);

        ///relaxations
        if(i==0){setRelaxation(i+1,true,false);}
        if(i==lgtroncons.count()-1){setRelaxation(i+1,false,true);}
        if(lgtroncons.count() == 1){setRelaxation(1,true,true);}
    }
    createBarres();
    getBarre(1)->setCaracteristiques(materiau,classeBois,classeService,dureeCharge);

}

void Modele::addNoeud(double x, double y)
{
    QPointF *n=new QPointF(x,y);
    m_noeuds.push_back(n);

    QList <bool> b;for (int i=0;i<4;i++) {b.append(false);}
    while(m_noeuds.count()>=m_appuis.count()){m_appuis.push_back(b);}
}
QList<int> Modele::getNoeuds(){QList<int> l;for(int i=0;i<m_noeuds.count();i++){l.append(i+1);}return l;}

void Modele::addLiaison(int noeudDebut, int noeudFin, double b_Debut, double H_Debut, double b_Fin, double H_Fin)
{
    noeudDebut=noeudDebut-1;
    noeudFin=noeudFin-1;
    QList <double>sec;
    //std::vector<double>sec(4);
    sec.append(H_Debut);sec.append(b_Debut);
   // sec.at(0)=H_Debut;sec.at(1)=b_Debut;
    if(H_Fin ==0.0){
        //sec.at(2)=H_Debut;
        sec.append(H_Debut);
    }else{
        sec.append(H_Fin);
        //sec.at(2)=H_Fin;
    }
    if(b_Fin ==0.0)
    {
        sec.append(b_Debut);
        //sec.at(3)=b_Debut;
    }else{
        sec.append(b_Fin);
       // sec.at(3)=b_Fin;
    }
    m_sections.push_back(sec);
    QList<int>b;
    b.append(noeudDebut);b.append(noeudFin);m_liaisons.push_back(b);
    m_relax.clear();//relax.resize(0);
    for(int i=0;i<m_liaisons.count();i++)
    {
        QList<bool>relaxtemp;
        relaxtemp<<false<<false;
        m_relax.push_back(relaxtemp);
    }
}

void Modele::addAppuis(int noeud, Appuis typeApp)
{
    noeud=noeud-1;
   // std::vector <bool> b(4,false);
    QList <bool> b;
    for (int i=0;i<4;i++) {b.append(false);}


    switch(typeApp)
    {
    case Appuis::Encastre:
    break;

    case Appuis::Articule:
        b.replace(3,true);
        break;

    case Appuis::Glissant:
        b.replace(3,true);
        b.replace(0,true);
        break;


    }
    if(!(noeud> m_noeuds.count()-1)){
            m_appuis.replace(noeud,b);
    }
    else {
        qDebug()<<"------------------Le noeud "<< noeud +1<<" n'existe pas !!------------------";
    }

}


void Modele::setRelaxation(int liaison, bool debut, bool fin)
{
    liaison=liaison-1;
    QList<bool>relaxtemp;relaxtemp<<debut<<fin;
    m_relax.replace(liaison,relaxtemp);
}

void Modele::setCombinaisons()
{
    for (int i=0;i<m_Liste_Barres.count();i++) {
        m_Liste_Barres.at(i)->setCombi_ELU();
        m_Liste_Barres.at(i)->setCombi_ELS();
    }
}

void Modele::createBarres()
{
    int noeudfin=0;int noeuddeb=0;m_Liste_Barres.clear();

    qDebug()<<"relax "<<m_relax.count();
    qDebug()<<"Liaisons"<<m_liaisons.count();
    qDebug()<<"Noeuds"<<m_noeuds.count();
    QList<QPointF *>points;
    QList<QList <bool> > app;

    //premier noeud
    points.push_back(m_noeuds.at(m_liaisons.at(0).at(0)));

    for(int i=0;i<m_liaisons.count();i++)//on parcourt les barres
    {

        if(m_relax[i][1])// on cherche si la fin de la barre est relaxée
        {
            noeudfin=m_liaisons[i][1];
            QLineF br(*m_noeuds.at(noeuddeb),*m_noeuds.at(noeudfin));

            points.push_back(m_noeuds.at(m_liaisons[i][1]));

            Barre *barre=new Barre(noeuddeb,noeudfin,m_lib);
            barre->setListePoints(points);
            qDebug()<<"noeud début"<<noeuddeb<<"|noeud fin"<<noeudfin;
            for(int j=noeuddeb;j<points.size();j++){app.push_back(m_appuis.at(j));}
            barre->setb(m_sections[i][1]);
            barre->setH(m_sections[i][0]);
            m_Liste_Barres.push_back(barre);
            barre->setListeAppuis(app);
            qDebug()<<"nbre d'appuis trouvés"<<barre->getNbreAppuis();
            qDebug()<<"longueur "<<barre->getLg();
            noeuddeb=noeudfin;
            points.clear();//points.resize(0);
            app.clear();//app.resize(0);
            points.push_back(m_noeuds.at(m_liaisons[i][1]));
        }//sinon on ajoute ce noeud à la liste de noeuds de la barre
        else
        {
            qDebug()<<"salut";
            points.push_back(m_noeuds.at(m_liaisons[i][1]));
        }
    }
}

Barre *Modele::getBarre(int numBarre)
{
    try{return m_Liste_Barres.at(numBarre-1);}
    catch(int e){qDebug()<<e<<"La barre "+QString().setNum(numBarre)+" nexiste pas !!";}
    return nullptr;
}



int Modele::getNbreBarre(){return m_Liste_Barres.count();}

bool Modele::isAppuis(int noeud)
{
    bool test=false;
    QList<bool> b;
    b=m_appuis.at(noeud);
    for(int i=0;i<b.size();i++){if(b.at(i)){test=true;}}
    return test;
}




//////// CREATION D'UNE BARRE POUR SIMPLIFIER L'UTILISATION DE LA LIBRAIRIE EC5////////////////////////////////////////////////////////////////////////////
//////// et servant aux chargements ainsi qu'aux calculs rdm

/*
 * TODO transformer les std::vector en QList  et les insérer dans le .h
 */

std::vector<double> flambement(5);

Barre::Barre(int numNoeudDeb, int numNoeudFin, Libeurocod2 *lib)
{
    m_precision=50;///precision du découpage des charges et des barres

    //qDebug()<<"Barre::Barre";
    for (int i=0;i<10;i++) {m_caracteristiques<<"";}
    m_categories_actions_variables<<""<<""<<""<<"Alt. inf. 1000m"<<"Vent"<<"Vent"<<"Vent"<<"Vent";///en fonction des cas de charges
    m_noms_cas_Charge<<""<<"G"<<"Q"<<"S"<<"W1"<<"W2"<<"W3"<<"W4";
   // Q_is_Entretien=false;
    ec5=new Libeurocod2();
    if(lib != nullptr){/*qDebug()<<"Barre pointeur non null";*/ec5=lib ;}
    else {qDebug()<<"nullpointer";}
    m_ndeb=new QPointF;
    m_nfin=new QPointF;
    m_numNoeudDeb=numNoeudDeb+1;
    m_numNoeudFin=numNoeudFin+1;
    m_geometrie.clear();for (int i=0;i<30;i++) {m_geometrie.append(0);}
    m_charges.clear();
    m_catQ=Libeurocod2::CategorieCharge::Null;
    m_catS=Libeurocod2::CategorieCharge::Null;
    m_matrice=new Matrice_Chargement;
    m_noeuds.clear();
    kmod=0;gammaM=0;m_action_variable.clear();    
   }

Barre::Barre()
{
    m_matrice=new Matrice_Chargement;

}

void Barre::setListePoints(QList<QPointF*> points)
{
    m_noeuds=points;

    m_ndeb=points.at(0);
    m_nfin=points.at(points.size()-1);
     //for (int i=0;i<points.size();i++){QPointF test=points.at(i);}
}

QList <QPointF*> Barre::getListePoints(){return m_noeuds;}

std::vector<double> Barre::getCoordonnees()
{
    std::vector<double> d;
    for(int i=0;i<m_charges.count();i++)
    {
        d.push_back(m_charges.at(i).at(2));
    }
    return d;
}

void Barre::setListeAppuis(QList<QList<bool> > app)
{
    m_appuis.clear();
    for (int i=0;i<app.count();i++) {m_appuis.append(app.at(i));}
    clearChargements();
}




void Barre::setGeometrie(bool articule_pied, int articule_tete, int atf_Y, int atf_Z,double H_2,double bande_charge)
{
    double Lg;Lg=getLg();
    double b=getb();
    double H=getH();

    m_geometrie.replace(2,Lg);//Longueur réel
    art_pied=articule_pied;//articulations
    art_tete=articule_tete;
    m_geometrie.replace(10,Lg/(atf_Y+1));//Lg flambement selon Y
    m_geometrie.replace(11,Lg/(atf_Z+1));//Lg flambement selon Z
    m_geometrie.replace(12,H_2);//hauteur à inertie variable


   //calcul de la hauteur moyenne
    if(H_2>H)
    {
        double testH, val1,val2;
        testH=H/H_2;
        if(testH > 0.8){m_geometrie.replace(13,(H+H_2)/2.0);}
        else
        {
           val1=H+(H/Lg)*((1.0/3.0)* m_geometrie.at(10));
           val2=H+(H/Lg)*((H/H_2)* m_geometrie.at(10));
           m_geometrie.replace(13,qMin(val1,val2));
        }
    }
    else {m_geometrie.replace(13,0);}

    m_geometrie.replace(14,bande_charge);

}



int Barre::setCaracteristiques(QString materiau, QString classeBois, QString classeService, QString dureeCharge, QString type_element, bool ksys)
{
   /* if(materiau.isEmpty()||classeBois.isEmpty()||classeService.isEmpty()||dureeCharge.isEmpty()||Categorie.isEmpty())
    {
        qDebug()<<"Barre::setCaracteristiques,ERREUR arguments manquants";
        return -1;
    }*/

    qDebug()<<"Barre::setCaracteristiques";

    bois_Massif=true;
    m_caracteristiques.replace(0,materiau);
    m_caracteristiques.replace(1,classeBois);
    m_caracteristiques.replace(2,classeService);
    m_caracteristiques.replace(3,dureeCharge);
    //m_caracteristiques.replace(4,QString().setNum(categorieQ));
    m_caracteristiques.replace(5,type_element);
    if(ksys){m_caracteristiques.replace(6,QString().setNum(1));}
    else{m_caracteristiques.replace(6,QString().setNum(0));}
    kmod=ec5->getkmod(classeService,dureeCharge);
    gammaM=ec5->getgammaM(materiau);
    if(materiau.contains("lamellé",Qt::CaseInsensitive)){bois_Massif=false;}
   // double poidspropre=ec5->getMasseVolMoy(classeBois)*(getb()/1000.0)*(getH()/1000.0);
    clearChargements();

    return 0;
}



double Barre::getb(){return m_geometrie.at(0);}
void Barre::setb(double b){m_geometrie.replace(0,b);}
double Barre::getH(){return m_geometrie.at(1);}
void Barre::setH(double H){m_geometrie.replace(1,H);}
int Barre::getNbreNoeuds(){return m_noeuds.count();}

bool Barre::isAppuis(int numNoeuds)
{
    bool test=false;

    numNoeuds=numNoeuds-1;

        QList<bool> b=m_appuis.at(numNoeuds);
        for(int j=0;j<b.size();j++)
        {
            if(b.at(j)==true){test=true;}
        }   
        return test;
}

double Barre::getNoeud_X(int numNoeud){return m_noeuds.at(numNoeud-1)->x();}
double Barre::getNoeud_Y(int numNoeud){return m_noeuds.at(numNoeud-1)->y();}

double Barre::getLgConsole_G()
{
    double lgcg=0;
    if(!isAppuis(1))
    {
        for(int i=0;i<m_noeuds.count();i++)
        {
            if(isAppuis(i+1)){lgcg=getNoeud_X(1)-getNoeud_X(i+1);break;}
        }
    }
    else{lgcg=0;}

    return qAbs(lgcg);
}

double Barre::getLgConsole_D()
{
    double lgcd=0;
    if(!isAppuis(m_noeuds.count()))
    {
        for(int i=m_noeuds.count();i>0;i--)
        {
            if(isAppuis(i-1)){lgcd=getNoeud_X(m_noeuds.count())-getNoeud_X(i-1);break;}
        }
    }
    else{lgcd=0;}

    return qAbs(lgcd);
}



QList<double> Barre::getLg_Travees()
{
    QList <double> travees;

    if(getLgConsole_G()>0){travees.push_back(getLgEntreNoeuds(1,getListeAppuis().at(0)));}
    for(int i=0;i<getListeAppuis().size()-1;i++)
    {
        travees.push_back(getLgEntreNoeuds(getListeAppuis().at(i),getListeAppuis().at(i+1)));
    }
    return travees;
}

std::vector<int> Barre::getNoeuds_Travee(int travee)
{
    std::vector <int>  n(0);
    travee=travee-1;

    if((getLgConsole_G()>0.0) &&( travee==0) )// donne le noeud 1 pour la console
    {
        n.push_back(1);n.push_back(getListeAppuis().at(0));
    }
    else
    {
        if(getLgConsole_G()>0.0)
        {
            n.push_back(getListeAppuis().at(travee-1));
            n.push_back(getListeAppuis().at(travee));
        }
        else
        {
            n.push_back(getListeAppuis().at(travee));
            n.push_back(getListeAppuis().at(travee+1));
        }

    }
    return n;
}

int Barre::getNbreTravees(){return getLg_Travees().size();}

double Barre::getLgEntreNoeuds(int noeudA, int noeudB)
{
    QLineF ligne(*m_noeuds.at(noeudA-1),*m_noeuds.at(noeudB-1));
    return ligne.length();
}

QList<int> Barre::getListeAppuis()
{
    //int test=-1;
    QList<int> sortie;
    for(int i=0;i<m_appuis.count();i++)
    {        
      if(isAppuis(i+1))
      {
          sortie.append(i+1);
      }
    }
    return sortie;
}
int Barre::getNbreAppuis(){return getListeAppuis().count();}

double Barre::getLg()
{

   QLineF ligne(*m_noeuds.at(0),*m_noeuds.at(m_noeuds.count()-1));
   return ligne.length();
}

void Barre::setLg(){m_geometrie.replace(2,QLineF(*m_ndeb,*m_nfin).length());}



double Barre::getLg_fY(){return m_geometrie.at(10);}//lg flambement selon Y
double Barre::getLg_fZ(){return m_geometrie.at(11);}
double Barre::getH_2(){return m_geometrie.at(12);}//2eme hauteur pour poteau à inertie variable
double Barre::getH_moy(){return m_geometrie.at(13);}

double Barre::getI(bool Iz) //moment quadratique b *h3 /12
{
    double d=0;
    if(!Iz){d=(getb()*qPow(getH(),3))/12.0;}///Ix
    else{d=(getH()*qPow(getb(),3))/12.0;}///Iy
    return d;
}

double Barre::getIV(bool IVz)//module d'inertie I / H/2
{
    double d=0;
    if(!IVz){d= getI() / (getH()/2.0);}/// I/Vy
    else {d=getI(IVz)/(getb()/2.0);}    /// I/Vz
    return d;
}


double Barre::getBande_de_charge(){return m_geometrie.at(14);}

QString Barre::getMateriau(){return m_caracteristiques.at(0);}
QString Barre::getClasseBois(){return m_caracteristiques.at(1);}
QString Barre::getclasseService(){return m_caracteristiques.at(2);}
QString Barre::getDuree_Charge(){return m_caracteristiques.at(3);}
//QString Barre::getCategorie(){return m_caracteristiques.at(4); }
QString Barre::getTypeElement(){return m_caracteristiques.at(5);}
double Barre::getksys(){return ec5->getksys(QString(m_caracteristiques.at(6)).toInt());}

Libeurocod2::CategorieCharge Barre::getCatS() const{return m_catS;}
void Barre::setCatS(const Libeurocod2::CategorieCharge &catS){m_catS = catS;}


Libeurocod2::CategorieCharge Barre::getCatQ() const{return m_catQ;}
void Barre::setCatQ(const Libeurocod2::CategorieCharge &catQ){m_catQ = catQ;}




void Barre::clearChargements()
{
    m_charges.clear();
    QList <double> d;for(int i=0;i<13;i++){d.append(0);}
    //////////////tableau de chargements/////////////////
    /// voir add chargements

    double lseg=getNoeud_X(1);
    d.replace(2,lseg);
    for(int i=0;i<(m_noeuds.count()-1);i++)
    {
        m_charges.push_back(d);
        for(int j=0;j<m_precision-1;j++)
        {
           lseg=lseg+getLgEntreNoeuds(i+1,i+2)/m_precision;
           d.replace(2,lseg);
           m_charges.push_back(d);
           if(j==m_precision-2){lseg=lseg+getLgEntreNoeuds(i+1,i+2)/m_precision;d.replace(2,lseg);}//d.at(2)=lseg;}
        }
    }
    m_charges.push_back(d);
}

void Barre::calcul2App()
{
    qDebug()<<"calcul sur 2 appuis";
    m_matrice->setF(m_eff_pond);
    m_matrice->setPosition(m_coordonees);
    m_matrice->setLg_Barre(getLg());
    m_matrice->setLg_console_G(getLgConsole_G());
    m_matrice->setNoeudDeb(m_numNoeudDeb);
    m_matrice->setNoeudFin(m_numNoeudFin);
    m_matrice->calculer();
    tabMomFlex=m_matrice->getTabMfy();

}

double Barre::getResistanceFlexion(bool G_seul)//fmd
{
    double kh=0;double d=0;
    kh=ec5->getkH(bois_Massif,getH());
    double kmod2=kmod;
    if(G_seul){kmod2=ec5->getkmod(getclasseService(),ec5->getListeClasseDuree().at(0));}
    d=ec5->getResistanceFlexion(ec5->getfmk(getClasseBois()),kmod2,gammaM,getksys(),kh);
    return d;
}

double Barre::getContrainteFlexion(int combi,int travee)//sigma_m
{
    double d=0;


    //double mfy=m_matrice->getMfy(getNoeuds_Travee(travee).at(0),getNoeuds_Travee(travee).at(1),m_precision);
    double mfy=m_matrice->getMfyTravee();
    d= mfy/ getIV();
    qDebug()<<"Mfy"<<mfy<<"getIV "<<getIV()<<"sigma md="<<mfy/getIV()<<" MPa";

    return qAbs(d);
}

double Barre::getMomentFlexionDelta()
{

    double test1=0;double test2=0;
    for(int i=0;i<m_combi_ELU.count();i++)
    {
        for(int j=1;j<getNbreTravees()+1;j++)
        {
            if(m_matrice->getMfy(getNoeuds_Travee(j).at(0),getNoeuds_Travee(j).at(1),m_precision)>0)
            {test1=qMax(qAbs(test1),qAbs(m_matrice->getMfy(getNoeuds_Travee(j).at(0),getNoeuds_Travee(j).at(1),m_precision)));}
            else{test2=qMax(qAbs(test2),qAbs(m_matrice->getMfy(getNoeuds_Travee(j).at(0),getNoeuds_Travee(j).at(1),m_precision)));}
        }
    }
    return test1+test2;
}

QList<double> Barre::getTabMoments(){return tabMomFlex;}

QList<double> Barre::getMomentsHyperS()
{

    QList<QList<double>> in=get_Effort_Barre();
    QList<double> out;
    int nbreSegments=getNbreSegments();
    for (int i=0;i<nbreSegments;i++) {out<<in[1][i];}
    return out;
}

QList<double> Barre::getEffTranchantHyperS()
{
    QList<QList<double>> in=get_Effort_Barre();
    QList<double> out;
    int nbreSegments=getNbreSegments();
    for (int i=0;i<nbreSegments;i++) {out<<in[0][i];}
    return out;

}


double Barre::getTauxtravailFlexion(double contrainte, double resistance){return qAbs(ec5->getTauxtravail(contrainte,resistance));}

double Barre::getDeversement(int combi)
{

    double e005=ec5->getE05pourc(getClasseBois());

    double sigma_crit=ec5->getContrainteCritique(e005,getb(),getH(),getLg_fY(),true,false,true);
    double elancement=ec5->getElancement_relatif_flexion(sigma_crit,ec5->getfmk(getClasseBois()));
    double kcrit=1.56 - (0.75* elancement);
    double sigma_m_crit=getContrainteFlexion(combi,1)/(kcrit * getResistanceFlexion());/// A modifier suivant les travées

    return sigma_m_crit;
}

double Barre::getFleche(ELS typefleche)
{
    double out=0,cumulfleche=0;double e0mean=0,cumul_fin_et_creep;
    bool netfinal=false;if(typefleche == ELS::NetFin){netfinal=true;}
    int indexflechemax=0;
    e0mean=ec5->getE0mean(getClasseBois());

    QString resultats;

    ///fleche net finale
    /// on calcul dans un premier temps la flèche finale
    if(netfinal){typefleche=ELS::Fin;}

    for(int i=0;i<m_combi_ELS.count();i++){
        if(m_combi_ELS.at(i).at(8)==QString().setNum(typefleche))
        {
           // setChargements(true,i);
            //Matrice_Chargement m(m_eff_pond,m_coordonees,getLg(),m_appuis,getLgConsole_G(),0);
            cumulfleche=qAbs(m_matrice->getFleche()/(2* e0mean * getI()));
            if(out<=cumulfleche)
            {
                out=cumulfleche;
                indexflechemax=i;
            }
        }
    }
    resultats.append(m_combi_ELS.at(indexflechemax).at(0));

    ///fleche net finale
    /// puis wcreep * kdef
   if(netfinal){
       cumul_fin_et_creep=out;
       out=0;
        typefleche=ELS::Creep;
        for(int i=0;i<m_combi_ELS.count();i++){
            if(m_combi_ELS.at(i).at(8)==QString().setNum(typefleche))
            {
                setChargements(true,i);
                Matrice_Chargement m(m_eff_pond,m_coordonees,getLg(),m_appuis,getLgConsole_G(),0);
                cumulfleche=qAbs(m.getFleche()/(2* e0mean * getI()));
                if(out<=cumulfleche)
                {
                    out=cumulfleche;
                    indexflechemax=i;
                }
            }
        }
        out=out+cumul_fin_et_creep;
        resultats.append(" | "+ m_combi_ELS.at(indexflechemax).at(0));

   }

   resultats.append(" | Flèche :"+QString().setNum(out)+"mm");

  //  qDebug()<<resultats;
    return out;
}

double Barre::getWnet_fin(int travee)
{
    if(travee ==1){}
    return 0;
}

double Barre::getWinstQ_limite(int travee)
{
    return ec5->getWinstQ_lim(getLg_Travees().at(travee-1),getTypeElement());
}
double Barre::getWnet_fin_limite()
{
    return ec5->getWnetfin_lim(getLg(),getTypeElement());
}
double Barre::getTauxtravailWinstQ(double WinstQ,double WinstQ_lim){return (WinstQ/WinstQ_lim) *100.0;}
double Barre::getTauxtravailWnet_fin(double Wnet_fin,double Wnet_fin_lim){return (Wnet_fin/Wnet_fin_lim) *100.0;}

double Barre::getContrainteCompressionAxiale(double Effort_DaN)
{
    double H=getH();
    if(getH_moy()>0){H=getH_moy();}
    return ec5->getContrainteCompressionAxiale(Effort_DaN,getb()*H);
}

double Barre::getResistanceCompressionAxiale(){return ec5->getResistanceCompressionAxiale(ec5->getfc0k(getClasseBois()),kmod,gammaM);}
double Barre::getTauxtravailCompressionAxiale(double contrainte, double resistance){return  (contrainte/(resistance*getFlambement_Max()))*100.0;}


// index 0 kcmax
// index 1 kcY
// index 2 kcZ
// index 3 elancement selon Y
// index 4 elancement selon Z
void Barre::setFlambement()
{
    double H=getH();
    if(getH_moy()>0){H=getH_moy();}
    flambement=ec5->getElancement_relatif(getLg_fY(),getLg_fZ(),getb(),H,ec5->getfc0k(getClasseBois()),ec5->getE05pourc(getClasseBois()),art_tete,art_pied,bois_Massif);

}

int Barre::getNumNoeudFin() const
{
    return m_numNoeudFin;
}

void Barre::setNumNoeudFin(int numNoeudFin)
{
    m_numNoeudFin = numNoeudFin;
}

int Barre::getNumNoeudDeb() const
{
    return m_numNoeudDeb;
}

void Barre::setNumNoeudDeb(int numNoeudDeb)
{
    m_numNoeudDeb = numNoeudDeb;
}

int Barre::getPrecision() const
{
    return m_precision;
}

int Barre::getNbreSegments(){return get_Effort_Barre()[0].count();}

QList<double> Barre::getCoordonees() const
{
    return m_coordonees;
}



////                                                                    Travaux d'antoine
///
///
///
///




////Matrices servant à résourde l'équation A * M = B
/// A= Matrice des coefficient créé à l'aide du théorème des 3 moments (méthode de Clapeyron)
/// M = Matrice des moments -> inconnus recherchés
/// B = Matrice des rotations aux appuis


////récupère le tableau des efforts sur la barre et leurs coordonnées
QList<QList <double>> Barre::getMcharges()
{
    QList<QList <double>> out;


    for (int i=0;i<m_charges.count();i++){
        QList<double> recup;
        recup.append(m_charges[i][2]);/// coordonnées
        recup.append(m_eff_pond.at(i));/// efforts suivant combinaisons
        out.append(recup);
    }

    return out;
}


///assemblage des matrices venant de set_lignei_Btab
QList<double> Barre::set_Matrice_B(QList<QList<double>> tab_force, int compteur_force)
{

    QList<int> list_appuis = getListeAppuis();
    QList<double> B;

    for (int i =0;i<list_appuis.count();i++) {
        B.append(set_lignei_Btab(tab_force,list_appuis[i],compteur_force));
    }
    return B;
}

/// recherche de la rotation à un appuis précis
double Barre::set_lignei_Btab(QList<QList<double>> tab_force,int numero_appui,int compteur_force)
{   
    QList<double> ltab=getLg_Travees();
    QList<double> Ftab=tab_force[compteur_force];
    QList<int> list_appuis=getListeAppuis();
    double L=0;
    double a=0;
    double b=0;
    double li=0;
    double d=0;
    double debut;
    double fin;
    int n=0;

    double result=0;

    //Situe sur quelle travée est la force
    while (Ftab[0]>=L) {
        L=L+ltab[n];
        n++;//n donne le numéro de la travée sur laquelle se trouve la force
    }

    debut=getNoeud_X(getNoeuds_Travee(n)[0]);
    fin=getNoeud_X(getNoeuds_Travee(n)[1]);

    li=fin-debut;//Longueur de la travée de la force

    d=Ftab[0];//abscisse de la force sur la poutre
    a=(d-debut);
    b=(li-a);

    //Attention, les conditions après && font mauvais ménage avec les portes à faux !!

    if ((numero_appui==getNoeuds_Travee(n)[1])&&(numero_appui!=list_appuis.last())){//||(numero_appui==getNoeuds_Travee(n)[1])){

        Trois_appuis *trois =new Trois_appuis;
        result=trois->getRotationTroisMoment_Ponctuelle(Ftab[1],a,b,li);
    }

    else {
        if((numero_appui==getNoeuds_Travee(n)[0])&&(numero_appui!=list_appuis.first())){
            Trois_appuis *trois =new Trois_appuis;
            result=trois->getRotationTroisMoment_Ponctuelle(Ftab[1],b,a,li);
        }

    }

    return result;
}

///assemblage des matrices venant de set_lignei_Atab
QList<QList<double> > Barre::set_Matrice_A()
{
    QList<int> list_appuis = getListeAppuis();
    QList<QList<double>> A;

    A=Maths().init_matrice(list_appuis.count(),list_appuis.count());

    for (int i =0;i<list_appuis.count();i++) {
        A=Maths().Matrice_Addition(A,set_lignei_Atab(list_appuis[i],i));
    }

    return A;
}

/// recherche des coefficients à un appuis précis
QList<QList<double> > Barre::set_lignei_Atab(int numero_appui, int compteur_appui)
{
    QList<double> Tab_travee;
    QList<int> list_appuis;
    int before=0;
    int after=0;

    list_appuis=getListeAppuis();

    QList<QList<double>> Atab=Maths().init_matrice(list_appuis.count(),list_appuis.count());

    for (int i=0;i<list_appuis.count()-1;i++) {
        if (numero_appui==list_appuis[i]){
            before=list_appuis[i-1];
            after=list_appuis[i+1];
    }

    ///Ici numero_appui sert à retrouver la travée
    /// compteur_appui est l'appui sur lequel on se trouve
    }
    ///Cette condition aussi est importante pour le porte à faux
    if((compteur_appui!=0)&&(compteur_appui!=list_appuis.count()-1)){
        Tab_travee.append(getLgEntreNoeuds(before,numero_appui)/1000);
        Tab_travee.append(getLgEntreNoeuds(numero_appui,after)/1000);
    }

    ///C'est dans cette condition qu'il faudra ajouter un truc pour les portes à faux
    if (compteur_appui==0){
        Atab[compteur_appui][compteur_appui]=1;
        //Atab[numero_appui-1][numero_appui-1]=2*(Tab_travee[0]+Tab_travee[1]);
        //Atab[numero_appui-1][numero_appui]=Tab_travee[1];
    }
    ///Et dans celle là pour le porte à faux de droite
    else{if (compteur_appui==list_appuis.count()-1){
            Atab[compteur_appui][compteur_appui]=1;
            //Atab[numero_appui-1][numero_appui-2]=Tab_travee[0];
            //Atab[numero_appui-1][numero_appui-1]=2*(Tab_travee[0]+Tab_travee[1]);
        }
        else{
            Atab[compteur_appui][compteur_appui-1]=Tab_travee[0];
            Atab[compteur_appui][compteur_appui]=2*(Tab_travee[0]+Tab_travee[1]);
            Atab[compteur_appui][compteur_appui+1]=Tab_travee[1];
        }
    }

    return Atab;

}

///
/// Réactions aux appuis dû à un effort ponctuel sur une travée
QList<double> Barre::getReaction_effort(QList<QList<double>> tab_force,int compteur_force)
{
    QList<double> ltab=getLg_Travees();
    QList<double> Ftab=tab_force[compteur_force];
    QList<double> result;
    double Rgauche=0;
    double Rdroite=0;
    double L=0;
    double a=0;
    double b=0;
    double li=0;
    double d=0;
    double debut;
    double fin;
    int n=0;

    //Recherche sur quelle travée est la force
    while (Ftab[0]>=L) {
        L=L+ltab[n];
        n++;//n donne le numéro de la travée sur laquelle se trouve la force
    }

    debut=getNoeud_X(getNoeuds_Travee(n)[0]);
    fin=getNoeud_X(getNoeuds_Travee(n)[1]);


    li=fin-debut;//Longueur de la travée de la force

    d=Ftab[0];//abscisse de la force sur la poutre
    a=(d-debut);
    b=(li-a);

    Rgauche=-Ftab[1]/1000*(li-a)/li;
    Rdroite=-Ftab[1]/1000*a/li;


    ///filtre noeuds intermédiaires/appuis
    for (int i=1;i<getNbreNoeuds()+1;i++) {
        //qDebug()<<getNoeuds_Travee(n)<<"n="<<n;
        //qDebug()<<getNbreNoeuds();
        if(isAppuis(i)){
            if (i==getNoeuds_Travee(n)[0]){// si i est l'appui de gauche
                result.append(Rgauche);
            }
            else{
                if (i==getNoeuds_Travee(n)[1]){// si i est l'appui de droite
                    result.append(Rdroite);
                }
                else{result.append(0.0);}
            }
        }
    }
   // qDebug()<<"react_effort"<<result;
    return result;

}


////théorème
/// réaction dû aux moments calculé avec la méthode de Clapeyron
QList<double> Barre::getReaction_moment(QList<double> list_moment_theoreme)
{
    QList<double> ltab=getLg_Travees();
    QList<QList<double>> moment_par_travee;
    QList<QList<double>> reaction_par_travee;
    QList<double> result;
    double reaction=0;


    /// place les moments sur les appuis
    for (int i=0;i<getNbreTravees();i++) {
        //qDebug()<<getNbreTravees();
        moment_par_travee.append(QList<double>());
        moment_par_travee[i].append(list_moment_theoreme[i]);
        moment_par_travee[i].append(list_moment_theoreme[i+1]);

        ///crée les réactions dû aux moments
        reaction_par_travee.append(QList<double>());
        reaction=(moment_par_travee[i][0]-moment_par_travee[i][1])/(getLg_Travees()[i]/1000);
        if (qAbs(reaction)<0.000001){reaction=0;} //on arrondit à 0 car les doubles ont du mal
        reaction_par_travee[i].append(reaction);
        reaction_par_travee[i].append(-reaction);
    }


    result.append(reaction_par_travee[0][0]);

    for (int j=1;  j<getNbreAppuis()-1; j++) {
        result.append(reaction_par_travee[j-1][1]+reaction_par_travee[j][0]);
    }
    result.append(reaction_par_travee[getNbreTravees()-1][1]);

  //  qDebug()<<"react moment"<<result;
    return result;
}


///assemblages des réactions moments et efforts
QList<double> Barre::getReaction_appuis(QList<double> reaction_effort, QList<double> reaction_moment)
{
    QList<double> result;

    for (int i=0;i<getNbreAppuis();i++) {
        result.append(reaction_effort[i]+reaction_moment[i]);
    }
   // qDebug()<<"react_appuis"<<result;
    return result;
}


///....
QList<double> Barre::getMoment(QList<double> reaction_appuis, QList<double> effort_positioneffort)
{
    QList<QList<double>> ensemble_effort;

    QList<double> result;

    effort_positioneffort[1]=effort_positioneffort[1]/1000;
  //  qDebug()<<"moment";
    for (int i = 0; i < getNbreAppuis(); i++) {
        ensemble_effort.append(QList<double>());
        ensemble_effort[i].append(getNoeud_X(getListeAppuis()[i]));
        ensemble_effort[i].append(reaction_appuis[i]);
    }

    for (int i=0;i<getNbreAppuis();i++){
        if (effort_positioneffort[0]==getNoeud_X(getListeAppuis()[i])){
            ensemble_effort[i][1]=effort_positioneffort[1]+ensemble_effort[i][1];
        }
        else{
            if((effort_positioneffort[0]>getNoeud_X(getListeAppuis()[i]))&&(effort_positioneffort[0]<getNoeud_X(getListeAppuis()[i+1]))){
            ensemble_effort.insert(i+1,effort_positioneffort);

            }
        }
    }

  //  qDebug()<<ensemble_effort;


    double x=0;
    int n=0; //pour suivre les efforts servant au calcul : quand on passe un effort il vient s'ajouter
    double moment=0;

    while (x<=getLg()){
        double somme = 0;
        for (int j=0;j<n;j++) {
            somme=somme-ensemble_effort[j][1]*(x/1000-ensemble_effort[j][0]/1000);
        }

        if (qAbs(somme)<0.00001){
            somme=0.0; //les doubles ont du mal à tomber sur 0 avec des sommes
        }

        moment=somme;
        result.append(moment);
        x=x+100;
        if (x>ensemble_effort[n][0]){n=n+1;}
    }

    return result;
}


///...
QList<double> Barre::getTranchant(QList<double> reaction_appuis, QList<double> effort_positioneffort)
{
    QList<QList<double>> ensemble_effort;

    QList<double> result;

    effort_positioneffort[1]=effort_positioneffort[1]/1000;

    for (int i = 0; i < getNbreAppuis(); i++) {
        ensemble_effort.append(QList<double>());
        ensemble_effort[i].append(getNoeud_X(getListeAppuis()[i]));
        ensemble_effort[i].append(reaction_appuis[i]);
    }

    for (int i=0;i<getNbreAppuis();i++){
        if (effort_positioneffort[0]==getNoeud_X(getListeAppuis()[i])){
            ensemble_effort[i][1]=effort_positioneffort[1]+ensemble_effort[i][1];
        }
        else{
            if((effort_positioneffort[0]>getNoeud_X(getListeAppuis()[i]))&&(effort_positioneffort[0]<getNoeud_X(getListeAppuis()[i+1]))){
            ensemble_effort.insert(i+1,effort_positioneffort);

            }
        }
    }


    double x=0;
    int n=0; //effort servant au calcul : quand on passe un effort il vient s'ajouter
    double tranchant=ensemble_effort[0][1];

    while (x<=getLg()){
        double somme = 0;
        for (int j=0;j<n;j++) {
            somme=somme-ensemble_effort[j][1];
        }
        tranchant=somme;
        result.append(tranchant);
        x=x+100;
        if (x>ensemble_effort[n][0]){n=n+1;}
    }

   // qDebug()<<"tranchant";
    return result;
}


//// retourne le tableau des efforts dû à une force ponctuelle
/// index [0][n] : efforts tranchants
/// index [1][n] : moments
QList<QList<double>> Barre::get_Effort_Barre_Force_Ponctuelle(QList<QList<double>> tab_force,int compteur_force)
{
    QList<QList<double>> result;

    result.append(getTranchant(getReaction_appuis(getReaction_effort(tab_force,compteur_force),getReaction_moment(Maths().Matrice_Resolution_Systeme(set_Matrice_A(),set_Matrice_B(tab_force,compteur_force)))),tab_force[compteur_force]));
    result.append(getMoment(getReaction_appuis(getReaction_effort(tab_force,compteur_force),getReaction_moment(Maths().Matrice_Resolution_Systeme(set_Matrice_A(),set_Matrice_B(tab_force,compteur_force)))),tab_force[compteur_force]));

    return result;
}



///assemble les efforts présents sur toute la barre à l'aide de "get_Effort_Barre_Force_Ponctuelle"
QList<QList<double> > Barre::get_Effort_Barre()
{


    ///dim result=taille du tableau
    int dim_result=get_Effort_Barre_Force_Ponctuelle(getMcharges(),0)[0].count();


    //initialisation de la matrice result
    QList<QList<double>> result;
    result.append(QList<double>());
    result[0]=Maths().init_matrice_ligne(dim_result);
    result.append(QList<double>());
    result[1]=Maths().init_matrice_ligne(dim_result);

    QList<QList<double>> effort;


    //remplissage de la matrice result
    for (int i=0;i<getMcharges().count()-1;i++) {
       // qDebug()<<"";
      //  qDebug()<<i;
        effort=get_Effort_Barre_Force_Ponctuelle(getMcharges(),i);

        for (int j=0;j<effort[0].count();j++) {
            result[0][j]=result[0][j]+effort[0][j]; //addition des tranchants de chaque force ponctuelle créée le long de la barre
            result[1][j]=result[1][j]+effort[1][j]; //addition des moments de chaque force ponctuelle créée le long de la barre
        }

    }

    return result;
}

////calcul d'une flèche avec la méthode de la force fictive (ou théorème de castigliano)

QList<double> Barre::get_Fleche_Barre()
{
    QList<double> result;
    int dim_result=get_Effort_Barre_Force_Ponctuelle(getMcharges(),0)[0].count();



    QList<QList<double>> effort_reels=get_Effort_Barre();

    double I=getI(); //moment quadratique de la barre
    double S=5.0/6.0*getH()*getb();//section S' de la barre


    double G=ec5->getGmean(getClasseBois());

    double E=ec5->getE0mean(getClasseBois());
   /// qDebug()<<E*I<<G<<S;


    //création du tableau des tableaux de force fictives
    QList<QList<QList<double>>> tab_forces_fictives;
    for (int k=0;k<dim_result;k++) {
        tab_forces_fictives.append(QList<QList<double>>());
        double x=0;
        for (int i=0;i<dim_result;i++) {
            tab_forces_fictives[k].append(QList<double>());
            tab_forces_fictives[k][i].append(x);
            tab_forces_fictives[k][i].append(0.0);
            x=x+100;
        }
        tab_forces_fictives[k][k][1]=-1000;
    }


    //calcule une flèche
    for (int k = 0;k<dim_result-1;k++) {
        QList<QList<double>> effort_fictif;
        effort_fictif=get_Effort_Barre_Force_Ponctuelle(tab_forces_fictives[k],k);


        double fleche_moment=0;
        double fleche_tranchant=0;
        double fleche=0;
        for (int i=0;i<dim_result;i++) {
            fleche_tranchant=effort_reels[0][i]*effort_fictif[0][i]*100*qPow(10,3)/(G*S);
            fleche_moment=effort_reels[1][i]*effort_fictif[1][i]*qPow(10,9)*100/(E*I);
            fleche=fleche+fleche_moment+fleche_tranchant;
        }


        result.append(fleche);
    }

    return result;
}




QList<double> Barre::getEff_pond() const{return m_eff_pond;}





double Barre::getFlambement_Y(){setFlambement(); return flambement.at(1);}
double Barre::getFlambement_Z(){setFlambement(); return flambement.at(2);}
double Barre::getElancement_Y(){setFlambement(); return flambement.at(3);}
double Barre::getElancement_Z(){setFlambement(); return flambement.at(4);}
double Barre::getFlambement_Max(){setFlambement(); return flambement.at(0);}

double Barre::getResistanceCompressionTransversale(){return ec5->getResistanceCompressionTransversale(ec5->getfc90k(getClasseBois()),kmod,gammaM);}

double Barre::getContrainteTractionAxiale(double Effort_en_DaN){return ec5->getContrainteTractionAxiale(Effort_en_DaN,getb()*getH());}
double Barre::getResistanceTractionAxiale(){return ec5->getResistanceTractionAxiale(ec5->getft0k(getClasseBois()),kmod,gammaM,ec5->getkH(bois_Massif,getH()));}
double Barre::getTauxtravailTractionAxiale(double contrainte, double resistance){return ec5->getTauxtravail(contrainte,resistance);}

double Barre::getkH(){return ec5->getkH(bois_Massif,getH());}


void Barre::setChargements(bool ELS,int num_combi)
{
  //  qDebug()<<"Barre::setChargements";

    ///tableau charges :    
    ///  Voir AddChargement

    m_eff_pond.clear();m_coordonees.clear();
    for (int i=0;i<m_charges.count();i++) {m_eff_pond.append(0);}
   // for (int i=0;i<m_precision+1;i++) {m_eff_pond.append(0);}
    QList <QStringList> combi;

    ///remplissage des pondérations

    /// pondérations ELS
    if (ELS){combi=m_combi_ELS;}
    ///pondérations ELU
    else {combi=m_combi_ELU;
      //  qDebug()<<"Pondérations ELU"<<combi;
    }



    QString nom;
    nom.append(combi.at(num_combi).at(0));
   // qDebug()<<"Barre::setChargements nom:"<<nom;

    /// Ici donc on va pondérér les efforts suivant les combinaisons obtenues

        for(int i=0;i < m_charges.count();i++)
        {
            for(int j=5;j<12;j++)
            {
                double charge=0;
                if(combi.at(num_combi).at(0).contains("creep")){
                    double kdef=ec5->getkdef(getMateriau(),getclasseService());
                    charge=m_eff_pond.at(i)+(m_charges.at(i).at(j) * combi.at(num_combi).at(j-4).toDouble()*kdef);
                }
                else {
                    charge=m_eff_pond.at(i)+(m_charges.at(i).at(j) * combi.at(num_combi).at(j-4).toDouble());
                }

                m_eff_pond.replace(i,charge);
            }
            m_coordonees.push_back(m_charges.at(i).at(2));
        }
        if(getNbreAppuis()==2){calcul2App();}////lance le calcul sur 2 appuis
}



double Barre::getCombi_ELS(int num_Combi)
{
    double d=0;
    try
    {
        for(int i=1;i<7;i++)
        {/// !!!! a mon avis remplacer chargements par charges
          d=d +(m_combi_ELS.at(num_Combi).at(i).toDouble() * m_charges.at(i-1).at(0));
            //d=d +(QString(combi_ELS[num_Combi][i]).toDouble() * chargements.at(i-1));
          for( int j=0;j<m_charges.count();j++)
          {
          }
        }
    }
    catch(int n){qDebug()<<n;}
    return d;
}


int Barre::getNombre_combi_ELS(){return m_combi_ELS.count();}

QStringList Barre::getNoms_Combi_ELU(){
    QStringList t;
    for (int i=0;i<m_combi_ELU.count();i++) {
        t<<m_combi_ELU.at(i).at(0);
    }
    return t;
}


QString Barre::getType_combi_ELS(int num_Combi){return m_combi_ELS.at(num_Combi).at(8);}

int Barre::getNombre_combi_ELU(){return m_combi_ELU.count();}

QStringList Barre::getNoms_Combi_ELS(){

    QStringList t;
    for (int i=0;i<m_combi_ELS.count();i++) {
        t<<m_combi_ELS.at(i).at(0);
    }
    return t;
}



bool Barre::getChargement_utilise(int num_cas_charge)
{
    bool b=false;

    for(int i=0;i<m_charges.count();i++)
    {
        if(m_charges.at(i).at(4+num_cas_charge) !=0.0)//si le cas de charge est rempli, alors
        {b=true;}
    }
    return b;
}

int Barre::getNbreActionsVariable()
{

    m_action_variable.clear();

    ///retourne la valeur i dans le tableau si on a une action variable
    for(int i= 2;i<8;i++)
    {
        if(getChargement_utilise(i)){m_action_variable.push_back(i);}
    }
    return m_action_variable.count();
}




void Barre::addChargementUniforme(int cas_de_Charge, double Q, double bande_charge,int  categorie, int noeudDeb, int noeudFin)
{
    ///Q en kN/m²
    /// bande_de_charge en mm

    //////////////tableau charges/////////////////
    /// 0 VIDE
    /// 1 VIDE
    /// 2 coordonnées du noeud sur la barre
    /// 3 oui/non charge entretien NOTA: Les charges d'entretien ne sont pas prisent en compte pour le calcul des flèches
    /// 4 situation sur la barre: 0 console_G, 1 en travée, 2 en console droite
    ///
    /// 5 G  valeur du chargement
    /// 6 Q  dito
    /// 7 S  ...
    /// 8 W1
    /// 9 W2
    /// 10 W3
    /// 11 W4
    /// 12 Sacc
    ///
    /// Ainsi à chaque noeud, on se retrouve avec ce tableau de 13 colonnes

    int noeudlocalDeb,noeudlocalFin;
    if((noeudDeb ==0) && (noeudFin == 0))
    {noeudlocalDeb=m_numNoeudDeb;noeudlocalFin=m_numNoeudFin;}
    else {noeudlocalDeb=noeudDeb;noeudlocalFin=noeudFin;}



    //if(cas_de_Charge== Chargements::Q){Q_is_Entretien=true;}
    if(cas_de_Charge== NatureChargements::Q){m_catQ=Libeurocod2::CategorieFromInt(categorie);}
    if(cas_de_Charge== NatureChargements::S){m_catS=Libeurocod2::CategorieFromInt(categorie);}
    double longueur_segment=0;

   // double coord_P=getNoeud_X(1);// coordonnée x du premier noeud
    double coord_P=getNoeud_X(noeudlocalDeb);


    if((Q != 0.0))//chargement uniforme
    {
        longueur_segment=getNoeud_X(noeudlocalFin)-getNoeud_X(noeudlocalDeb);
        bande_charge=bande_charge /1000.0;
        qDebug()<<"Q"<<Q<<"kN/m² | bdc"<<bande_charge<<"m";
        double qbdc=Q*bande_charge;

        qDebug()<<"Effort Cas"<<cas_de_Charge<<":"<<qbdc<<" N/mm";
       // Q=qbdc/100.0;
        qbdc=qbdc*longueur_segment;/// en N

        double p2=m_precision*(noeudlocalFin - noeudlocalDeb);//cast int en double pour division double/double
        //qDebug()<<"p2"<<p2;
        double eff=qbdc/p2;
        int curseur=0;
        ///passage des num de noeud en index pour les tableaux
        noeudlocalDeb=noeudlocalDeb-1;
        noeudlocalFin=noeudlocalFin-1;
        for(int i=noeudlocalDeb*m_precision;i<(noeudlocalFin*m_precision)+1;i++)
        {
            QList <double> d;
            d=m_charges.at(i);

            if(categorie){d.replace(3,1);}else{d.replace(3,0);}
            if(i==noeudlocalDeb*m_precision)//chargement début de barre 1/2 charge
            {
                d.replace(4+cas_de_Charge,(m_charges.at(i).at(4+cas_de_Charge))+(eff/2.0));
            }
            else
            {
                if(i<(noeudlocalFin*m_precision))//chargement dans la barre
                {
                   coord_P=(longueur_segment/p2)*curseur;
                    d.replace(4+cas_de_Charge,(m_charges.at(i).at(4+cas_de_Charge))+eff);

                }
                else//chargement fin de barre 1/2 charge
                {
                    d.replace(4+cas_de_Charge,(m_charges.at(i).at(4+cas_de_Charge))+(eff/2.0));
                }
            }
            curseur++;
            d.replace(0,cas_de_Charge);
            d.replace(2,m_charges.at(i).at(2));
            m_charges.replace(i,d);
        }


    }



    for(int i=0;i<m_charges.count();i++)
    {
      //  qDebug()<<"|coordonné noeud"<<m_charges[i][2]<<"i"<<i<<": charge G :"<<" :"<<m_charges[i][5]<<"charge Q"<<m_charges[i][6];
    }

    getNbreActionsVariable();
}

void Barre::addChargementPonctuel(NatureChargements cas_de_Charge, double P, int numNoeud, int categorie)
{
    ///récup de l'index du noeud
    int index=0;numNoeud=numNoeud-1;index=numNoeud*m_precision;
    QList <double> ligneChargement;
    ///récup de la ligne de chargement existante
    ligneChargement=m_charges.at(index);
    ///conversion kN en N
    P=P*1000.0;
    qDebug()<<"Chargement non pondéré :"<<P<<"N au noeud "<<numNoeud;
    ///ajout de la charge
    ligneChargement.replace(4+cas_de_Charge,P+m_charges[index][4+cas_de_Charge]);
   // if(Q_type_entretien){ligneChargement.replace(3,1);}
    ///remplacement de la ligne modifiée dans le tableau
    m_charges.replace(index,ligneChargement);


    for(int i=0;i<m_charges.count();i++)
    {/*qDebug()<<"|coordonné noeud"<<m_charges[i][2]<<"i"<<i<<": charge [i][5] :"<<" :"<<m_charges[i][5]<<"charge [i][6]"<<m_charges[i][6];*/}

    getNbreActionsVariable();

}


/////////////////////COMBINAISONS ELS ET ELUm_chargeq

void Barre::setCombi_ELS()/// pondérations Aux ELS
{
  //  qDebug()<<"-------------------Barre::setCombi_ELS-----------------------------";
    m_combi_ELS.clear();
    /// 0 Nom combi
    /// 1 G
    /// 2 Q
    /// 3 S
    /// 4 W1
    /// 5 W2
    /// 6 W3
    /// 7 W4
    /// 8 type de flèche voir enum ELS


    QStringList comb_inst1,comb_inst2,comb_creep,comb_fin;
    for(int i=0;i<9;i++){comb_inst1<<"";comb_inst2<<"";comb_creep<<"";comb_fin<<"";}

    QString nomcombi_ELS_Inst,nomcombi2,nomcombidiff,nomcombi_ELS_Fin,nomcombi_ELS_Creep;
    double psi0=0;double psi2=0;
    QString cat;
    int indexVarDominante=0,indexVarAccomp=0;
    QList<QStringList> ELSINST,ELSFIN,ELSCREEP;


    ///passage de Q à W4
    for(int i=2;i<8;i++)
    {
        ///on determine l'action dominante
        /// m_action liste les actions variables
        if(m_action_variable.contains(i))
        {
            nomcombi_ELS_Inst.clear();nomcombi2.clear();nomcombidiff.clear();nomcombi_ELS_Fin.clear();nomcombi_ELS_Creep.clear();
            for(int l=2;l<8;l++){comb_inst1.replace(l,"");}
            comb_inst1.replace(i,"1");
            indexVarDominante=i;

            nomcombi_ELS_Inst.append("W inst(Q):"+m_noms_cas_Charge.at(i));///ELS INST(Q)

            ///switch permetant de déterminer psi2 sur la charge dominante dans wcreep
            Libeurocod2::CategorieCharge catcreep=Libeurocod2::CategorieCharge::Null;
            switch (i) {
            case 2:
                catcreep=m_catQ;
                break;
            case 3:
                catcreep=m_catS;
                break;
            case 4:
                catcreep=Libeurocod2::CategorieCharge::Vent;
                break;
            case 5:
                catcreep=Libeurocod2::CategorieCharge::Vent;
                break;
            case 6:
                catcreep=Libeurocod2::CategorieCharge::Vent;
                break;
            case 7:
                catcreep=Libeurocod2::CategorieCharge::Vent;
                break;
            }
            psi2=ec5->getpsi2(catcreep);

            nomcombi_ELS_Creep.append("W creep:G");
            if(psi2>0){
                nomcombi_ELS_Creep.append(" + "+QString().setNum(psi2)+m_noms_cas_Charge.at(i));
            comb_creep.replace(0,nomcombi_ELS_Creep);
            comb_creep.replace(1,"1");
            comb_creep.replace(i,QString().setNum(psi2));
            comb_creep.replace(8,QString().setNum(ELS::Creep));
            ELSCREEP.push_back(comb_creep);}

           // qDebug()<<nomcombi_ELS_Inst;
          //  qDebug()<<nomcombi_ELS_Creep;

            ///Combinaison avec seulemnt la charge dominante
            comb_inst1.replace(0,nomcombi_ELS_Inst);
            comb_inst1.replace(8,QString().setNum(ELS::Inst_Q));
            ELSINST.push_back(comb_inst1);


            ///variables d'accompagnements


                QString nomELSInstintermediaire,nomELSCreepintermediaire;
                QString nomELSInstlong;nomELSInstlong.append(nomcombi_ELS_Inst);
                QString nomELSCreeplong;nomELSCreeplong.append(nomcombi_ELS_Creep);
                for(int j=0;j<m_action_variable.count();j++)
                {
                    if(m_action_variable.at(j)!=indexVarDominante)/// il ne faut pas la combi dominante
                    {
                        indexVarAccomp=m_action_variable.at(j);
                        if(indexVarAccomp==NatureChargements::Q)
                        {
                            psi0=ec5->getpsi0(m_catQ);
                            psi2=ec5->getpsi2(m_catQ);
                        }
                        else {
                            if(indexVarAccomp==NatureChargements::S)
                            {
                               psi0=ec5->getpsi0(m_catS);
                               psi2=ec5->getpsi2(m_catS);

                               ///pour ELS inst
                               nomELSInstintermediaire.clear();
                               comb_inst1.replace(indexVarAccomp,QString().setNum(psi0));///insertion des pondérations dans la liste
                               nomELSInstintermediaire.append(nomcombi_ELS_Inst);
                               nomELSInstintermediaire.append(" + "+QString().setNum(psi0)+m_noms_cas_Charge.at(m_action_variable.at(j)));
                               comb_inst1.replace(0,nomELSInstintermediaire);///insertion du nom de la combi dans la liste
                               comb_inst1.replace(8,QString().setNum(ELS::Inst_Q));
                               ELSINST.push_back(comb_inst1);

                               ///pour wcreep
                              if(psi2 >0)
                              {
                               nomELSCreepintermediaire.clear();
                               comb_creep.replace(indexVarAccomp,QString().setNum(psi2));
                               nomELSCreepintermediaire.append(nomcombi_ELS_Creep);
                               nomELSCreepintermediaire.append(" + "+QString().setNum(psi2)+m_noms_cas_Charge.at(m_action_variable.at(j)));
                               comb_creep.replace(0,nomELSCreepintermediaire);///insertion du nom de la combi dans la liste
                               comb_creep.replace(8,QString().setNum(ELS::Creep));
                               ELSCREEP.push_back(comb_creep);
                              }

                            }
                            else if ((NatureChargements::W1<=indexVarAccomp)&&(indexVarAccomp<=NatureChargements::W4))
                            {
                                    psi0=ec5->getpsi0(Libeurocod2::CategorieCharge::Vent);
                                    psi2=ec5->getpsi2(Libeurocod2::CategorieCharge::Vent);

                                    nomELSInstintermediaire.clear();
                                    comb_inst1.replace(indexVarAccomp,QString().setNum(psi0));///insertion des pondérations dans la liste
                                    nomELSInstintermediaire.append(nomcombi_ELS_Inst);
                                    nomELSInstintermediaire.append(" + "+QString().setNum(psi0)+m_noms_cas_Charge.at(m_action_variable.at(j)));
                                    comb_inst1.replace(0,nomELSInstintermediaire);///insertion du nom de la combi dans la liste
                                    comb_inst1.replace(8,QString().setNum(ELS::Inst_Q));
                                    ELSINST.push_back(comb_inst1);

                                    ///pour wcreep
                                    /// psi 2  est toujours à  0 sur les cas de vent
                            }
                        }

                        ///action variable d'accompagnement


                        nomELSInstlong.append(" + "+QString().setNum(psi0)+m_noms_cas_Charge.at(m_action_variable.at(j)));
                        comb_inst1.replace(indexVarAccomp,QString().setNum(psi0));///insertion des pondérations dans la liste
                        comb_inst1.replace(0,nomELSInstlong);///insertion du nom de la combi dans la liste
                        comb_inst1.replace(8,QString().setNum(ELS::Inst_Q));

                        if(comb_inst1 != ELSINST.last())///supprime les combinaisons en doublons
                        {
                            ///supprime les combi avec plusieurs cas de vent
                            if(comb_inst1.at(0).count("W")<3){ELSINST.push_back(comb_inst1);}
                        }


                        ///Wcreep
                       if(psi2 >0)
                       {
                        nomELSCreeplong.append(" + "+QString().setNum(psi2)+m_noms_cas_Charge.at(m_action_variable.at(j)));
                       // qDebug()<<"psi2 >0"<<psi2<<" :"<<nomELSCreeplong;
                        comb_creep.replace(indexVarAccomp,QString().setNum(psi2));///insertion des pondérations dans la liste
                        comb_creep.replace(0,nomELSCreeplong);///insertion du nom de la combi dans la liste
                        comb_creep.replace(8,QString().setNum(ELS::Creep));

                        if(comb_creep != ELSCREEP.last())///supprime les combinaisons en doublons
                        {
                            ELSCREEP.push_back(comb_creep);
                        }

                       }



                    }
                }
        }
    }
    m_combi_ELS.clear();
    m_combi_ELS.append(ELSINST);

    ///création des flèches finales


    //création de la combi G seul
    comb_fin.clear();
    comb_fin<<"W fin : G"<<"1"<<""<<""<<""<<""<<""<<""<<QString().setNum(ELS::Fin);
    ELSFIN.append(comb_fin);
    for (int i=0;i<ELSINST.count();i++)
    {
        QString change;
        comb_fin=ELSINST.at(i);change=comb_fin.at(0);
        change.replace("W inst(Q):","W fin: G+");///changement du nom de la combi
        comb_fin.replace(0,change);
       // qDebug()<<comb_fin.at(0);
        comb_fin.replace(1,"1");///change la pondération de G à 1
        comb_fin.replace(8,QString().setNum(ELS::Fin));
        ELSFIN.append(comb_fin);
    }

    m_combi_ELS.append(ELSFIN);

    ///Ajout de la combi G seul à wcreep
    comb_creep.clear();
    comb_creep<<"W creep: G"<<"1"<<""<<""<<""<<""<<""<<""<<QString().setNum(ELS::Creep);
    ELSCREEP.prepend(comb_creep);
    ///ajout des combis wcreep aux combi ELS
    m_combi_ELS.append(ELSCREEP);

}



void Barre::setCombi_ELU()
{
    /// 0 Nom combi
    /// 1 G
    /// 2 Q
    /// 3 S
    /// 4 W1
    /// 5 W2
    /// 6 W3
    /// 7 W4

    QStringList ELU_STR_resist,ELU_STR_resist_soul,ELU_EQU_soul,ELU_Acc;
    for(int i=0;i<9;i++){ELU_STR_resist<<"";ELU_STR_resist_soul<<"";ELU_EQU_soul<<"";ELU_Acc<<"";}

    double gammaG=0;double gammaQ=1.5;double psi0=0;int action_Dominante=0;
    m_combi_ELU.clear();//m_combi_ELU.resize(0);

    //resistance structure 1.35 G seul
    gammaG=ec5->getgamma_G();

    ELU_STR_resist.replace(8,QString().setNum(0));
    ELU_STR_resist.replace(0,"ELU "+QString().setNum(gammaG)+"G");
    ELU_STR_resist.replace(1,QString().setNum(gammaG));

    m_combi_ELU.push_back(ELU_STR_resist);


    //resistance structure charges variables

    for(int k=2;k<8;k++)
    {


        if(m_action_variable.contains(k))
        {
            //détermination de l'action variable dominante

            QString nomcombi;
            action_Dominante=k;
            nomcombi=m_noms_cas_Charge.at(k);


            ELU_STR_resist.replace(0,"ELU "+QString().setNum(gammaG)+"G + "+QString().setNum(gammaQ)+nomcombi);//ex: 1.35G + 1.5Q|| 1.35G + 1.5S
            ELU_STR_resist.replace(1,QString().setNum(gammaG));
            ELU_STR_resist.replace(action_Dominante,QString().setNum(gammaQ));


            m_combi_ELU.push_back(ELU_STR_resist);

            //avec variable d'accompagnement
            ELU_STR_resist.clear();for (int i=0;i<8;i++) {ELU_STR_resist.append("0");}


            for(unsigned int i=2; i<8; i++)
            {   if(getChargement_utilise(i))
                {
                    if(i!=action_Dominante && m_action_variable.contains(i))
                    {
                        if(i==NatureChargements::Q)
                        {
                          //  qDebug()<<"i==Chargements::Q";
                            psi0=ec5->getpsi0(m_catQ);
                        }
                        else {
                           // qDebug()<<m_catS;
                            if((i==NatureChargements::S)&& (m_catS!= Libeurocod2::CategorieCharge::Null))
                            {
                               //qDebug()<<"ELU"<<"i==Chargements::S";
                               psi0=ec5->getpsi0(m_catS);
                            }
                            else if ((NatureChargements::W1<=i)&&(i<=NatureChargements::W4) ) {
                               // qDebug()<<"ELU"<<"i==Chargements::W";
                                psi0=ec5->getpsi0(Libeurocod2::CategorieCharge::Vent);
                            }
                        }
                       // if(i!=Chargements::Q){psi0=ec5->getpsi0(m_categories_actions_variables.at(i));}
                      //  else{psi0=ec5->getpsi0(getCategorie());}//psi0 surcharges

                        ELU_STR_resist.replace(0,"ELU "+QString().setNum(gammaG)+"G + "+QString().setNum(gammaQ)+nomcombi+" + "+QString().setNum(gammaQ*psi0)+m_noms_cas_Charge.at(i));//avec actions d'accompagnement
                        ELU_STR_resist.replace(1,QString().setNum(gammaG));
                        ELU_STR_resist.replace(action_Dominante,QString().setNum(gammaQ));
                        ELU_STR_resist.replace(i,QString().setNum(gammaQ * psi0));
                        m_combi_ELU.push_back(ELU_STR_resist);
                    }
                }
            }
        }
    }
}
