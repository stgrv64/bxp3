/**
  @file
  @author Stefan Frings
*/

#ifndef GfiController_H
#define GfiController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class GfiController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(GfiController)
public:

    /** Constructor */
    GfiController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_gfi(HttpRequest& request, HttpResponse& response);
    void parseXml_gfi(const QByteArray &data, HttpResponse& response);

};
#endif // GfiController_H
