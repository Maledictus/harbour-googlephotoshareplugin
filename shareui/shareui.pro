TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++0x

TARGET = $$qtLibraryTarget(googlephotoshareplugin)

target.path = /usr/lib/nemo-transferengine/plugins

QT += network

CONFIG += plugin link_pkgconfig

PKGCONFIG += nemotransferengine-qt5 mlite5 accounts-qt5

qml.path = /usr/share/nemo-transferengine/plugins
qml.files = qml/GooglePhotoShareUi.qml

INSTALLS += target qml

DISTFILES += qml/GooglePhotoShareUi.qml

HEADERS += \
    src/googlephotomediatransfer.h \
    src/googlephotoplugininfo.h \
    src/googlephototransferiface.h

SOURCES += \
    src/googlephotomediatransfer.cpp \
    src/googlephotoplugininfo.cpp \
    src/googlephototransferiface.cpp

INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/include/accounts-qt5
