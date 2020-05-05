INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/client.h \
    $$PWD/cmd.h

SOURCES += \
    $$PWD/client.cpp \
    $$PWD/cmd.cpp

include(qtmqtt/mqtt.pri)

