#define DEFINE_GLOBAL
#ifdef DEFINE_GLOBAL
#define GLOBAL
#define INIT(x)=x
#else
#define GLOBAL extern
#define INIT(x)
#endif
#include "Hashes.h"
#include "Encryptions.h"
#include "Conversions.h"
#include "BruteForce.h"
#include "requestmapper.h"
#include "x7gestconfig.h"
#include "x7gestlecteur.h"
#include "httplistener.h"
#include <process.h>
#include <supervision.h>
#include <badge.h>
#include <mainwindow.h>
#include <QApplication>
#include <QRegExp>
#include <QStringList>
#include <QKeyEvent>
#include <QtCore/QCoreApplication>
//#include "customnetworkmanagerfactory.h"
#include <QCursor>
#include <QtMultimedia>
#include <QProcess>
//#include <QWSServer>
#include <QBitmap>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQuickItem>
#include <QSettings>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

using namespace stefanfrings;

/** Name of this application */

#define APPNAME "Terminal Interactif"

/** Publisher of this application */
#define ORGANISATION "INETUM"

/** The HTTP listener of the application */
HttpListener* listener;
/** The HTTP listener of the application */
HttpListener* listener1;
log4cpp::Category *mainLog;

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QGuiApplication app(argc, argv);

    QApplication time2bii(argc, argv);
    log4cpp::PropertyConfigurator::configure("/var/x7/etc/log4cpp.properties");
#if QT_VERSION >= 0x050700
    qDebug() << "Check Version Qt5: Qt5 Framework supported";
#else
    qDebug() << "Check Version Qt5: Qt5 Framework not supported";
    exit(3);
#endif

    qDebug() <<"===========      Demarrage SMARTIME       ========";
    qDebug() << "Application name: " << QCoreApplication::applicationName();
    qDebug() << "Application PID:  " << QCoreApplication::applicationPid();
    mainLog = &log4cpp::Category::getInstance("main");
    mainLog->warn("==================================================");
    mainLog->warn("===========      Demarrage SMARTIME       ========");
    mainLog->warn("==================================================");

    MainWindow window(0);

    //Change le curseur par un carré bleu
    QBitmap bitmap(2,2);
    bitmap.fill("blue");
    QCursor mycursor(bitmap);
    QApplication::changeOverrideCursor(mycursor);
    QApplication::setOverrideCursor(mycursor);


    QByteArray langue=getenv("LC_TIME");
    QTranslator* qtTranslator = new QTranslator();
    QString traduction("time2bii_");
    traduction.append(langue);
    if (!langue.contains("fr"))
    {
        if( !qtTranslator->load(traduction,"langues")){
            mainLog->debug("Traduction " + traduction.toStdString());
            qtTranslator->load("time2bii_en","langues");
        }
        time2bii.installTranslator(qtTranslator);
    }

    QUrl source("qrc:/qml/MainView.qml");
    //QQmlApplicationEngine engin;
    //engin.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    window.resize(800,480);
    window.setAttribute(Qt::WA_TranslucentBackground, true);
    window.setCentralWidget(window.container);

    // Pour la gestion d'un proxy pour la partie navigateur Web intégrée (touche de fonction étendue)
   //window.myNetworkAccessManagerFactory = new CustomNetworkManagerFactory(&time2bii);
  //  window.view->engine()->setNetworkAccessManagerFactory(window.myNetworkAccessManagerFactory);


    //Configuration l'ecran principal
    window.view->setObjectName(QString::fromUtf8("MainView"));
    window.container->setEnabled(true);
    window.view->setMaximumSize(QSize(800, 800));
    window.container->setLayoutDirection(Qt::LeftToRight);
    //window.view->setSceneRect(QRectF(0, 0, 800, 800));
    //window.view->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

    window.view->setResizeMode(QQuickView::SizeViewToRootObject);
    window.view->setCursor(mycursor);
    window.set_touchesfonction();
    window.view->setSource(source); //qt5 remplacé par engine.load
    window.engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    //QQmlComponent component(window.view->engine(), QUrl(QLatin1String("qrc:/qml/MainView.qml")));

    //Objet racine de l'interface
    QObject * rootObject = dynamic_cast<QObject*>(window.view->rootObject());

    qDebug() << "rootObject creation" << rootObject;
    //QObject * rootObject = component.create();

    //signaux pour wake_up (réveil ecran ,backlight, alim Usb,)
    QObject::connect( rootObject, SIGNAL( signal_screen_touch()), &process_thread.process, SLOT(wake_up()));
    qDebug() << "signal screen touch ";
    //signal initialisation touches fonctions
    QObject::connect( &process_thread.process, SIGNAL( signal_set_touchesfonction(void)), &window, SLOT(set_touchesfonction(void)));
    qDebug() << "signal set touches fonction ";
    //signal play audio
    QObject::connect( rootObject, SIGNAL( signal_audio_play(QString)), &window, SLOT(audio_play(QString)));
    qDebug() << "signal audio play ";
    //Initialisation du format de l ecran : Paysage ou Portrait
    window.init_orientation_ecran();

    // Configure and start the TCP listener    
    QString configFileName="/var/x7/etc/Webservices.ini";
	qDebug("config file /var/x7/etc/Webservices.ini loaded");

    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&time2bii);
    listenerSettings->beginGroup("listener");
    listener=new HttpListener(listenerSettings, new RequestMapper(&time2bii), &time2bii);

    QSettings* listenerSettings1=new QSettings(configFileName,QSettings::IniFormat,&time2bii);
    listenerSettings1->beginGroup("listener1");
    listener1=new HttpListener(listenerSettings1,new RequestMapper1(&time2bii),&time2bii);



    //Affichage de l'ecran
    //window.show(); //qt5 remplacé par engine.load

    X7GestConfig::getInstance();
    mainLog->debug("start process_thread");
    process_thread.start();    
    mainLog->debug("start supervision_thread");
    supervision_thread.start();
    mainLog->debug("start badge_thread");
    badge_thread.start();
    return time2bii.exec();
}
