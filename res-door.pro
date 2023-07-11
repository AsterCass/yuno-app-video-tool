QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17

RC_ICONS = images/snow.ico
TARGET = AsterCascVideoTool

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    function/core_function.cpp \
    function/frp_file_expose_widget.cpp \
    function/frp_service_expose_widget.cpp \
    constants/constants.cpp \
    function/to_h264_mp4_widget.cpp \
    modules/core_module.cpp \
    modules/ffmpeg_module.cpp \
    modules/frp_module.cpp \
    utils/http_service.cpp \
    main.cpp \
    admin_widget.cpp \
    input_widget.cpp \
    output_widget.cpp

HEADERS += \
    admin_widget.h \
    function/core_function.h \
    function/frp_file_expose_widget.h \
    function/frp_service_expose_widget.h \
    constants/constants.h \
    function/to_h264_mp4_widget.h \
    modules/core_module.h \
    modules/ffmpeg_module.h \
    modules/frp_module.h \
    utils/http_service.h \
    input_widget.h \
    output_widget.h

RESOURCES += \
     application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
