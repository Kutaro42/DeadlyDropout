QT += testlib core gui widgets multimedia
CONFIG += c++17 testcase
TARGET = deadly_dropout_tests

INCLUDEPATH += ../deadly-dropout

SOURCES += \
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
