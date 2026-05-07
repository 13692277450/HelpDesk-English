QT       += core gui sql network quick charts testlib opengl webview

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#.....
SOURCES += \
    dialogdonate.cpp \
    dialoghelp.cpp \
    dialogupgrade.cpp \
    engineerwindow.cpp \
    main.cpp \
    mainwidget.cpp \
    managerwindow.cpp

HEADERS += \
    dialogdonate.h \
    dialoghelp.h \
    dialogupgrade.h \
    engineerwindow.h \
    mainwidget.h \
    managerwindow.h

FORMS += \
    dialogdonate.ui \
    dialoghelp.ui \
    dialogupgrade.ui \
    engineerwindow.ui \
    mainwidget.ui \
    managerwindow.ui

TRANSLATIONS += \
    HelpDesk3_zh_CN.ts
    HelpDesk3_zh_EN.ts
CONFIG += lrelease
CONFIG += embed_translations
#CONFIG += console
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#version
TARGET = "Super_HelpDesk_Manager"
VERSION  = 2.0
#icon
#RC_ICONS = resource/Icon/bird2.ico
#Product
QMAKE_TARGET_PRODUCT = "Super HelpDesk Manager"
#Copyright
QMAKE_TARGET_COPYRIGHT = "CopyRight @2022, Zhang Mang, PavoGroup Inc."
#Description
QMAKE_TARGET_DESCRIPTION = "Mobile: +86-13692277450  Email: 13692277450@139.com Website: www.pavogroup.top"
#Chinese
RC_LANG = 0x0004
DISTFILES +=

RESOURCES += \
    ResourceFile.qrc
DEFINES += QT_NO_WARNING_OUTPUT\
           QT_NO_DEBUG_OUTPUT
QT.testlib.CONFIG -= console
