/**
 * @file mainwindow.cpp
 * @brief MainWindow class and TouchesfonctionSqlModel class definition
*******************************************************************/

#include <mainwindow.h>
#include <process.h>
#include <supervision.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <QSqlRecord>
#include <QSqlField>

/**
 * @brief TouchesfonctionSqlModel::TouchesfonctionSqlModel
 * - constructor
*******************************************************************/
TouchesfonctionSqlModel::TouchesfonctionSqlModel(QObject *parent,QSqlDatabase config_db):QSqlQueryModel(parent)
{}

/**
 * @brief TouchesfonctionSqlModel::setQuery
 * - access database_setup.db
*******************************************************************/
void TouchesfonctionSqlModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    QSqlQueryModel::setQuery(query, db);
    generateRoleNames();
}

/**
 * @brief TouchesfonctionSqlModel::setQuery
 * - access database_setup.db
*******************************************************************/
void TouchesfonctionSqlModel::setQuery(const QSqlQuery & query)
{
    QSqlQueryModel::setQuery(query);
    generateRoleNames();
}

/**
 * @brief TouchesfonctionSqlModel::generateRoleNames
 * - access database_setup.db and create column names
*******************************************************************/
void TouchesfonctionSqlModel::generateRoleNames()
{
    m_roleNames.clear();
    for( int i = 0; i < record().count(); i ++) {
        m_roleNames.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
}

/**
 * TouchesfonctionSqlModel::data
 * - access database_setup.db, get database value
 * @param in : index, role
 * @param out : value
*******************************************************************/
QVariant TouchesfonctionSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role < Qt::UserRole) {
           value = QSqlQueryModel::data(index, role);
       }
       else {
           int columnIdx = role - Qt::UserRole - 1;
           QModelIndex modelIndex = this->index(index.row(), columnIdx);
           value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
       }
       return value;
}

/**
 * @brief MainWindow::MainWindow
 * - constructor MainWindow class
*******************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::FramelessWindowHint)
{
}

/**
 * @brief MainWindow::~MainWindow
 * - destructor MainWindow class
*******************************************************************/
MainWindow::~MainWindow()
{
}

/**
 * @brief MainWindow::Debug
 * - Debug main function
*******************************************************************/
void MainWindow::Debug(QString a)
{
     log4cpp::Category & mylogger = log4cpp::Category::getInstance("main");
    mylogger.debug(a.toStdString());

}

/**
 * @brief MainWindow::window_set_touchesfonction
 * - read database_setup.db file and create touchesfonction buttons
*******************************************************************/
void MainWindow::window_set_touchesfonction(void)
{
    Debug("MainWindow::window_set_touchesfonction");
    //FIXME factoriser l'accés à la BDD
    QSqlDatabase config_db;
    config_db = QSqlDatabase::addDatabase("QSQLITE","touchesfonction_setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");

    /* Open database_setub.db */
    if (config_db.open())
    {
        Debug("Lecture table touchesfonction");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM touchesfonction",config_db);
        if (!model.record(0).contains("tempo"))
        {
            Debug("AlterTable touches fonctions");
            model.setQuery("ALTER TABLE touchesfonction ADD tempo VARCHAR(5)",config_db);
            config_db.commit();
        }

        TouchesfonctionSqlModel *touchesfonctionSqlModel = new TouchesfonctionSqlModel(&engine,config_db);
        touchesfonctionSqlModel->setQuery("SELECT * FROM touchesfonction",config_db);
        /* creation of model used in TouchesFonction.qml*/
        engine.rootContext()->setContextProperty("touchesfonctionmodel", touchesfonctionSqlModel);
        model.setQuery("SELECT * FROM touchesfonction",config_db);
        engine.rootContext()->setContextProperty("nbtouchesfonction",model.rowCount());

        /* Close database_detup.db */
        config_db.close();
        model.clear();
        model.deleteLater();
        /*call SLOT mainview.qml reset_touche() */
        signal_reset_touche();
    }
}

/**
 * @brief MainWindow::window_set_proxy_web
 * - set proxy
********************************************************************************/
void MainWindow::window_set_proxy_web(QString proxy_id)
{
    Debug("MainWindow::window_set_proxy_web");
    QNetworkProxy proxy;
    QString name, port;
    name = proxy_id.section(':', 0, 0);
    port = proxy_id.section(':', 1, 1);
    if (name.isEmpty()) {
        proxy=QNetworkProxy::NoProxy;
        Debug("MainWindow::window_set_proxy_web no proxy");
    }
    else
    {
        proxy=QNetworkProxy::HttpProxy;
        proxy.setHostName(name);
        proxy.setPort(port.toInt());
    }
    this->myNetworkAccessManagerFactory->m_networkManager->setProxy(proxy);
    //   this->myNetworkAccessManagerFactory->m_networkManager->cache()->clear();
    QString a=QString("MainWindow::window_set_proxy_web name : %1 port :%2").arg(name).arg(port);
    Debug(a);
}

