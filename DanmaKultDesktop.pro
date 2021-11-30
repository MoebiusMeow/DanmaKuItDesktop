QT       += core gui
QT       += opengl openglwidgets
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DynamicVBoxLayout.cpp \
    KultLoginBox.cpp \
    KultMessageBox.cpp \
    ScreenOverlay/danmakutestset.cpp \
    ScreenOverlay/danmakutext.cpp \
    ScreenOverlay/danmakutextbottom.cpp \
    ScreenOverlay/danmakutextbottomset.cpp \
    ScreenOverlay/danmakutextfloat.cpp \
    ScreenOverlay/danmakutextfloatset.cpp \
    ScreenOverlay/danmakutextset.cpp \
    ScreenOverlay/danmakutexttop.cpp \
    ScreenOverlay/danmakutexttopset.cpp \
    ScreenOverlay/danmakuwidget.cpp \
    WaveWidget.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    DynamicVBoxLayout.h \
    KultLoginBox.h \
    KultMessageBox.h \
    MainWindow.h \
    ScreenOverlay/DanmakuConstants.h \
    ScreenOverlay/danmakutestset.h \
    ScreenOverlay/danmakutext.h \
    ScreenOverlay/danmakutextbottom.h \
    ScreenOverlay/danmakutextbottomset.h \
    ScreenOverlay/danmakutextfloat.h \
    ScreenOverlay/danmakutextfloatset.h \
    ScreenOverlay/danmakutextset.h \
    ScreenOverlay/danmakutexttop.h \
    ScreenOverlay/danmakutexttopset.h \
    ScreenOverlay/danmakuwidget.h \
    WaveWidget.h

TRANSLATIONS += \
    DanmaKultDesktop_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    Assets/Shaders/plainUV.vert \
    Assets/Shaders/waveShader.frag \
    Assets/Styles/mainUI.qss
