QT       += core gui network
QT       +=  multimedia
QT       += sql
TARGET = Mailto
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MTGlobalVar.cpp \
    MTNetwork.cpp \
    MTStorage.cpp \
    WDAddContacts.cpp \
    WDMessageHistory.cpp \
    WDSystemTrayIcon.cpp \
    WGContactItem.cpp \
    WGMessageDialogGreen.cpp \
    WGMessageDialogWhite.cpp \
    main.cpp \
    MTMessage.cpp \
    WDContacts.cpp \
    WDLogin.cpp \
    WDMessage.cpp \
    WDPreferences.cpp \
    WDRegister.cpp \
    WDUserSettings.cpp

HEADERS += \
    MTGlobalVar.h \
    MTNetwork.h \
    MTMessage.h \
    MTStorage.h \
    WDAddContacts.h \
    WDContacts.h \
    WDLogin.h \
    WDMessage.h \
    WDMessageHistory.h \
    WDPreferences.h \
    WDRegister.h \
    WDSystemTrayIcon.h \
    WDUserSettings.h \
    WGContactItem.h \
    WGMessageDialogGreen.h \
    WGMessageDialogWhite.h

FORMS += \
    WDAddContacts.ui \
    WDContacts.ui \
    WDLogin.ui \
    WDMessage.ui \
    WDMessageHistory.ui \
    WDPreferences.ui \
    WDRegister.ui \
    WDUserSettings.ui \
    WGContactItem.ui \
    WGMessageDialogGreen.ui \
    WGMessageDialogWhite.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc


ICON = mailto.icns
RC_ICONS = icon_windows.ico

DISTFILES += \
    Updatelog
