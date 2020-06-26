QT += quick quickcontrols2

CONFIG += c++11

SOURCES += \
        myqmlapplicationengine.cpp \
        main.cpp \
        myfilewatcher.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    myqmlapplicationengine.h \
    myfilewatcher.h

DISTFILES +=

RC_ICONS = img/time.ico
