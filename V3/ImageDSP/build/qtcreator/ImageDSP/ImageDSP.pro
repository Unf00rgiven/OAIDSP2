QT += core gui widgets

CONFIG += qt c++11


SOURCES += \
    ../../../src/ColorSpaces.cpp \
    ../../../src/ImageProcessing.cpp \
    ../../../src/main.cpp \
	../../../src/ImageFilter.cpp 


HEADERS += \
    ../../../inc/ColorSpaces.h \
    ../../../inc/ImageProcessing.h \
	../../../inc/ImageFilter.h 

INCLUDEPATH += ../../../lib_inc/ ../../../inc/


unix:!macx|win32: LIBS += -L$$PWD/../../../lib/windows-mingw/ -lImageDSPLib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../lib/windows-mingw/ImageDSPLib.lib


