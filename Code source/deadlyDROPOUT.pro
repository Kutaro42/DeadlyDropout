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
    Sources/appcontroller.cpp \
    Sources/bossdoormodel.cpp \
    Sources/bossdoorview.cpp \
    Sources/bossmodel.cpp \
    Sources/bossview.cpp \
    Sources/bulletmodel.cpp \
    Sources/bulletview.cpp \
    Sources/enemy.cpp \
    Sources/entity.cpp \
    Sources/exitdoormodel.cpp \
    Sources/exitdoorview.cpp \
    Sources/floormodel.cpp \
    Sources/floorview.cpp \
    Sources/gamecontroller.cpp \
    Sources/gamemodel.cpp \
    Sources/gameview.cpp \
    Sources/globaldoor.cpp \
    Sources/gunmodel.cpp \
    Sources/gunview.cpp \
    Sources/katana.cpp \
    Sources/levelbuilder.cpp \
    Sources/levelcontroller.cpp \
    Sources/levelmodel.cpp \
    Sources/levelregistry.cpp \
    Sources/main.cpp \
    Sources/marketcontroller.cpp \
    Sources/marketview.cpp \
    Sources/matrixextractor.cpp \
    Sources/menucontroller.cpp \
    Sources/menuview.cpp \
    Sources/musiccontroller.cpp \
    Sources/playermodel.cpp \
    Sources/playerview.cpp \
    Sources/projectile.cpp \
    Sources/rubblemodel.cpp \
    Sources/rubbleview.cpp \
    Sources/standarddoormodel.cpp \
    Sources/standarddoorview.cpp \
    Sources/wallmodel.cpp \
    Sources/wallview.cpp \
    Sources/zombielaunchermodel.cpp \
    Sources/zombielauncherview.cpp \
    Sources/zombiestdmodel.cpp \
    Sources/zombiestdview.cpp \


# ─── Sources ───────────────────────────────────────────────────────
HEADERS += \
    Headers/appcontroller.h \
    Headers/bossdoormodel.h \
    Headers/bossdoorview.h \
    Headers/bossmodel.h \
    Headers/bossview.h \
    Headers/bulletmodel.h \
    Headers/bulletview.h \
    Headers/config.h \
    Headers/enemy.h \
    Headers/entity.h \
    Headers/exitdoormodel.h \
    Headers/exitdoorview.h \
    Headers/floormodel.h \
    Headers/floorview.h \
    Headers/gamecontroller.h \
    Headers/gamemodel.h \
    Headers/gameview.h \
    Headers/globaldoor.h \
    Headers/gunmodel.h \
    Headers/gunview.h \
    Headers/katana.h \
    Headers/levelbuilder.h \
    Headers/levelcontroller.h \
    Headers/levelmodel.h \
    Headers/levelregistry.h \
    Headers/leveltemplate.h \
    Headers/marketcontroller.h \
    Headers/marketview.h \
    Headers/matrixextractor.h \
    Headers/menucontroller.h \
    Headers/menuview.h \
    Headers/musiccontroller.h \
    Headers/playermodel.h \
    Headers/playerview.h \
    Headers/projectile.h \
    Headers/rubblemodel.h \
    Headers/rubbleview.h \
    Headers/standarddoormodel.h \
    Headers/standarddoorview.h \
    Headers/vector2f.h \
    Headers/wallmodel.h \
    Headers/wallview.h \
    Headers/zombielaunchermodel.h \
    Headers/zombielauncherview.h \
    Headers/zombiestdmodel.h \
    Headers/zombiestdview.h


# ─── Ressources (sprites, fonts) ───────────────────────────────────
# Décommenter quand les assets sont prêts :
# RESOURCES += assets.qrc

# ─── Déploiement ───────────────────────────────────────────────────
qnx:          target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += Headers

RESOURCES += \
    ressources.qrc

DISTFILES += \
    arborescence.txt

CONFIG += resources_big
RC_ICONS = favi.ico
