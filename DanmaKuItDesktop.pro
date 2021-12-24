QT       += core gui
QT       += opengl openglwidgets
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Network/NetworkAPI.cpp \
    ScreenOverlay/DanmakuTestSet.cpp \
    ScreenOverlay/DanmakuText.cpp \
    ScreenOverlay/DanmakuTextBottom.cpp \
    ScreenOverlay/DanmakuTextBottomSet.cpp \
    ScreenOverlay/DanmakuTextFloat.cpp \
    ScreenOverlay/DanmakuTextFloatSet.cpp \
    ScreenOverlay/DanmakuTextSet.cpp \
    ScreenOverlay/DanmakuTextTop.cpp \
    ScreenOverlay/DanmakuTextTopSet.cpp \
    ScreenOverlay/DanmakuWidget.cpp \
    View/DynamicVBoxLayout.cpp \
    View/KultLoginBox.cpp \
    View/KultMessageBox.cpp \
    View/MainWindow.cpp \
    View/WaveWidget.cpp \
    main.cpp \

HEADERS += \
    Network/NetworkAPI.h \
    Network/NetworkConfig.h \
    ScreenOverlay/DanmakuConstants.h \
    ScreenOverlay/DanmakuTestSet.h \
    ScreenOverlay/DanmakuText.h \
    ScreenOverlay/DanmakuTextBottom.h \
    ScreenOverlay/DanmakuTextBottomSet.h \
    ScreenOverlay/DanmakuTextFloat.h \
    ScreenOverlay/DanmakuTextFloatSet.h \
    ScreenOverlay/DanmakuTextSet.h \
    ScreenOverlay/DanmakuTextTop.h \
    ScreenOverlay/DanmakuTextTopSet.h \
    ScreenOverlay/DanmakuWidget.h \
    View/DynamicVBoxLayout.h \
    View/KultLoginBox.h \
    View/KultMessageBox.h \
    View/MainWindow.h \
    View/WaveWidget.h \

TRANSLATIONS += \
    DanmaKuItDesktop_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS   = icon.ico

RESOURCES += \
    resources.qrc

DISTFILES += \
    Assets/Shaders/plainUV.vert \
    Assets/Shaders/waveShader.frag \
    Assets/Styles/mainUI.qss
