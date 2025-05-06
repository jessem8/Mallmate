QT += core gui widgets sql printsupport svg charts

CONFIG += c++17
QT += multimedia

SOURCES += \
    client.cpp \
    connection.cpp \
    dialog.cpp \
    evacuation.cpp \
    main.cpp \
    risque.cpp \
    widget.cpp

HEADERS += \
    client.h \
    connection.h \
    dialog.h \
    evacuation.h \
    risque.h \
    widget.h

FORMS += \
    dialog.ui \
    evacuation.ui \
    widget.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
