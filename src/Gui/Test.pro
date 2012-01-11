#-------------------------------------------------
#
# Project created by QtCreator 2011-06-24T11:21:49
#
#-------------------------------------------------

QT       += core gui

TARGET = RedCloudGui_Beta
TEMPLATE = app
RC_FILE += RedCloud.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    uploadwidget.cpp \
    uploadwatcher.cpp \
    optionswidget.cpp \
    logwidget.cpp \
    s_options.cpp

HEADERS  += mainwindow.h \
    uploadwidget.h \
    uploadwatcher.h \
    optionswidget.h \
    logwidget.h \
    s_options.h

FORMS    +=

OTHER_FILES += \
    RedCloud.rc
