# QT modules needed
QT       += core gui widgets printsupport charts sql  # <<< Added sql

# Define for using Qt Charts classes without namespace qualifier
DEFINES += QT_CHARTS_USE_NAMESPACE

# Project configuration
CONFIG += c++17 warn_on
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Ensure no duplicates in SOURCES or HEADERS
SOURCES += \
    databaseconnection.cpp \
    main.cpp \
    founditemsadminwidget.cpp \
    additemdialog.cpp \
    addlostitemdialog.cpp # Added ONCE

HEADERS += \
    databaseconnection.h \
    founditemsadminwidget.h \
    additemdialog.h \
    addlostitemdialog.h # Added ONCE

FORMS +=

# Resource file
RESOURCES += \
    resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Optional: Specify icon for macOS/Windows
# win32: RC_ICONS = appicon.ico
# macx: ICON = appicon.icns

