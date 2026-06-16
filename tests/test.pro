QT += testlib core gui widgets multimedia
CONFIG += c++17 testcase
TARGET = deadly_dropout_tests

INCLUDEPATH += ../deadly-dropout

SOURCES += \
    ../DEADLY_DROPOUT/appcontroller.cpp \
    ../DEADLY_DROPOUT/bossdoormodel.cpp \
    ../DEADLY_DROPOUT/bossdoorview.cpp \
    ../DEADLY_DROPOUT/bossmodel.cpp \
    ../DEADLY_DROPOUT/bossview.cpp \
    ../DEADLY_DROPOUT/bulletmodel.cpp \
    ../DEADLY_DROPOUT/bulletview.cpp \
    ../DEADLY_DROPOUT/enemy.cpp \
    ../DEADLY_DROPOUT/entity.cpp \
    ../DEADLY_DROPOUT/exitdoormodel.cpp \
    ../DEADLY_DROPOUT/exitdoorview.cpp \
    ../DEADLY_DROPOUT/floormodel.cpp \
    ../DEADLY_DROPOUT/floorview.cpp \
    ../DEADLY_DROPOUT/gamecontroller.cpp \
    ../DEADLY_DROPOUT/gamemodel.cpp \
    ../DEADLY_DROPOUT/gameview.cpp \
    ../DEADLY_DROPOUT/globaldoor.cpp \
    ../DEADLY_DROPOUT/gunmodel.cpp \
    ../DEADLY_DROPOUT/gunview.cpp \
    ../DEADLY_DROPOUT/katana.cpp \
    ../DEADLY_DROPOUT/levelbuilder.cpp \
    ../DEADLY_DROPOUT/levelcontroller.cpp \
    ../DEADLY_DROPOUT/levelmodel.cpp \
    ../DEADLY_DROPOUT/levelregistry.cpp \
    ../DEADLY_DROPOUT/main.cpp \
    ../DEADLY_DROPOUT/marketcontroller.cpp \
    ../DEADLY_DROPOUT/marketview.cpp \
    ../DEADLY_DROPOUT/matrixextractor.cpp \
    ../DEADLY_DROPOUT/menucontroller.cpp \
    ../DEADLY_DROPOUT/menuview.cpp \
    ../DEADLY_DROPOUT/musiccontroller.cpp \
    ../DEADLY_DROPOUT/playermodel.cpp \
    ../DEADLY_DROPOUT/playerview.cpp \
    ../DEADLY_DROPOUT/projectile.cpp \
    ../DEADLY_DROPOUT/rubblemodel.cpp \
    ../DEADLY_DROPOUT/rubbleview.cpp \
    ../DEADLY_DROPOUT/standarddoormodel.cpp \
    ../DEADLY_DROPOUT/standarddoorview.cpp \
    ../DEADLY_DROPOUT/wallmodel.cpp \
    ../DEADLY_DROPOUT/wallview.cpp \
    ../DEADLY_DROPOUT/zombielaunchermodel.cpp \
    ../DEADLY_DROPOUT/zombielauncherview.cpp \
    ../DEADLY_DROPOUT/zombiestdmodel.cpp \
    ../DEADLY_DROPOUT/zombiestdview.cpp \
    main_test.cpp \
    test_vector2f.cpp \
    test_entity.cpp \
    test_playermodel.cpp \
    test_katana.cpp \
    test_gunmodel.cpp \
    test_projectile.cpp \
    test_enemy.cpp \
    test_zombiestdmodel.cpp \
    test_zombielaunchermodel.cpp \
    test_bossmodel.cpp \
    test_doors.cpp \
    test_levelmodel.cpp \
    test_levelcontroller.cpp \
    ../deadly-dropout/entity.cpp \
    ../deadly-dropout/playermodel.cpp \
    ../deadly-dropout/katana.cpp \
    ../deadly-dropout/gunmodel.cpp \
    ../deadly-dropout/projectile.cpp \
    ../deadly-dropout/bulletmodel.cpp \
    ../deadly-dropout/rubblemodel.cpp \
    ../deadly-dropout/enemy.cpp \
    ../deadly-dropout/zombiestdmodel.cpp \
    ../deadly-dropout/zombielaunchermodel.cpp \
    ../deadly-dropout/bossmodel.cpp \
    ../deadly-dropout/globaldoor.cpp \
    ../deadly-dropout/exitdoormodel.cpp \
    ../deadly-dropout/standarddoormodel.cpp \
    ../deadly-dropout/bossdoormodel.cpp \
    ../deadly-dropout/wallmodel.cpp \
    ../deadly-dropout/floormodel.cpp \
    ../deadly-dropout/levelmodel.cpp \
    ../deadly-dropout/levelcontroller.cpp \
    ../deadly-dropout/vector2f.cpp

