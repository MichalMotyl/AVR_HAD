QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = AvrClient
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    avrclient.cpp

HEADERS += \
    avrclient.h
