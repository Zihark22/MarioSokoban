QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    jeuMario.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    jeuMario.h \
    mainwindow.h

TRANSLATIONS += \
    MarioApp_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

#Include librairie SDL
    LIBS += -L /usr/local/Cellar/sdl2/2.0.14_1/lib -lSDL2
    INCLUDEPATH += /usr/local/Cellar/sdl2/2.0.14_1/include/SDL2
#Include librairie SDL_image
    LIBS += -L /usr/local/Cellar/sdl2_image/2.0.5/lib/libSDL2_image-2.0.0.dylib
    INCLUDEPATH += /usr/local/Cellar/sdl2_image/2.0.5/include/SDL2
#Include librairie SDL_mixer
    LIBS += -L /usr/local/Cellar/sdl2_mixer/2.0.4_1/lib/libSDL2_mixer-2.0.0.dylib
    INCLUDEPATH += /usr/local/Cellar/sdl2_mixer/2.0.4_1/include/SDL2
#Include librairie SDL_ttf
    LIBS += -L /usr/local/Cellar/sdl2_ttf/2.0.15/lib/libSDL2_ttf-2.0.0.dylib
    INCLUDEPATH += /usr/local/Cellar/sdl2_ttf/2.0.15/include/SDL2

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    MarioDocs/MarioAndLuigi.ttf \
    MarioDocs/SDL.dll \
    MarioDocs/SDL_image.dll \
    MarioDocs/SuperMario256.ttf \
    MarioDocs/SuperMarioBros.wav \
    MarioDocs/batmfa.ttf \
    MarioDocs/bravo.jpg \
    MarioDocs/caisse.jpg \
    MarioDocs/caisse_ok.jpg \
    MarioDocs/eat.wav \
    MarioDocs/instructions.jpg \
    MarioDocs/instructions_jeu.png \
    MarioDocs/jpeg.dll \
    MarioDocs/key.gif \
    MarioDocs/libpng13.dll \
    MarioDocs/mario_bas.gif \
    MarioDocs/mario_droite.gif \
    MarioDocs/mario_gauche.gif \
    MarioDocs/mario_haut.gif \
    MarioDocs/menu.jpg \
    MarioDocs/mur.jpg \
    MarioDocs/niveaux.lvl \
    MarioDocs/objectif.png \
    MarioDocs/supermario.gif \
    MarioDocs/titreMario.png \
    MarioDocs/zlib1.dll
