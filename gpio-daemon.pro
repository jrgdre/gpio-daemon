TARGET = gpio-daemon

QT += dbus
QT -= gui

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

target.path = /usr/sbin/

systemd.path = /etc/systemd/system/
systemd.files = config/$${TARGET}.service
dbus.path = /etc/dbus-1/system.d/
dbus.files = config/$${TARGET}.conf

INSTALLS += target systemd dbus

message($${INSTALLS})
message($${DEFINES})

INCLUDEPATH += ./inc

SOURCES += \
    src/tohkbd.cpp \
    src/toh.cpp \
    src/tca8424.cpp \
    src/worker.cpp \
    src/tohkeyboard.cpp \
    src/uinputif.cpp \
    src/gpio.cpp \
    src/gpio-dbus.cpp

HEADERS += \
    inc/toh.h \
    inc/tca8424.h \
    inc/worker.h \
    inc/tohkeyboard.h \
    inc/uinputif.h \
    inc/gpio-dbus.h \
    inc/gpio.h

OTHER_FILES += \
    rpm/gpio-daemon.spec \
    config/gpio-daemon.service \
    config/gpio-daemon.conf
