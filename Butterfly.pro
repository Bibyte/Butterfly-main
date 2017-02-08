#-------------------------------------------------
#
# Project created by QtCreator 2010-10-09T00:00:15
#
#-------------------------------------------------

QT       = core gui
QT       += network
QT       += webkit
QT       += webkitwidgets

CONFIG += thread
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += HAVE_QT5

TARGET = Butterfly
TEMPLATE = app

# The application version
VERSION = 5.1.0

SOURCES += main.cpp\
        dialog.cpp \
    VBSearshModule.cpp \
    VBSearshMachine.cpp \
    VBCheckUrlThread.cpp \
    VBParameters.cpp \
    VBSearshEngineParameters.cpp \
    VBFindProfile.cpp \
    VBMainParams.cpp \
    VBParamEngine.cpp \
    SearshEngineEditor.cpp \
    FindProfileEditor.cpp \
    VBProfileListWidget.cpp \
    VBOrAndWidget.cpp \
    VBBanner.cpp \
    Authentication/GAuth.cpp \
    Proxy/gproxyengine.cpp \
    Proxy/proxylist.cpp \
    Proxy/gproxyoptions.cpp \
    GResultContainer.cpp \
    GSearchParams.cpp \
    Proxy/GProxyTest.cpp \
    VBOnlineParams.cpp \
    VBPOSTSessionEngine.cpp \
    VBRUCapcha.cpp \
    VBRUCaptchaEngine.cpp

HEADERS  += dialog.h \
    VBSearshModule.h \
    VBSearshMachine.h \
    VBCheckUrlThread.h \
    VBParameters.h \
    VBSearshEngineParameters.h \
    VBFindProfile.h \
    global.h \
    VBMainParams.h \
    VBParamEngine.h \
    ../Butterfly-build-desktop/ui_FindProfileEditor.h \
    ../Butterfly-build-desktop/ui_SearshEngineEditor.h \
    SearshEngineEditor.h \
    FindProfileEditor.h \
    VBProfileListWidget.h \
    VBOrAndWidget.h \
    VBBanner.h \
    Authentication/GCrypt.h \
    Authentication/GBProtocol.h \
    Authentication/gmac.h \
    Authentication/GAuth.h \
    Proxy/gproxyengine.h \
    Proxy/proxylist.h \
    Proxy/gproxyoptions.h \
    GResultContainer.h \
    GSearchParams.h \
    Proxy/GProxyTest.h \
    VBOnlineParams.h \
    VBPOSTSessionEngine.h \
    VBRUCapcha.h \
    VBRUCaptchaEngine.h

FORMS    += dialog.ui \
    FindProfileEditor.ui \
    SearshEngineEditor.ui

RESOURCES += \
    MainResource.qrc

RC_FILE = Butterfly.rc

OTHER_FILES += \
    ToDo.txt

TRANSLATIONS   += \
    TS/ru.ts \

