QT       += core gui
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = icon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    schedulewidget.cpp \
    taskmanagerwidget.cpp \
    taskmodifydialog.cpp

HEADERS += \
    login.h \
    mainwindow.h \
    registerdialog.h \
    schedulewidget.h \
    taskmanagerwidget.h \
    taskmodifydialog.h

FORMS += \
    login.ui \
    mainwindow.ui \
    registerdialog.ui \
    schedulewidget.ui \
    taskmanagerwidget.ui \
    taskmodifydialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
