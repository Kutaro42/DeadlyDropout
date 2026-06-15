#pragma once
#include <QtTest/QtTest>
#include "zombiestdmodel.h"
#include "playermodel.h"

class TestZombieStdModel : public QObject {
    Q_OBJECT
private slots:
    void idleWhenFar() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(9999,9999));
        QCOMPARE(z.getAIState(), EnemyAIState::IDLE);
    }
    void chasingWhenClose() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(100,0));
        QCOMPARE(z.getAIState(), EnemyAIState::CHASING);
    }
    void movesTowardPlayer() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(1.0f, Vector2f(500,0));
        QVERIFY(z.getPosition().x > 0.0f);
    }
    void doesNotMoveWhenIdle() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(1.0f, Vector2f(9999,9999));
        QCOMPARE(z.getPosition().x, 0.0f);
        QCOMPARE(z.getPosition().y, 0.0f);
    }
    void attackDamagesPlayer() {
        ZombieStdModel z(Vector2f(0,0));
        PlayerModel p(Vector2f(0,0));
        int hpBefore = p.getHp();
        z.attack(&p);
        QVERIFY(p.getHp() < hpBefore);
    }
    void attackCooldown() {
        ZombieStdModel z(Vector2f(0,0));
        PlayerModel p(Vector2f(0,0));
        z.attack(&p);
        int hpAfterFirst = p.getHp();
        z.attack(&p); // cooldown pas écoulé
        QCOMPARE(p.getHp(), hpAfterFirst);
    }
    void attackNullPlayer() {
        ZombieStdModel z(Vector2f(0,0));
        // Ne doit pas crasher
        z.attack(nullptr);
    }
    void velocityZeroWhenIdle() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(9999,9999));
        QCOMPARE(z.getVelocity().length(), 0.0f);
    }
    void velocityNonZeroWhenChasing() {
        ZombieStdModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(100,0));
        QVERIFY(z.getVelocity().length() > 0.0f);
    }
    void inactiveNotUpdated() {
        ZombieStdModel z(Vector2f(0,0));
        z.setActive(false);
        // L'IA ne doit pas changer d'état
        z.update(1.0f, Vector2f(100,0));
        QCOMPARE(z.getPosition().x, 0.0f);
    }
};