#pragma once
#include <QtTest/QtTest>
#include "bossmodel.h"
#include "playermodel.h"

class TestBossModel : public QObject {
    Q_OBJECT
private slots:
    void initialStateP1() {
        bossModel b(Vector2f(0,0), 1);
        QCOMPARE(b.getHp(),    Cfg::BOSSP1_MAX_HP);
        QVERIFY(b.isActive());
        QVERIFY(b.getisBoss());
        QVERIFY(!b.isZoneCharging());
    }
    void initialStateP2() {
        bossModel b(Vector2f(0,0), 2);
        QCOMPARE(b.getHp(), Cfg::BOSSP2_MAX_HP);
    }
    void initialStateP3() {
        bossModel b(Vector2f(0,0), 3);
        QCOMPARE(b.getHp(), Cfg::BOSSP3_MAX_HP);
    }
    void takeDamage() {
        bossModel b(Vector2f(0,0), 1);
        b.takeDamage(50);
        QCOMPARE(b.getHp(), Cfg::BOSSP1_MAX_HP - 50);
    }
    void attackDamagesPlayer() {
        bossModel b(Vector2f(0,0), 1);
        PlayerModel p(Vector2f(0,0));
        int hp = p.getHp();
        b.attack(&p);
        QVERIFY(p.getHp() < hp);
    }
    void attackCooldown() {
        bossModel b(Vector2f(0,0), 1);
        PlayerModel p(Vector2f(0,0));
        b.attack(&p);
        int hp = p.getHp();
        b.attack(&p);
        QCOMPARE(p.getHp(), hp);
    }
    void chasingWhenClose() {
        bossModel b(Vector2f(0,0), 1);
        b.update(0.016f, Vector2f(100,0));
        QCOMPARE(b.getAIState(), EnemyAIState::CHASING);
    }
    void idleWhenFar() {
        bossModel b(Vector2f(0,0), 1);
        b.update(0.016f, Vector2f(9999,9999));
        QCOMPARE(b.getAIState(), EnemyAIState::IDLE);
    }
    void p2ZoneTriggersAfterCooldown() {
        bossModel b(Vector2f(0,0), 2);
        // Simuler suffisamment de ticks avec le joueur proche
        b.update(Cfg::BOSS_ZONE_CHARGE_CD + 0.1f, Vector2f(100,0));
        QVERIFY(b.isZoneCharging());
    }
    void p1NoZone() {
        bossModel b(Vector2f(0,0), 1);
        b.update(Cfg::BOSS_ZONE_CHARGE_CD + 0.5f, Vector2f(100,0));
        QVERIFY(!b.isZoneCharging());
    }
    void zoneExplodesAfterTimer() {
        bossModel b(Vector2f(0,0), 2);
        b.update(Cfg::BOSS_ZONE_CHARGE_CD + 0.1f, Vector2f(100,0));
        if (b.isZoneCharging()) {
            b.update(Cfg::BOSS_ZONE_TIMER + 0.1f, Vector2f(100,0));
            QVERIFY(b.zoneJustExploded());
        }
    }
    void clearZoneExploded() {
        bossModel b(Vector2f(0,0), 2);
        b.update(Cfg::BOSS_ZONE_CHARGE_CD + 0.1f, Vector2f(100,0));
        if (b.isZoneCharging()) {
            b.update(Cfg::BOSS_ZONE_TIMER + 0.1f, Vector2f(100,0));
            b.clearZoneExploded();
            QVERIFY(!b.zoneJustExploded());
        }
    }
    void setWallColliders() {
        bossModel b(Vector2f(0,0), 2);
        QVector<QRectF> walls = { QRectF(0,0,64,64) };
        b.setWallColliders(walls); // ne doit pas crasher
    }
};