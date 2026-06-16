#pragma once
#include <QtTest/QtTest>
#include "../DEADLY_DROPOUT/levelmodel.h"
#include "../DEADLY_DROPOUT/wallmodel.h"
#include "../DEADLY_DROPOUT/floormodel.h"
#include "../DEADLY_DROPOUT/zombiestdmodel.h"
#include "../DEADLY_DROPOUT/exitdoormodel.h"
#include "../DEADLY_DROPOUT/standarddoormodel.h"
#include "../DEADLY_DROPOUT/bossdoormodel.h"

class TestLevelModel : public QObject {
    Q_OBJECT
private slots:
    void initiallyEmpty() {
        LevelModel lm;
        QVERIFY(lm.getWalls().isEmpty());
        QVERIFY(lm.getEnemies().isEmpty());
        QVERIFY(lm.getStandardDoors().isEmpty());
        QVERIFY(lm.getBossDoors().isEmpty());
        QVERIFY(lm.getExitDoor() == nullptr);
    }
    void addWall() {
        LevelModel lm;
        lm.addWall(new WallModel(Vector2f(0,0), 64.0f, 64.0f));
        QCOMPARE(lm.getWalls().size(), 1);
    }
    void addFloorTile() {
        LevelModel lm;
        lm.addFloorTile(new FloorModel(Vector2f(0,0), 64.0f, 64.0f));
        QCOMPARE(lm.getFloorTiles().size(), 1);
    }
    void addEnemy() {
        LevelModel lm;
        lm.addEnemy(new ZombieStdModel(Vector2f(0,0)));
        QCOMPARE(lm.getEnemies().size(), 1);
    }
    void addStandardDoor() {
        LevelModel lm;
        lm.addStandardDoor(new StandardDoorModel(Vector2f(0,0), 64.0f, 64.0f));
        QCOMPARE(lm.getStandardDoors().size(), 1);
    }
    void addBossDoor() {
        LevelModel lm;
        lm.addBossDoor(new BossDoorModel(Vector2f(0,0), 128.0f, 64.0f));
        QCOMPARE(lm.getBossDoors().size(), 1);
    }
    void setExitDoor() {
        LevelModel lm;
        lm.setExitDoor(new ExitDoorModel(Vector2f(0,0), 128.0f, 128.0f));
        QVERIFY(lm.getExitDoor() != nullptr);
    }
    void setPlayerSpawnPos() {
        LevelModel lm;
        lm.setPlayerSpawnPos(Vector2f(100,200));
        QCOMPARE(lm.getSpawnPos().x, 100.0f);
        QCOMPARE(lm.getSpawnPos().y, 200.0f);
    }
    void setLevelId() {
        LevelModel lm;
        lm.setLevelId(3);
        QCOMPARE(lm.getLevelId(), 3);
    }
    void clearResetsAll() {
        LevelModel lm;
        lm.addWall(new WallModel(Vector2f(0,0), 64.0f, 64.0f));
        lm.addEnemy(new ZombieStdModel(Vector2f(0,0)));
        lm.setExitDoor(new ExitDoorModel(Vector2f(0,0), 64.0f, 64.0f));
        lm.clear();
        QVERIFY(lm.getWalls().isEmpty());
        QVERIFY(lm.getEnemies().isEmpty());
        QVERIFY(lm.getExitDoor() == nullptr);
    }
    void bossDefeatWhenBossInactive() {
        LevelModel lm;
        auto* boss = new bossModel(Vector2f(0,0), 1);
        lm.addEnemy(boss);
        boss->setActive(false);
        // bossDefeat émet un signal, on vérifie juste que ça ne crashe pas
        lm.bossDefeat();
    }
};
