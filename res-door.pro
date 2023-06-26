QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17

RC_ICONS = images/snow.ico
TARGET = AsterCascVideoTool

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    http_service.cpp \
    main.cpp \
    constants.cpp \
    admin_widget.cpp \
    input_widget.cpp \
    output_widget.cpp \
    frp_file_expose_widget.cpp

HEADERS += \
    constants.h \
    admin_widget.h \
    http_service.h \
    input_widget.h \
    output_widget.h \
    frp_file_expose_widget.h 

RESOURCES += \
     application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
