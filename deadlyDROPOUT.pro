# ═══════════════════════════════════════════════════════════════════
#  deadlydropout.pro — fichier de projet qmake (Deadly Dropout)
#  Lancer la compilation : qmake deadlydropout.pro && make
# ═══════════════════════════════════════════════════════════════════

QT       += core gui widgets
CONFIG   += c++17
TARGET    = DeadlyDropout
TEMPLATE  = app

# Désactive les avertissements de dépreciation Qt < 6
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ─── Sources ───────────────────────────────────────────────────────
SOURCES += \
    appcontroller.cpp \
    globaldoor.cpp \
    katana.cpp \
    main.cpp           \
    entity.cpp         \
    enemy.cpp          \
    menucontroller.cpp \
    menuview.cpp \
    playermodel.cpp    \
    playerview.cpp     \
    wallmodel.cpp      \
    wallview.cpp       \
    exitdoormodel.cpp  \
    exitdoorview.cpp   \
    zombiestdmodel.cpp \
    zombiestdview.cpp  \
    levelmodel.cpp     \
    levelbuilder.cpp   \
    levelcontroller.cpp \
    gamemodel.cpp      \
    gameview.cpp       \
    gamecontroller.cpp

# ─── Headers ───────────────────────────────────────────────────────
HEADERS += \
    appcontroller.h \
    config.h           \
    globaldoor.h \
    katana.h \
    menucontroller.h \
    menuview.h \
    vector2f.h         \
    entity.h           \
    enemy.h            \
    playermodel.h      \
    playerview.h       \
    wallmodel.h        \
    wallview.h         \
    exitdoormodel.h    \
    exitdoorview.h     \
    zombiestdmodel.h   \
    zombiestdview.h    \
    leveltemplate.h    \
    levelmodel.h       \
    levelbuilder.h     \
    levelcontroller.h  \
    gamemodel.h        \
    gameview.h         \
    gamecontroller.h

# ─── Ressources (sprites, fonts) ───────────────────────────────────
# Décommenter quand les assets sont prêts :
# RESOURCES += assets.qrc

# ─── Déploiement ───────────────────────────────────────────────────
qnx:          target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
