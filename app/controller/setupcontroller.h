/**
  @file
  @author Stefan Frings
*/

#ifndef SetupController_H
#define SetupController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
//#include <QTcpServer>
#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class SetupController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(SetupController)
public:

    /** Constructor */
    SetupController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_setup(HttpRequest& request, HttpResponse& response);
    void parseXml_setup(const QByteArray &data, HttpResponse& response);

signals:
    void signal_process_set_lecteur_badge(QString lecteur,QString option,QString masque,QString format);
    void signal_process_set_ntp(QString serveur,QString cadence,QString timezone,QString langue);
    void signal_process_set_divers(QString ecran,QString bl1,QString delai,QString bl2,QString delai_off,QString sleep_mode);
    void signal_process_set_http(QString serveur,QString https,QString http,QString service,QString user,QString password,QString timeout);
    void signal_set_touches_fonction(QString libelle,QString activite,QString url,QString proxy,QString tempo);

};
#endif // SetupController_H
