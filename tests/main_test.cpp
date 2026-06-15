#include <QtTest/QtTest>

#include "test_vector2f.cpp"
#include "test_entity.cpp"
#include "test_playermodel.cpp"
#include "test_katana.cpp"
#include "test_gunmodel.cpp"
#include "test_projectile.cpp"
#include "test_enemy.cpp"
#include "test_zombiestdmodel.cpp"
#include "test_zombielaunchermodel.cpp"
#include "test_bossmodel.cpp"
#include "test_doors.cpp"
#include "test_levelmodel.cpp"
#include "test_levelcontroller.cpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    int status = 0;

    auto run = [&](QObject* test) {
        status |= QTest::qExec(test, argc, argv);
        delete test;
    };

    run(new TestVector2f);
    run(new TestEntity);
    run(new TestPlayerModel);
    run(new TestKatana);
    run(new TestGunModel);
    run(new TestProjectile);
    run(new TestEnemy);
    run(new TestZombieStdModel);
    run(new TestZombieLauncherModel);
    run(new TestBossModel);
    run(new TestDoors);
    run(new TestLevelModel);
    run(new TestLevelController);

    return status;
}
