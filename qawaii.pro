TEMPLATE = app
QT = gui core phonon network
CONFIG += qt release warn_on
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build
RC_DIR = build
FORMS = 
HEADERS = src/qawaii.h
SOURCES = src/qawaii.cpp src/main.cpp
RESOURCES = qawaii.qrc