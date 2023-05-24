/**
  @file
  @author Stefan Frings
*/

#ifndef BROWSERCONTROLLER_H
#define BROWSERCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QtXml>
#include <log4cpp/Category.hh>
using namespace stefanfrings;

class BrowserController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(BrowserController)
public:

    /** Constructor */
    BrowserController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    void readXml_browser(HttpRequest& request, HttpResponse& response);
    void parseXml_browser(const QByteArray &data, HttpResponse& response);

signals:
    void signal_process_browser(QString url,QString proxy,QString audio,QString tempo);
};

#endif // BROWSERCONTROLLER_H
