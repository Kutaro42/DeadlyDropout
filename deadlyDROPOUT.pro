# ═══════════════════════════════════════════════════════════════════
#  deadlydropout.pro — fichier de projet qmake (Deadly Dropout)
#  Lancer la compilation : qmake deadlydropout.pro && make
# ═══════════════════════════════════════════════════════════════════

QT       += core gui widgets
QT       += multimedia
CONFIG   += c++17
TARGET    = DeadlyDropout
TEMPLATE  = app

# Désactive les avertissements de dépreciation Qt < 6
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# ─── Sources ───────────────────────────────────────────────────────
SOURCES += \
    appcontroller.cpp \
    bossdoormodel.cpp \
    bossdoorview.cpp \
    bossmodel.cpp \
    bossview.cpp \
    bulletmodel.cpp \
    bulletview.cpp \
    floormodel.cpp \
    floorview.cpp \
    globaldoor.cpp \
    gunmodel.cpp \
    gunview.cpp \
    katana.cpp \
    levelregistry.cpp \
    main.cpp           \
    entity.cpp         \
    enemy.cpp          \
    marketcontroller.cpp \
    marketview.cpp \
    matrixextractor.cpp \
    menucontroller.cpp \
    menuview.cpp \
    musiccontroller.cpp \
    playermodel.cpp    \
    playerview.cpp     \
    projectile.cpp \
    rubblemodel.cpp \
    rubbleview.cpp \
    standarddoormodel.cpp \
    standarddoorview.cpp \
    wallmodel.cpp      \
    wallview.cpp       \
    exitdoormodel.cpp  \
    exitdoorview.cpp   \
    zombielaunchermodel.cpp \
    zombielauncherview.cpp \
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
    bossdoormodel.h \
    bossdoorview.h \
    bossmodel.h \
    bossview.h \
    bulletmodel.h \
    bulletview.h \
    config.h           \
    floormodel.h \
    floorview.h \
    globaldoor.h \
    gunmodel.h \
    gunview.h \
    katana.h \
    levelregistry.h \
    marketcontroller.h \
    marketview.h \
    matrixextractor.h \
    menucontroller.h \
    menuview.h \
    musiccontroller.h \
    projectile.h \
    rubblemodel.h \
    rubbleview.h \
    standarddoormodel.h \
    standarddoorview.h \
    vector2f.h         \
    entity.h           \
    enemy.h            \
    playermodel.h      \
    playerview.h       \
    wallmodel.h        \
    wallview.h         \
    exitdoormodel.h    \
    exitdoorview.h     \
    zombielaunchermodel.h \
    zombielauncherview.h \
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

DISTFILES += \
    arborescence.txt
