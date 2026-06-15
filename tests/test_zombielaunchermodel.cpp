#pragma once
#include <QtTest/QtTest>
#include "zombielaunchermodel.h"
#include "playermodel.h"

class TestZombieLauncherModel : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        ZombieLauncherModel z(Vector2f(0,0));
        QVERIFY(z.isActive());
        QCOMPARE(z.getAIState(), EnemyAIState::IDLE);
        QVERIFY(z.getRubbles().isEmpty());
    }
    void idleWhenFar() {
        ZombieLauncherModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(9999,9999));
        QCOMPARE(z.getAIState(), EnemyAIState::IDLE);
    }
    void chasingOutsideShootRange() {
        ZombieLauncherModel z(Vector2f(0,0));
        // Entre LAUNCHER_SHOOT_R et LAUNCHER_DETECT_R
        float dist = (Cfg::LAUNCHER_SHOOT_R + Cfg::LAUNCHER_DETECT_R) / 2.0f;
        z.update(0.016f, Vector2f(dist, 0));
        QCOMPARE(z.getAIState(), EnemyAIState::CHASING);
    }
    void attackingInsideShootRange() {
        ZombieLauncherModel z(Vector2f(0,0));
        z.update(0.016f, Vector2f(Cfg::LAUNCHER_SHOOT_R / 2.0f, 0));
        QCOMPARE(z.getAIState(), EnemyAIState::ATTACKING);
    }
    void shootCreatesRubble() {
        ZombieLauncherModel z(Vector2f(0,0));
        // Forcer le tir en simulant plusieurs ticks dans la portée
        float dist = Cfg::LAUNCHER_SHOOT_R / 2.0f;
        z.update(Cfg::LAUNCHER_SHOOT_CD + 0.1f, Vector2f(dist,0));
        QVERIFY(!z.getRubbles().isEmpty());
    }
    void rubblesMove() {
        ZombieLauncherModel z(Vector2f(0,0));
        float dist = Cfg::LAUNCHER_SHOOT_R / 2.0f;
        z.update(Cfg::LAUNCHER_SHOOT_CD + 0.1f, Vector2f(dist,0));
        if (!z.getRubbles().isEmpty()) {
            float x0 = z.getRubbles()[0]->getPosition().x;
            z.update(0.1f, Vector2f(dist,0));
            float x1 = z.getRubbles()[0]->getPosition().x;
            QVERIFY(x0 != x1 || z.getRubbles()[0]->getPosition().y != 0.0f);
        }
    }
    void pruneInactiveRubbles() {
        ZombieLauncherModel z(Vector2f(0,0));
        float dist = Cfg::LAUNCHER_SHOOT_R / 2.0f;
        z.update(Cfg::LAUNCHER_SHOOT_CD + 0.1f, Vector2f(dist,0));
        if (!z.getRubbles().isEmpty()) {
            z.getRubbles()[0]->setActive(false);
            z.pruneInactiveRubbles();
            for (auto* r : z.getRubbles())
                QVERIFY(r->isActive());
        }
    }
    void attackDoesNothing() {
        ZombieLauncherModel z(Vector2f(0,0));
        PlayerModel p(Vector2f(0,0));
        int hp = p.getHp();
        z.attack(&p); // no-op
        QCOMPARE(p.getHp(), hp);
    }
};