/**
 * @brief MainWindow::window_init_orientation_ecran
 * - called by signal process signal_window_init_orientation_ecran()
 * - check database_setup.db ecran value (0-horizontal; 1-vertical)
 * - call signal_ihm_orientation_ecran()
********************************************************************************/
void MainWindow::window_init_orientation_ecran()
{
    Debug("MainWindow::window_init_orientation_ecran");
    //FIXME factoriser l'accés à la BDD
    QSqlDatabase config_db;
    config_db = QSqlDatabase::addDatabase("QSQLITE","divers_setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");

    double opacity=1;
    int a=0;
    /* Open table divers */
    if (config_db.open())
    {
        Debug("Lecture table divers.");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM divers",config_db);
        if (model.rowCount())
        {
            /* lecture valeur champ ecran */
            QString b=model.record(0).value("ecran").toString();
            a=b.toInt();
            if (b.size() >1) opacity=0;
        }
        /* Close table divers */
        config_db.close();
        model.clear();
        model.deleteLater();
    }

    engine.rootContext()->setContextProperty("badge_text_opacity",opacity);
    /* get blue background image */
    QString background_url;
    if ((a %2)==0)
    {
        if (QFile("/var/x7/contents/images/background_horizontal.img").exists()) background_url="file:/var/x7/contents/images/background_horizontal.img";
        else background_url="qrc:/qml/content/background_horizontal.jpg";
    }
    else
    {
        if (QFile("/var/x7/contents/images/background_vertical.img").exists()) background_url="file:/var/x7/contents/images/background_vertical.img";
        else background_url="qrc:/qml/content/background_vertical.jpg";
    }
    engine.rootContext()->setContextProperty("background_file",background_url);
    /* call SLOT mainView.qml orientation_ecran */
    signal_ihm_orientation_ecran(a);

}

/**
 * @brief MainWindow::window_orientation_ecran
 * - called by signal mainView.qml signal_window_orientation_ecran()
 * - check database_setup.db ecran value (0-horizontal; 1-vertical)
 * - call signal_ihm_orientation_ecran()
********************************************************************************/
void MainWindow::window_orientation_ecran(void)
{
    QSqlDatabase config_db;
    int b=0;
    double opacity=1;

    Debug("MainWindow::window_orientation_ecran");
    //FIXME factoriser l'accés à la BDD
    config_db = QSqlDatabase::addDatabase("QSQLITE","auxilary_config");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");
    if (config_db.open())
    {
        Debug("Lecture/ecriture table divers");

        QSqlQueryModel model;
        model.setQuery("SELECT * FROM divers",config_db);
        if (model.rowCount())
        {
            QString a=model.record(0).value("ecran").toString();
            b=a.toInt();
            b=(++b)%4;
            /* call SLOT mainView.qml orientation_ecran */
            signal_ihm_orientation_ecran(b);
            QSqlQuery query(config_db);
            query.prepare("UPDATE divers SET ecran=:ecran");
            if (a.size() >1)
            {
                a.clear();
                a.append('0');
                opacity=0;
            }
            engine.rootContext()->setContextProperty("badge_text_opacity",opacity);
            a.append(QString("%1").arg(b));
            query.bindValue(":ecran",a);
            query.exec();
            query.finish();
            config_db.commit();
            config_db.close();
        }
        model.clear();
        model.deleteLater();
        /* get blue background image */
        QString background_url;
        if ((b %2)==0)
        {
            if (QFile("/var/x7/contents/images/background_horizontal.img").exists()) background_url="file:/var/x7/contents/images/background_horizontal.img";
            else background_url="qrc:/qml/content/background_horizontal.jpg";
        }
        else
        {
            if (QFile("/var/x7/contents/images/background_vertical.img").exists()) background_url="file:/var/x7/contents/images/background_vertical.img";
            else background_url="qrc:/qml/content/background_vertical.jpg";
        }
        engine.rootContext()->setContextProperty("background_file",background_url);
    }
}

/**
 * @brief play audio
********************************************************************************/
void MainWindow::audio_play(QString audio)
{
    Debug("MainWindow::audio_play");
    if (!audio.isEmpty())
    {
        QFile faudio(audio);
        if (faudio.exists()) {
            QString b=QString("aplay ");
            b.append(audio);
            QProcess *qProc = new QProcess(this);
            qProc->startDetached(b);
        }
    }

}

/**
 * @brief (NOT USED) restart time2bi application
********************************************************************************/
void MainWindow::restart_app(void) {
    Debug("Performing application restart...");
    //QProcess::startDetached(qApp->applicationFilePath());
    QProcess::startDetached(qApp->arguments()[0],qApp->arguments().mid(1));
    qApp->exit( MainWindow::EXIT_CODE_RESTART );
    // exit(12);
    // QProcess::startDetached(qApp->quitOnLastWindowClosed());
    // qApp->quit();
    // QProcess::startDetached(qApp->arguments()[0],qApp->arguments().mid(1)); ->failed to move cursor on screen HDMI1
    // qApp->exit(-1);
}

/**
 * @brief (NOT USED) keyPressEvent function
*******************************************************************/
void MainWindow::keyPressEvent(QKeyEvent *k)
{

}
