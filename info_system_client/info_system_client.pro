QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
msvc{
QMAKE_CXXFLAGS +=/utf-8
QMAKE_CFLAGS += /utf-8
}

SOURCES += \
    client.cpp \
    t_network/t_socket.cpp \
    thread_about/mthread.cpp \
    user.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    ui_class/label_editable.cpp \
    ui_class/mbutton.cpp \
    ui_class/mlabel.cpp \
    ui_class/mlineedit.cpp \
    ui_class/select_label.cpp \
    ui_class/text_edit.cpp \
    ui_class/scroll_label.cpp \
    utils/message_serialization.cpp \

HEADERS += \
    t_network/t_socket.h \
    thread_about/mthread.h \
    user.h \
    client.h \
    mainwindow.h \
    message.h \
    ui_class/label_editable.h \
    ui_class/mbutton.h \
    ui_class/mlabel.h \
    ui_class/mlineedit.h \
    ui_class/select_label.h \
    ui_class/text_edit.h \
    ui_class/ui_config.h \
    ui_class/scroll_label.h \
    utils/message_serialization.h

    user.h
FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
