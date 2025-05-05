# Add all necessary Qt modules
QT += core gui widgets sql network charts serialport

# Add the icon file (ensure mallmate_logo.ico is in your project folder or resources)
RC_ICONS = mallmate_logo.ico

# Conditional widget inclusion (standard)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Configuration
CONFIG += c++17
CONFIG -= precompile_header # Keep PCH disabled

# -------------------------------------------------
# Link against libqrencode installed via MSYS2/MinGW
# -------------------------------------------------

# Adjust path if your MSYS2/MinGW installation is different
# Common path for 64-bit MinGW installed via MSYS2
MINGW_PREFIX = C:/msys64/mingw64

# Add the include directory for qrencode.h
INCLUDEPATH += $$MINGW_PREFIX/include

# Add the library path and link the library
# The -L flag specifies the library path
# The -l flag specifies the library name (without 'lib' prefix and '.a' suffix)
LIBS += -L$$MINGW_PREFIX/lib -lqrencode

# -------------------------------------------------

# Source files - Added qrcodegeneratordialog.cpp etc.
SOURCES += \
    kioskmonitordialog.cpp \
    main.cpp \
    offeritemdelegate.cpp \
    offersflashscreen.cpp \
    settingswidget.cpp \
    statswidget.cpp \
    suggestiondialog.cpp
    # offeritemwidget.cpp # Removed this - was it used? Add back if needed.

# Header files - Added qrcodegeneratordialog.h etc.
HEADERS += \
    kioskmonitordialog.h \
    offerdata.h \
    offeritemdelegate.h \
    offersflashscreen.h \
    settingswidget.h \
    statswidget.h \
    suggestiondialog.h
    # offeritemwidget.h # Removed this - was it used? Add back if needed.

# UI Form file
FORMS += \
    kioskmonitordialog.ui \
    offresflashscreen.ui

# Resource file
RESOURCES += \
    resources.qrc

# Distributable files (like stylesheets) - Added style.qss
DISTFILES += \
    style.qss

# Default rules for deployment (standard)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Define executable name and project type (standard)
TARGET = OffresFlash # Or keep as OffresFlash if you prefer
TEMPLATE = app

# Optional: Disable deprecated APIs (uncomment if needed)
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