SUBDIRS += \
    ../DEADLY_DROPOUT/deadlyDROPOUT.pro

RESOURCES += \
    ../DEADLY_DROPOUT/ressources.qrc

DISTFILES += \
    ../DEADLY_DROPOUT/README.md \
    ../DEADLY_DROPOUT/arborescence.txt \
    ../DEADLY_DROPOUT/favi.ico

HEADERS += \
    ../DEADLY_DROPOUT/appcontroller.h \
    ../DEADLY_DROPOUT/bossdoormodel.h \
    ../DEADLY_DROPOUT/bossdoorview.h \
    ../DEADLY_DROPOUT/bossmodel.h \
    ../DEADLY_DROPOUT/bossview.h \
    ../DEADLY_DROPOUT/bulletmodel.h \
    ../DEADLY_DROPOUT/bulletview.h \
    ../DEADLY_DROPOUT/config.h \
    ../DEADLY_DROPOUT/enemy.h \
    ../DEADLY_DROPOUT/entity.h \
    ../DEADLY_DROPOUT/exitdoormodel.h \
    ../DEADLY_DROPOUT/exitdoorview.h \
    ../DEADLY_DROPOUT/floormodel.h \
    ../DEADLY_DROPOUT/floorview.h \
    ../DEADLY_DROPOUT/gamecontroller.h \
    ../DEADLY_DROPOUT/gamemodel.h \
    ../DEADLY_DROPOUT/gameview.h \
    ../DEADLY_DROPOUT/globaldoor.h \
    ../DEADLY_DROPOUT/gunmodel.h \
    ../DEADLY_DROPOUT/gunview.h \
    ../DEADLY_DROPOUT/katana.h \
    ../DEADLY_DROPOUT/levelbuilder.h \
    ../DEADLY_DROPOUT/levelcontroller.h \
    ../DEADLY_DROPOUT/levelmodel.h \
    ../DEADLY_DROPOUT/levelregistry.h \
    ../DEADLY_DROPOUT/leveltemplate.h \
    ../DEADLY_DROPOUT/marketcontroller.h \
    ../DEADLY_DROPOUT/marketview.h \
    ../DEADLY_DROPOUT/matrixextractor.h \
    ../DEADLY_DROPOUT/menucontroller.h \
    ../DEADLY_DROPOUT/menuview.h \
    ../DEADLY_DROPOUT/musiccontroller.h \
    ../DEADLY_DROPOUT/playermodel.h \
    ../DEADLY_DROPOUT/playerview.h \
    ../DEADLY_DROPOUT/projectile.h \
    ../DEADLY_DROPOUT/rubblemodel.h \
    ../DEADLY_DROPOUT/rubbleview.h \
    ../DEADLY_DROPOUT/standarddoormodel.h \
    ../DEADLY_DROPOUT/standarddoorview.h \
    ../DEADLY_DROPOUT/vector2f.h \
    ../DEADLY_DROPOUT/wallmodel.h \
    ../DEADLY_DROPOUT/wallview.h \
    ../DEADLY_DROPOUT/zombielaunchermodel.h \
    ../DEADLY_DROPOUT/zombielauncherview.h \
    ../DEADLY_DROPOUT/zombiestdmodel.h \
    ../DEADLY_DROPOUT/zombiestdview.h
