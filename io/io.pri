INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/client.h

SOURCES += \
    $$PWD/client.cpp

include(qtmqtt/mqtt.pri)

