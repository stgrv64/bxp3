INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

#unix:qtConfig(libudev) {
 #   DEFINES += LINK_LIBUDEV
  #  INCLUDEPATH += $$QMAKE_INCDIR_LIBUDEV
  #  LIBS_PRIVATE += $$QMAKE_LIBS_LIBUDEV
#}

PUBLIC_HEADERS += \
    $$PWD/qserialportglobal.h \
    $$PWD/qserialport.h \
    $$PWD/qserialportinfo.h

PRIVATE_HEADERS += \
    $$PWD/qserialport_p.h \
    $$PWD/qserialportinfo_p.h

SOURCES += \
    $$PWD/qserialport.cpp \
    $$PWD/qserialportinfo.cpp \
    $$PWD/qserialport_unix.cpp \
    $$PWD/qserialportinfo_unix.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
