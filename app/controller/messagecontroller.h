/**
  @file
  @author Stefan Frings
*/

#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QtXml>

#include <log4cpp/Category.hh>
using namespace stefanfrings;

class MessageController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(MessageController)
public:

    /** Constructor */
    MessageController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    void readXml_message(HttpRequest& request, HttpResponse& response);
    void parseXml_message(const QByteArray &data, HttpResponse& response);

signals:
    void signal_display_message(QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString ,QString );
};

#endif // MESSAGECONTROLLER_H
