QT       += core gui httpserver

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Tcads/twincatadsview.cpp \
    centralview.cpp \
    http/httpview.cpp \
    main.cpp \
    mainwindow.cpp \
    log/Log.cpp \
    log/LogView.cpp \
    support/MainWindowExt.cpp \
    support/controls/StatusIndicator.cpp \
    support/controls/WaitingSpinner.cpp \
    support/dialog/WaitDialog.cpp \
    support/style/CustomStyle.cpp \
    settings/Settings.cpp \
    settings/SettingsView.cpp \
    TcADS/twincatads.cpp

HEADERS += \
    Tcads/twincatadsview.h \
    centralview.h \
    http/httpview.h \
    mainwindow.h \
    log/Log.h \
    log/LogView.h \
    support/MainWindowExt.h \
    support/controls/StatusIndicator.h \
    support/controls/WaitingSpinner.h \
    support/dialog/WaitDialog.h \
    support/style/CustomStyle.h \
    settings/Settings.h \
    settings/SettingsView.h \
    TcADS/twincatads.h

FORMS += \
    Tcads/twincatadsview.ui \
    centralview.ui \
    http/httpview.ui \
    mainwindow.ui \
    log/LogView.ui \
    settings/SettingsView.ui \
    support/about/AboutDialog.ui

INCLUDEPATH += \
    TcADS

LIBS += \
    -L$$PWD/TcADS \
    -lTcAdsDll

RESOURCES += \
    resources/style/DarkStyle.qrc \
    resources/style/LightStyle.qrc \
    resources/resource.qrc

RC_FILE += \
    resources/resource.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
