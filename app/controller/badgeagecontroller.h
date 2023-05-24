/**
  @file
  @author Stefan Frings
*/

#ifndef BadgeageController_H
#define BadgeageController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
//#include <QTcpServer>
#include <log4cpp/Category.hh>
using namespace stefanfrings;

/**
  This controller gere la database badge.
*/

class BadgeageController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(BadgeageController)
public:

    /** Constructor */
    BadgeageController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    void readXml_badgeage(HttpRequest& request, HttpResponse& response);
    void delete_badgeage(HttpRequest& request, HttpResponse& response);
//    void parseXml_badgeage(const QByteArray &data,HttpResponse& response);
    static log4cpp::Category * log4cpp;

signals:

};
#endif // BadgeageController_H
