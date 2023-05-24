/**
 * @file
 * @brief CustomNetworkManagerFactory class declaration
 * - gestion proxy custom
*******************************************************************/
#ifndef CUSTOMNETWORKMANAGERFACTORY_H
#define CUSTOMNETWORKMANAGERFACTORY_H
#include <QObject>
#include <QNetworkAccessManager>
//#include <QDeclarativeNetworkAccessManagerFactory> //Qt4
#include <QQmlNetworkAccessManagerFactory> //Qt5
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QSslError>
 // need to derive your class from QQmlNetworkAccessManagerFactory, Our class is derived also from QObject
 // the reason is to use signal and slot mechanism

class CustomNetworkManagerFactory:public QObject,public QQmlNetworkAccessManagerFactory
 {
   Q_OBJECT
 public:
   explicit CustomNetworkManagerFactory(QObject *parent = 0);
   virtual QNetworkAccessManager *create(QObject *parent);
   QNetworkAccessManager* m_networkManager;
 public slots:
   void onIgnoreSSLErrors(QNetworkReply* reply,QList<QSslError> error);


 };
#endif // CUSTOMNETWORKMANAGERFACTORY_H
