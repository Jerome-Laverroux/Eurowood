#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T07:46:07
#
#-------------------------------------------------

QT += core gui widgets sql qml quick 3dinput
contains(ANDROID_TARGET_ARCH,arm64-v8a){QT += androidextras}




TARGET = Eurowood
TEMPLATE = app

QML_IMPORT_PATH += $$PWD \


SOURCES += main.cpp\
    configuration/config.cpp \
    libeurocod2.cpp \
    librdm.cpp \
    ass_gousset.cpp \
    dessin.cpp \
    modele.cpp

HEADERS  += \
    ass_gousset.h \
    configuration/config.h \
    dessin.h \
    libeurocod2.h \
    librdm.h \
    modele.h

FORMS    += \
    ass_gousset.ui \
    compression.ui

RESOURCES += \
    Eurowood.qrc



DISTFILES += \
    base.db \


android{
    deployment.files += base.db
    deployment.path = /assets/db
    INSTALLS += deployment
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
  #  ANDROID_PACKAGE_SOURCE_DIR = \
   #     $$PWD/android

}

#contains(ANDROID_TARGET_ARCH,arm64-v8a) {
#    ANDROID_EXTRA_LIBS = \
#        $$PWD/lib/openssl/libcrypto.so \
#        $$PWD/lib/openssl/libssl.so


#}
