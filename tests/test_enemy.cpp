#pragma once
#include <QtTest/QtTest>
#include "zombiestdmodel.h"
#include "playermodel.h"

class TestEnemy : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        ZombieStdModel z(Vector2f(0,0));
        QCOMPARE(z.getHp(),    Cfg::ZOMBIE_MAX_HP);
        QCOMPARE(z.getMaxHp(), Cfg::ZOMBIE_MAX_HP);
        QVERIFY(z.isActive());
        QCOMPARE(z.getAIState(), EnemyAIState::IDLE);
        QVERIFY(!z.getHitThisSlash());
    }
    void takeDamage() {
        ZombieStdModel z(Vector2f(0,0));
        z.takeDamage(5);
        QCOMPARE(z.getHp(), Cfg::ZOMBIE_MAX_HP - 5);
        QCOMPARE(z.getAIState(), EnemyAIState::HURT);
    }
    void takeDamageKills() {
        ZombieStdModel z(Vector2f(0,0));
        z.takeDamage(Cfg::ZOMBIE_MAX_HP);
        QCOMPARE(z.getHp(), 0);
        QCOMPARE(z.getAIState(), EnemyAIState::DEAD);
    }
    void deadIgnoresDamage() {
        ZombieStdModel z(Vector2f(0,0));
        z.takeDamage(Cfg::ZOMBIE_MAX_HP);
        z.takeDamage(999);
        QCOMPARE(z.getHp(), 0);
    }
    void hurtTimerReturnsToIdle() {
        ZombieStdModel z(Vector2f(0,0));
        z.takeDamage(1);
        z.update(Cfg::ENEMY_HURT_DUR + 0.1f, Vector2f(9999,9999));
        QCOMPARE(z.getAIState(), EnemyAIState::IDLE);
    }
    void deadTimerDeactivates() {
        ZombieStdModel z(Vector2f(0,0));
        z.takeDamage(Cfg::ZOMBIE_MAX_HP);
        z.update(Cfg::ENEMY_DEAD_DUR + 0.1f, Vector2f(0,0));
        QVERIFY(!z.isActive());
    }
    void detectsPlayer() {
        ZombieStdModel z(Vector2f(0,0));
        QVERIFY(z.detectsPlayer(Vector2f(100,0)));
        QVERIFY(!z.detectsPlayer(Vector2f(9999,0)));
    }
    void goldDrop() {
        ZombieStdModel z(Vector2f(0,0));
        QVERIFY(z.getGoldDrop() >= 0);
        QVERIFY(!z.hasDroppedGold());
        z.markGoldDropped();
        QVERIFY(z.hasDroppedGold());
    }
    void hitThisSlash() {
        ZombieStdModel z(Vector2f(0,0));
        z.setHitThisSlash(true);
        QVERIFY(z.getHitThisSlash());
        z.setHitThisSlash(false);
        QVERIFY(!z.getHitThisSlash());
    }
};