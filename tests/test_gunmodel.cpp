#pragma once
#include <QtTest/QtTest>
#include "gunmodel.h"

class TestGunModel : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        GunModel g;
        QCOMPARE(g.getMag(),     Cfg::GUN_MAG_SIZE);
        QCOMPARE(g.getMagSize(), Cfg::GUN_MAG_SIZE);
        QVERIFY(g.getReserve()   > 0);
        QVERIFY(g.isReady());
    }
    void shoot() {
        GunModel g;
        bool fired = g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        QVERIFY(fired);
        QCOMPARE(g.getMag(), Cfg::GUN_MAG_SIZE - 1);
        QVERIFY(!g.isReady());
    }
    void cooldownPreventsShoot() {
        GunModel g;
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        bool second = g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        QVERIFY(!second);
    }
    void updateClearsShootTimer() {
        GunModel g;
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        g.update(Cfg::GUN_SHOOT_CD + 0.1f);
        QVERIFY(g.isReady());
    }
    void emptyMagPreventsShoot() {
        GunModel g(1, 0); // chargeur 1, réserve 0
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        g.update(Cfg::GUN_SHOOT_CD + 0.1f);
        bool fired = g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        QVERIFY(!fired);
    }
    void reload() {
        GunModel g(10, 20);
        // Vider le chargeur
        for (int i = 0; i < 10; ++i) {
            g.tryShoot(Vector2f(0,0), Vector2f(100,0));
            g.update(Cfg::GUN_SHOOT_CD + 0.01f);
        }
        QCOMPARE(g.getMag(), 0);
        g.reload();
        QCOMPARE(g.getMag(), 10);
        QCOMPARE(g.getReserve(), 10);
    }
    void reloadPartial() {
        GunModel g(10, 3); // réserve seulement 3
        for (int i = 0; i < 10; ++i) {
            g.tryShoot(Vector2f(0,0), Vector2f(100,0));
            g.update(Cfg::GUN_SHOOT_CD + 0.01f);
        }
        g.reload();
        QCOMPARE(g.getMag(), 3);
        QCOMPARE(g.getReserve(), 0);
    }
    void reloadWhenFull() {
        GunModel g(10, 20);
        int reserveBefore = g.getReserve();
        g.reload();
        QCOMPARE(g.getMag(),     10);
        QCOMPARE(g.getReserve(), reserveBefore); // rien prélevé
    }
    void bulletsCreated() {
        GunModel g;
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        QCOMPARE(g.getBullets().size(), 1);
    }
    void pruneInactiveBullets() {
        GunModel g;
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        g.getBullets()[0]->setActive(false);
        g.pruneInactiveBullets();
        QCOMPARE(g.getBullets().size(), 0);
    }
    void addReserve() {
        GunModel g(10, 0);
        g.addReserve(15);
        QCOMPARE(g.getReserve(), 15);
    }
    void cooldownRatio() {
        GunModel g;
        QCOMPARE(g.getCooldownRatio(), 0.0f);
        g.tryShoot(Vector2f(0,0), Vector2f(100,0));
        QVERIFY(g.getCooldownRatio() > 0.0f);
    }
};