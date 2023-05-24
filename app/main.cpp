/**
 * @file main.cpp
 * @brief main file
******************************************************/
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
#include "httplistener.h"

#include "x7gestconfig.h"
#include "x7gestlecteur.h"

#include <process.h>
#include <supervision.h>
#include <badge.h>
#include <mainwindow.h>

#include <QApplication>
#include <QRegExp>
#include <QStringList>
#include <QKeyEvent>
#include <QtCore/QCoreApplication>
#include <QCursor>
#include <QtMultimedia>
#include <QProcess>
#include <QBitmap>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQuickItem>
#include <QSettings>
#include <QtWebEngine>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

using namespace stefanfrings;

/** Name of this application */

#define APPNAME "Terminal Interactif"

/** Publisher of this application */
#define ORGANISATION "INETUM"

//#define EXIT_CODE_RESTART 1000

/** The HTTP listener of the application */
HttpListener* listener;
/** The HTTP listener of the application */
HttpListener* listener1;
log4cpp::Category *mainLog;

int main(int argc, char *argv[])
{
    int currentExitCode = 0;
 //   QPointer<QApplication> time2bii;
  //  QPointer<MainWindow> window;
    do {
   // if (time2bii) delete time2bii;
   // if (window) delete window;

    QCoreApplication::setOrganizationName("INETUM");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebEngine::initialize();

    QApplication time2bii(argc, argv);
//time2bii = new QApplication(argc, argv);
    QPixmap pixmap("qrc:/qml/content/information.png");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
  //  splash.show();
    QTimer::singleShot(5000, &splash, &QWidget::close);
    time2bii.processEvents();

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
//window = new MainWindow(0);
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
    CustomNetworkManagerFactory *customNetworkManagerFactory = new CustomNetworkManagerFactory();
    customNetworkManagerFactory->create(&time2bii);
    window.myNetworkAccessManagerFactory = customNetworkManagerFactory;
    window.view->engine()->setNetworkAccessManagerFactory(customNetworkManagerFactory);


    //Configuration l'ecran principal
    window.view->setObjectName(QString::fromUtf8("MainView"));
    window.container->setEnabled(true);
    window.view->setMaximumSize(QSize(800, 800));
    window.container->setLayoutDirection(Qt::LeftToRight);
    //window.view->setSceneRect(QRectF(0, 0, 800, 800));
    //window.view->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

    window.view->setResizeMode(QQuickView::SizeViewToRootObject);
    window.view->setCursor(mycursor);
    window.window_set_touchesfonction();
    //window.view->setSource(source); //qt5 remplacé par engine.load
//qDebug() << "window.engine: " <<  window.engine;
    window.engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    //Objet racine de l'interface
    qDebug() << "main.cpp rootObject creation";
    QObject * rootObject = window.engine.rootObjects().first()->findChild<QObject*>("MainView");
    qDebug() << "main.cpp rootObject " << rootObject;

    //QObject * rootObject = component.create();
    // FIXME ne pas connecter les signaux ici mais dans l'initialisation des objets en question.    
    QObject::connect(&process_thread.process, SIGNAL( signal_supervision_init_http(void)), &supervision_thread.supervision, SLOT(supervision_init_http(void)));
    QObject::connect(&process_thread.process, SIGNAL( signal_supervision_http_status(int)), &supervision_thread.supervision, SLOT(supervision_http_status(int)));

    //Utilisation de la configuration lecteur X7 sur un changement d'option lecteur
    //QObject::connect(&process_thread.process, SIGNAL( signal_badge_init_lecteur(void)), &badge_thread.badge, SLOT(init_lecteur(void)));
    QObject::connect(&process_thread.process, SIGNAL( signal_X7GestLecteur_initLecteur(void)), X7GestLecteur::getInstance(), SLOT(initLecteur(void)));

    QObject::connect(rootObject, SIGNAL(signal_supervision_ihm_post_badge(QString,QString,QString,QString)), &supervision_thread.supervision, SLOT(ihm_post_badge(QString,QString,QString,QString)));

    //Utilisation de la lecture X7 pour les badges
    //QObject::connect( &badge_thread.badge, SIGNAL( ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(new_Badge(QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect(X7GestLecteur::getInstance(), SIGNAL( ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(new_Badge(QVariant,QVariant,QVariant,QVariant,QVariant)));
    //QObject::connect( &badge_thread.badge, SIGNAL( ihm_badge_inconnu()), rootObject, SLOT(erreur_Badge()));
    QObject::connect( X7GestLecteur::getInstance(), SIGNAL( ihm_badge_inconnu()), rootObject, SLOT(erreur_Badge()));

    QObject::connect( &badge_thread.badge, SIGNAL( ihm_morpho_status(QVariant)), rootObject, SLOT(morpho_status(QVariant))); //signal etat base lecteur morpho empreintes
    QObject::connect( &badge_thread.badge, SIGNAL( ihm_morpho_enabled()), rootObject, SLOT(morpho_enabled())); //signal présence lecteur morpho empreintes (défaut = pas présent)
    QObject::connect( &badge_thread.badge, SIGNAL( ihm_morpho_message(QVariant)), rootObject, SLOT(morpho_message(QVariant))); //message de service du lecteur vers ihm morpho
    QObject::connect( &badge_thread.badge, SIGNAL( ihm_badge_message(QVariant)), rootObject, SLOT(ihm_badge_message(QVariant))); //message "Présenter badge ou empreinte" lecteur vers ihm morpho

    //signaux pour wake_up (réveil ecran ,backlight, alim Usb,)
    qDebug() << "main.cpp init signal screen touch ";
    QObject::connect( rootObject, SIGNAL( signal_screen_touch()), &process_thread.process, SLOT(wake_up()));
   //signal clear badge depuis ihm
    QObject::connect( rootObject, SIGNAL( badge_clear()), &badge_thread.badge, SLOT(badge_clear_slot()));

    //signal changement défaut connection http pour affichage sur ihm
    QObject::connect( &supervision_thread.supervision, SIGNAL(signal_ihm_http_status(QVariant)), rootObject, SLOT(ihm_http_status(QVariant)));

    //signal changement défaut database pour affichage sur ihm
    QObject::connect( &supervision_thread.supervision, SIGNAL(signal_ihm_badge_db_status(QVariant)), rootObject, SLOT(ihm_badge_db_status(QVariant)));

    //signal insertion/ extraction clé usb pour affichage sur ihm
    QObject::connect( &process_thread.process, SIGNAL(signal_ihm_usbkey_status(QVariant)), rootObject, SLOT(usbkey_status(QVariant)));

    //signal initialisation mode ethernet  lancé par viewip
    QObject::connect( rootObject, SIGNAL(signal_x7GestConfig_set_mode_network(int,QString,QString,QString,QString,QString,QString)), X7GestConfig::getInstance(), SLOT(x7GestConfig_set_mode_network(int,QString,QString,QString,QString,QString,QString)));
    QObject::connect(  X7GestConfig::getInstance(), SIGNAL(signal_ihm_init_mode_network(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_mode_network(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect(  &supervision_thread.supervision, SIGNAL(signal_x7GestConfig_init_mode_network()), X7GestConfig::getInstance(), SLOT(x7GestConfig_init_mode_network()));

    //signal initialisation lecteur badge  lancé par viewlecteur
    QObject::connect( rootObject, SIGNAL(signal_ihm_process_set_lecteur_badge(QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_lecteur_badge(QString,QString,QString,QString)));
    QObject::connect(  &process_thread.process, SIGNAL(signal_ihm_init_lecteur_badge(QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_lecteur_badge(QVariant,QVariant,QVariant,QVariant)));

    //signal initialisation   lancé par viewhttp
    QObject::connect( rootObject, SIGNAL(signal_ihm_process_set_http(QString,QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_http(QString,QString,QString,QString,QString,QString,QString)));
    QObject::connect(  &process_thread.process, SIGNAL(signal_ihm_init_http(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_http(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect(  &supervision_thread.supervision, SIGNAL(signal_process_init_http()), &process_thread.process, SLOT(process_init_http(void)));

    //signal initialisation   lancé par viewntp
    QObject::connect( rootObject, SIGNAL(signal_ihm_process_set_ntp(QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_ntp(QString,QString,QString,QString)));
    QObject::connect(  &process_thread.process, SIGNAL(signal_ihm_init_ntp(QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_ntp(QVariant,QVariant,QVariant,QVariant)));

    //signal initialisation   lancé par viewdivers
    QObject::connect( rootObject, SIGNAL(signal_ihm_process_set_divers(QString,QString,QString,QString,QString,QString)),&process_thread.process,SLOT(process_set_divers(QString,QString,QString,QString,QString,QString)));
    QObject::connect(  &process_thread.process, SIGNAL(signal_ihm_init_divers(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_divers(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));

    //signal initialisation  lancé par viewwifi
    QObject::connect( rootObject, SIGNAL(signal_X7GestConfig_setWifi(QString,QString)), X7GestConfig::getInstance(), SLOT(x7GestConfig_setWifi(QString,QString)));
    QObject::connect( X7GestConfig::getInstance(),SIGNAL(signal_ihm_init_wifi(QVariant,QVariant)), rootObject, SLOT(ihm_init_wifi(QVariant,QVariant)));

    //signal initialisation lancé par view3G
    QObject::connect( rootObject, SIGNAL(signal_process_set_3G(QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_3G(QString,QString,QString,QString,QString)));
    QObject::connect( &process_thread.process,SIGNAL(signal_ihm_init_3G(QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_init_3G(QVariant,QVariant,QVariant,QVariant,QVariant)));

    //signal initialisation lancé par viewSSH
    QObject::connect( rootObject, SIGNAL(signal_process_set_SSH(QString)), &process_thread.process, SLOT(process_set_SSH(QString)));
    QObject::connect( &process_thread.process,SIGNAL(signal_ihm_init_SSH(QVariant)), rootObject, SLOT(ihm_init_SSH(QVariant)));

    //signal initialisation lancé par viewSwUpdate
    QObject::connect( rootObject, SIGNAL(signal_process_set_SwUpdate(QString)), &process_thread.process, SLOT(process_set_SwUpdate(QString)));
    QObject::connect( &process_thread.process,SIGNAL(signal_ihm_init_SwUpdate(QVariant)), rootObject, SLOT(ihm_init_SwUpdate(QVariant)));

    //signal lancement navigateur web NOT USED
    QObject::connect( &process_thread.process,SIGNAL(signal_ihm_browser(QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(ihm_browser(QVariant,QVariant,QVariant,QVariant)));
    qDebug() << "main.cpp ini signal browser creation";
    //signal lancement video
    QObject::connect( &process_thread.process,SIGNAL(signal_video(QVariant,QVariant)), rootObject, SLOT(video(QVariant,QVariant)));

    //signal lancement affichage message
    QObject::connect( &process_thread.process,SIGNAL(signal_display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect( &supervision_thread.supervision,SIGNAL(signal_display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), rootObject, SLOT(display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    //     QObject::connect( &process_thread.process,SIGNAL(signal_display_message(QVariant,QVariant,QVariant)), rootObject, SLOT(display_message(QVariant,QVariant,QVariant)));
    //    QObject::connect( &supervision_thread.supervision,SIGNAL(signal_display_message(QVariant,QVariant,QVariant)), rootObject, SLOT(display_message(QVariant,QVariant,QVariant)));

    //signal initialisation touches fonctions
    qDebug() << "main.cpp init signal set touches fonction ";
    QObject::connect( &process_thread.process, SIGNAL( signal_window_set_touchesfonction(void)), &window, SLOT(window_set_touchesfonction(void)));
    QObject::connect( &window, SIGNAL( signal_reset_touche(void)), rootObject, SLOT(reset_touche(void)));

    //signal play audio
    qDebug() << "main.cpp init signal audio play ";
    QObject::connect( rootObject, SIGNAL( signal_audio_play(QString)), &window, SLOT(audio_play(QString)));


    //signal orientation ecran donné par thread process
    QObject::connect( rootObject, SIGNAL( signal_window_orientation_ecran(void)), &window, SLOT(window_orientation_ecran(void)));
    QObject::connect( &window, SIGNAL( signal_ihm_orientation_ecran(QVariant)), rootObject, SLOT(ihm_orientation_ecran(QVariant)));
    QObject::connect(&process_thread.process,SIGNAL( signal_window_init_orientation_ecran(QVariant)), &window, SLOT(window_init_orientation_ecran()));
    // QObject::connect(&process_thread.process,SIGNAL( signal_orientation_ecran(QVariant)), rootObject, SLOT(orientation_ecran(QVariant))); //obsolete

    //signaux mise en place proxies
    QObject::connect( rootObject, SIGNAL( signal_window_set_proxy_web(QString)), &window, SLOT(window_set_proxy_web(QString)));

    //signaux messages de services (restart, reboot ....)
    QObject::connect( rootObject, SIGNAL( qml_message(QString)), &process_thread.process,  SLOT(qml_message(QString)));
    QObject::connect( &process_thread.process, SIGNAL( signal_window_restart_app(void)), &window, SLOT(restart_app(void)));
    //QObject::connect( qApp, &QApplication::aboutToQuit, &window, &MainWindow::close);

    //signal mise à l'heure
    QObject::connect( rootObject, SIGNAL( signal_qml_set_date_heure(QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(priv_set_date_heure(QString,QString,QString,QString,QString,QString)));

    //signal lancement initialisation touchscreen calibration
    QObject::connect( rootObject, SIGNAL( signal_qml_ts_calibrate_init()), &process_thread.process, SLOT(priv_ts_calibrate_init()));

    //signal lancement touchscreen calibration
    QObject::connect( rootObject, SIGNAL( signal_qml_ts_calibrate()), &process_thread.process, SLOT(priv_ts_calibrate()));

    //signal lancement Test hard time2bii
    QObject::connect( rootObject, SIGNAL( signal_time2bii_tests(int)), &process_thread.process, SLOT(time2bii_tests(int)));
    //signal retour tests hard time2bii
    QObject::connect( &process_thread.process, SIGNAL(signal_ihm_retour_test(QVariant,QVariant)), rootObject, SLOT(retour_test(QVariant,QVariant)));

    //signal lancement configuration volume audio
    QObject::connect( rootObject, SIGNAL( audio_set_volume(QString)), X7GestConfig::getInstance(), SLOT(setAudioVolume(QString)));

    //signal passage en sleep mode emit par process  pour badge NOT USED
    //QObject::connect( &process_thread.process, SIGNAL( signal_badge_sleep_mode()), &badge_thread.badge, SLOT(sleep_mode_slot()));

    //signal clear badge depuis ihm
    QObject::connect( rootObject, SIGNAL( badge_clear()), &badge_thread.badge, SLOT(badge_clear_slot()));

    // SIGNAUX RELATIFS à  LECTEUR EMPREINTES
    //signal effacement database lecteur
    QObject::connect( rootObject, SIGNAL(signal_morpho_deletealluser()), &badge_thread.badge, SLOT(morpho_deletealluser()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_enrolluser(QString)), &badge_thread.badge, SLOT(morpho_enrolluser(QString)));
    QObject::connect( rootObject, SIGNAL(signal_morpho_deleteuserindex(QString)), &badge_thread.badge, SLOT(morpho_deleteuserindex(QString)));
    QObject::connect( rootObject, SIGNAL(signal_morpho_cancel()), &badge_thread.badge, SLOT(morpho_cancel()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_destroy_database()), &badge_thread.badge, SLOT(morpho_destroy_database()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_create_database()), &badge_thread.badge, SLOT(morpho_create_database()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_getbaseconfig()), &badge_thread.badge, SLOT(morpho_getbaseconfig()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_reload_database()), &badge_thread.badge, SLOT(morpho_reload_database()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_start()), &badge_thread.badge, SLOT(morpho_start()));
    QObject::connect( rootObject, SIGNAL(signal_morpho_stop()), &badge_thread.badge, SLOT(morpho_stop()));

    //Initialisation du format de l ecran : Paysage ou Portrait
    window.window_init_orientation_ecran();

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
  //  splash.finish(&window);
    return time2bii.exec();
    currentExitCode = time2bii.exec();
    } while( currentExitCode == MainWindow::EXIT_CODE_RESTART );
    return currentExitCode;
}
