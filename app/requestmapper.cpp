/**
  @file
  @author Stefan Frings
*/

#include <QDateTime>
#include "requestmapper.h"
#include "controller/horodatecontroller.h"
#include "controller/setupcontroller.h"
#include "controller/formcontroller.h"
#include "controller/nomcontroller.h"
#include "controller/fileuploadcontroller.h"
#include "controller/gficontroller.h"
#include "controller/browsercontroller.h"
#include "controller/badgeagecontroller.h"
#include "controller/messagecontroller.h"
#include "controller/videocontroller.h"
#include "controller/statuscontroller.h"

log4cpp::Category * RequestMapper::log4cpp = &log4cpp::Category::getInstance("RequestMapper");
log4cpp::Category * RequestMapper1::log4cpp = &log4cpp::Category::getInstance("RequestMapper1");

RequestMapper::RequestMapper(QObject* parent) :HttpRequestHandler(parent) {
    log4cpp->debug("constructeur");
}
RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}
void RequestMapper::service(HttpRequest& request, HttpResponse& response) {
    log4cpp->debug("service");
    QByteArray path=request.getPath();
    log4cpp->debug("path="+QString(path.data()).toStdString());

    if (path.startsWith("/setup")) {
        FormController().service(request, response);
    }
    log4cpp->debug("finished request");
}

RequestMapper1::RequestMapper1(QObject* parent) :HttpRequestHandler(parent) {
    log4cpp->debug("constructeur");
}
RequestMapper1::~RequestMapper1()
{
    qDebug("RequestMapper1: deleted");
}
void RequestMapper1::service(HttpRequest& request, HttpResponse& response) {
    QByteArray path=request.getPath();
    log4cpp->debug("*********************************************");
    log4cpp->debug("Path : " + QString(path).toStdString());
    log4cpp->debug("*********************************************");

    if (path.startsWith("/horodate")) {
        HorodateController().service(request, response);
    }
    else if (path.startsWith("/setup")) {
        SetupController().service(request, response);
    }
    else if (path.startsWith("/hostname")) {
        NomController().service(request, response);
    }
    else if (path.startsWith("/fichier")) {
        FileUploadController().service(request, response);
    }
    else if (path.startsWith("/browser")) {
        BrowserController().service(request, response);
    }
    else if (path.startsWith("/message")) {
        MessageController().service(request, response);
    }
    else if (path.startsWith("/badgeage")) {
        BadgeageController().service(request, response);
    }
    else if (path.startsWith("/gfi")) {
        GfiController().service(request, response);
    }
    else if (path.startsWith("/video")) {
        VideoController().service(request, response);
    }
    else if (path.startsWith("/status")) {
        StatusController().service(request, response);
    }
    log4cpp->debug("finished request");
}
