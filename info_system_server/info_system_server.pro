QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
msvc{
QMAKE_CXXFLAGS +=/utf-8
QMAKE_CFLAGS += /utf-8
}
SOURCES += \
    administrator.cpp \
    dao/db_operation.cpp \
    dispatch/message_analysis.cpp \
    dispatch/message_operation.cpp \
    m_network/tserver.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    process/image_process.cpp \
    process/login_process.cpp \
    process/register_process.cpp \
    process/select_process.cpp \
    process/talk_process.cpp \
    process/update_process.cpp \
    server.cpp \
    thread_about/m_thread_pool.cpp \
    thread_about/mthread.cpp \
    ui_class/label_editable.cpp \
    ui_class/mbutton.cpp \
    ui_class/mlabel.cpp \
    ui_class/mlineedit.cpp \
    ui_class/select_label.cpp \
    ui_class/text_edit.cpp \
    ui_class/scroll_label.cpp \
    utils/message_serialization.cpp \


HEADERS += \
    administrator.h \
    dao/db_operation.h \
    dispatch/message_analysis.h \
    dispatch/message_operation.h \
    m_network/tserver.h \
    mainwindow.h \
    message.h \
    process/image_process.h \
    process/login_process.h \
    process/register_process.h \
    process/select_process.h \
    process/talk_process.h \
    process/update_process.h \
    server.h \
    thread_about/m_thread_pool.h \
    thread_about/mthread.h \
    ui_class/label_editable.h \
    ui_class/mbutton.h \
    ui_class/mlabel.h \
    ui_class/mlineedit.h \
    ui_class/select_label.h \
    ui_class/text_edit.h \
    ui_class/ui_config.h \
    ui_class/scroll_label.h \
    utils/message_serialization.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
