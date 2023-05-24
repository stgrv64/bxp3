/**
  @file
  @author Stefan Frings
*/

#ifndef NomController_H
#define NomController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class NomController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(NomController)
public:

    /** Constructor */
    NomController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_nom(HttpRequest& request, HttpResponse& response);
    void parseXml_nom(const QByteArray &data, HttpResponse& response);

signals:
    void signal_set_nom(QString nom);
    void signal_X7GestConfig_set_network(QString interface,QString ip,QString netmask,QString gateway);
    void signal_X7GestConfig_setWifiConfig(QString ssid,QString psk);

};
#endif // NomController_H
