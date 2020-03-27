#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QResource>
#include <QQuickView>
#include <QQuickStyle>

#include "modele.h"
#include "libeurocod2.h"
#include "librdm.h"
#include "configuration/config.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QLocale loc(QLocale::French);
    QQuickStyle::setStyle("System");
    QIcon::setThemeName("System");

#ifdef Q_OS_ANDROID
    QStringList permissions;
    permissions<<"android.permission.WRITE_EXTERNAL_STORAGE";
    permissions<<"android.permission.READ_EXTERNAL_STORAGE";
 //   permissions<<"android.permission.MOUNT_UNMOUNT_FILESYSTEMS";
   // permissions<<"android.permission.CAMERA";
   // permissions<<"android.permission.ACCESS_FINE_LOCATION";
   // permissions<<"android.permission.CONTROL_LOCATION_UPDATES";


    for(int i=0;i<permissions.count();i++)
    {
        QString permission=permissions.at(i);
        auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
        if(resultHash[permission] == QtAndroid::PermissionResult::Denied){
            return -1;
        }
    }
#endif



    loc.setNumberOptions(loc.numberOptions());
    QLocale::setDefault(loc);
    QResource::registerResource("Eurowood.qrc");

    Q_INIT_RESOURCE(Eurowood);

    Libeurocod2 *formule=new Libeurocod2(true);
    Config *config=new Config;




    Modele *modele=new Modele(formule);
    Trois_appuis *appuis=new Trois_appuis;



    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("formule",formule);
    engine.rootContext()->setContextProperty("cfg",config);
    engine.rootContext()->setContextProperty("modele",modele);

    qmlRegisterUncreatableType<Libeurocod2>("enums.Organe",1,0,"Organe","Error: only enums");
    qmlRegisterUncreatableType<Libeurocod2>("enums.Categories",1,0,"Categorie","Error: only enums");

    qmlRegisterUncreatableType<Barre>("enums.Ch",1,0,"NatureCharge","Error: only enums");

    qmlRegisterType<Barre>("com.modele",1,0,"Barre");

    engine.load(QUrl(QStringLiteral("qrc:/accueil.qml")));


    return a.exec();
    //return -1;


}
