QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appcontroller.cpp \
    enemy.cpp \
    entity.cpp \
    exitdoormodel.cpp \
    exitdoorview.cpp \
    gamecontroller.cpp \
    gamemodel.cpp \
    gameview.cpp \
    globaldoor.cpp \
    katana.cpp \
    levelmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    menucontroller.cpp \
    menuview.cpp \
    playermodel.cpp \
    playerview.cpp \
    wallmodel.cpp \
    wallview.cpp \
    zombiestdmodel.cpp \
    zombiestdview.cpp

HEADERS += \
    appcontroller.h \
    config.h \
    enemy.h \
    entity.h \
    exitdoormodel.h \
    exitdoorview.h \
    exitdorrmodel.h \
    gamecontroller.h \
    gamemodel.h \
    gameview.h \
    globaldoor.h \
    katana.h \
    levelmodel.h \
    mainwindow.h \
    menucontroller.h \
    menuview.h \
    player.h \
    playermodel.h \
    playerview.h \
    vector2f.h \
    wallmodel.h \
    wallview.h \
    zombiestdmodel.h \
    zombiestdview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    .gitlab-ci.yml \
    README.md

RESOURCES += \
    ressources.qrc
