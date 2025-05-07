QT += core gui widgets charts
QT       += core gui sql
QT += core gui widgets sql charts printsupport
QT += core gui sql charts
QT       += core gui widgets charts
QT       += core gui widgets charts serialport
QT += serialport
QT += charts
SOURCES += \
    connection.cpp \
    main.cpp \
    parking.cpp

HEADERS += \
    connection.h \
    parking.h

RESOURCES +=

FORMS += \
    parking.ui
