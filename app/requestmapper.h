/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

using namespace stefanfrings;

/**
  The request mapper dispatches incoming HTTP requests to controller classes
  depending on the requested path.
*/

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:

    /**
      Constructor.
      @param parent Parent object
    */
    RequestMapper(QObject* parent=0);

    /**
      Destructor.
    */
    ~RequestMapper();

    /**
      Dispatch incoming HTTP requests to different controllers depending on the URL.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(HttpRequest& request, HttpResponse& response);
    static log4cpp::Category * log4cpp;
};



class RequestMapper1 : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper1)
public:
    /**
      Constructor.
      @param parent Parent object
    */
    RequestMapper1(QObject* parent=0);

    /**
      Destructor.
    */
    ~RequestMapper1();
	
    /**
      Dispatch incoming HTTP requests to different controllers depending on the URL.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(HttpRequest& request, HttpResponse& response);
    static log4cpp::Category * log4cpp;
};

#endif // REQUESTMAPPER_H
