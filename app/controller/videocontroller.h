/**
  @file
  @author Stefan Frings
*/

#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QtXml>
#include <log4cpp/Category.hh>
using namespace stefanfrings;

class VideoController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(VideoController)
public:

    /** Constructor */
    VideoController();
    static log4cpp::Category * log4cpp;

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
    void readXml_video(HttpRequest& request, HttpResponse& response);
    void parseXml_video(const QByteArray &data, HttpResponse& response);

signals:
    void signal_video(QString name,QString tempo);
};

#endif // VIDEOCONTROLLER_H
