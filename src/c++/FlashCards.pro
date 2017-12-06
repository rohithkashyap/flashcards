#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T12:00:41
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = FlashCards
TEMPLATE = app
CONFIG += c++11

#RC_FILE += FlashCards.rc

SOURCES += main.cpp\
    tcFlashCards.cpp \
    tcAddCardWidget.cpp \
    tcAddSectionWidget.cpp \
    tcSectionDataWidget.cpp

HEADERS  += tcFlashCards.h \
    tcAddCardWidget.h \
    tcAddSectionWidget.h \
    tcSectionDataWidget.h

FORMS    += tcFlashCards.ui \
    tcAddCardWidget.ui \
    tcAddSectionWidget.ui \
    tcSectionDataWidget.ui


DESTDIR     = ../../bin
MOC_DIR     = ../../bld/FlashCards
OBJECTS_DIR = ../../bld/FlashCards
