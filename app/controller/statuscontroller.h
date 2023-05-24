#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller dumps the received HTTP request in the response.
*/

class StatusController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(StatusController)
public:

    /** Constructor */
    StatusController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_horodate(HttpRequest& request, HttpResponse& response);
    void parseXml_horodate(const QByteArray &data, HttpResponse& response);


    QString upTime() const;
    int average() const;
    QString freeMemory() const;
    int badgeStored() const;
    QString connect2Server() const;
    QString freeDiskMemory() const;
};

#endif // STATUSCONTROLLER_H
