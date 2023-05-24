# Fichier app time2bii 2022

TEMPLATE = app
TARGET = time2bii
target.path = /var/x7/bin

logconfig.path = /var/x7/etc
#fichier dans le projet ne pas modifier
logconfig.files = ../log4cpp.properties


INSTALLS += target logconfig

CONFIG += gnu++11
CONFIG += link_pkgconfig
CONFIG += mobility serialport
#CONFIG += console
MOBILITY += multimedia

QT+= core-private network qml quick sql testlib multimedia xml widgets quickcontrols2 webengine

#libs only for QtSerialPort
INCLUDEPATH += $$PWD/../libs
INCLUDEPATH += $$PWD/../libs/cryptopp
INCLUDEPATH += $$PWD/../libs/morpho
INCLUDEPATH += $$PWD/../libs/log4cpp
INCLUDEPATH += ../BadgeLib
LIBS += -L../BadgeLib -lBadgeLib

LIBS += -L$$PWD/../libs/cryptopp -lcryptopp
LIBS += -lMSO100 -llog4cpp


HEADERS += process.h \
	supervision.h \
	badge.h \
        morpho.h \
	mainwindow.h \
	customnetworkmanagerfactory.h \
	requestmapper.h \
	T2bEvent.h \
	Encryptions.h \
	Conversions.h \
	BruteForce.h \
	Hashes.h \
	controller/formcontroller.h \
	controller/fileuploadcontroller.h \
	controller/horodatecontroller.h \
	controller/setupcontroller.h \
	controller/nomcontroller.h \
	controller/gficontroller.h \
	controller/browsercontroller.h \
	controller/badgeagecontroller.h \
	controller/messagecontroller.h \
	controller/videocontroller.h \
	controller/statuscontroller.h \
    x7gestconfig.h \
    x7customevent.h \
    x7gestcleusb.h \
    x7portserie.h \
    x7interfaceserie.h \
    x7gestlecteur.h \
    x7carteio.h \
    x7lecteurrf.h \
    x7interfaceutilisateur.h \
    x7log.h

SOURCES += supervision.cpp \
    process.cpp \
    badge.cpp \
    morpho.cpp\
    main.cpp \
    mainwindow.cpp \
    customnetworkmanagerfactory.cpp \
    requestmapper.cpp \
	Hashes.cpp \
    Encryptions.cpp \
    Conversions.cpp \
    BruteForce.cpp \
    controller/formcontroller.cpp \
    controller/fileuploadcontroller.cpp \
    controller/horodatecontroller.cpp \
    controller/setupcontroller.cpp \
    controller/nomcontroller.cpp \
    controller/gficontroller.cpp \
    controller/browsercontroller.cpp \
    controller/badgeagecontroller.cpp \
    controller/messagecontroller.cpp \
    controller/videocontroller.cpp \
	controller/statuscontroller.cpp \
    x7gestconfig.cpp \
    x7gestcleusb.cpp \
    x7portserie.cpp \
    x7interfaceserie.cpp \
    x7gestlecteur.cpp \
    x7carteio.cpp \
    x7lecteurrf.cpp \
    x7interfaceutilisateur.cpp \
    x7log.cpp

OTHER_FILES +=    \
	qml/TouchesFonction.qml \
    qml/ViewHorodate.qml \
    qml/ViewLaunch.qml \
    qml/Webbrowser.qml \
    qml/ViewTests.qml \
    qml/mainView.qml \
    qml/BadgeView.qml \
    qml/ViewBase.qml \
    qml/ViewIntro.qml \
    qml/ViewMorphoOutils.qml \
    qml/colibri/CLStyle.qml \
    qml/colibri/NumericKeyboard.qml \
    qml/ViewLaunchSecurit.qml \
    qml/ViewIp.qml \
    qml/ViewLecteur.qml \
    qml/ViewHttp.qml \
    qml/colibri/AlphaKeyboard.qml \
    qml/ViewSetup.qml \
    qml/ViewNtp.qml \
    qml/ViewDivers.qml \
    qml/ViewWifi.qml \
    qml/ViewMessage.qml \
    qml/View3G.qml \
    qml/ViewSSH.qml \
    qml/ViewSwUpdate.qml \
    qml/Browser.qml

    
RESOURCES += \
    qml/qml.qrc \


DEFINES += LINUX HOST MORPHO_SDK_EXT

lupdate{
SOURCES += \
    supervision.cpp \
    process.cpp \
    badge.cpp \
    morpho.cpp \
    main.cpp \
    mainwindow.cpp \
    customnetworkmanagerfactory.cpp \
    requestmapper.cpp \
    controller/formcontroller.cpp \
    controller/fileuploadcontroller.cpp \
    controller/horodatecontroller.cpp \
    controller/setupcontroller.cpp \
    controller/nomcontroller.cpp \
    controller/gficontroller.cpp \
    controller/browsercontroller.cpp \
    controller/badgeagecontroller.cpp \
    controller/messagecontroller.cpp \
    controller/videocontroller.cpp \
    qml/TouchesFonction.qml \
    qml/ViewHorodate.qml \
    qml/ViewLaunch.qml \
    qml/Browser.qml \
    qml/Webbrowser.qml \
    qml/ViewTests.qml \
    qml/mainView.qml \
    qml/BadgeView.qml \
    qml/ViewBase.qml \
    qml/ViewIntro.qml \
    qml/ViewMorphoOutils.qml \
    qml/colibri/CLStyle.qml \
    qml/colibri/NumericKeyboard.qml \
    qml/ViewLaunchSecurit.qml \
    qml/ViewIp.qml \
    qml/ViewLecteur.qml \
    qml/ViewHttp.qml \
    qml/colibri/AlphaKeyboard.qml \
    qml/ViewSetup.qml \
    qml/ViewNtp.qml \
    qml/ViewDivers.qml \
    qml/ViewWifi.qml \
    qml/ViewSSH.qml \
    qml/ViewSwUpdate.qml \
    qml/ViewMessage.qml

}

TRANSLATIONS = time2bii_en.ts

DISTFILES += \
    changelog \
    /var/x7/etc/log4cpp.properties

include(../QtWebApp/QtWebApp/httpserver/httpserver.pri)
include(../qtserialport/src/serialport/serialport-libx7.pri)
