/**
  @file
  @author Stefan Frings
*/

#ifndef HorodateController_H
#define HorodateController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class HorodateController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(HorodateController)
public:

    /** Constructor */
    HorodateController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_horodate(HttpRequest& request, HttpResponse& response);
    void parseXml_horodate(const QByteArray &data, HttpResponse& response);

 signals:
    void signal_set_date_heure(QString an,QString mois,QString jour,QString heure,QString minute,QString seconde);


};

#endif // HorodateController_H
