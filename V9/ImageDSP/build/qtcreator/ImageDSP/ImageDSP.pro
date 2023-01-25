QT += core gui widgets

CONFIG += qt c++11


SOURCES += \
    ../../../src/ImageProcessing.cpp \
    ../../../src/main.cpp \
	../../../src/ColorSpaces.cpp \
	../../../src/ImageFIlter.cpp \
	../../../src/kMeans.cpp \
	../../../src/NoiseReduction.cpp \
	../../../src/openSIFT/ezsift.cpp \
	../../../src/openSIFT/FeatureDraw.cpp \
	../../../src/openSIFT/image_utility.cpp \
	../../../src/openSIFT/SIFTLib.cpp


HEADERS += \
    ../../../inc/ColorSpaces.h \
    ../../../inc/ImageFilter.h \
    ../../../inc/ImageProcessing.h \
	../../../inc/kMeans.h \
	../../../inc/NoiseReduction.h \
    ../../../inc/openSIFT/common.h \
    ../../../inc/openSIFT/ezsift.h \
    ../../../inc/openSIFT/FeatureDraw.h \
    ../../../inc/openSIFT/image.h \
    ../../../inc/openSIFT/image_utility.h \
    ../../../inc/openSIFT/SIFTLib.h \
    ../../../inc/openSIFT/SIFTTypes.h \
    ../../../inc/openSIFT/timer.h \
    ../../../inc/openSIFT/vvector.h	

INCLUDEPATH += ../../../lib_inc/ ../../../inc/ ../../../inc/openSIFT


unix:!macx|win32: LIBS += -L$$PWD/../../../lib/windows-mingw/ -lImageDSPLib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../lib/windows-mingw/ImageDSPLib.lib


