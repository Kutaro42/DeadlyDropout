#pragma once
#include <QtTest/QtTest>
#include "playermodel.h"

class TestPlayerModel : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        PlayerModel p(Vector2f(0,0));
        QCOMPARE(p.getHp(),    Cfg::PLAYER_MAX_HP);
        QCOMPARE(p.getMaxHp(), Cfg::PLAYER_MAX_HP);
        QCOMPARE(p.getGold(),  0);
        QVERIFY(!p.getHasKey());
        QVERIFY(!p.isMoving());
        QVERIFY(!p.isDead());
    }
    void takeDamage() {
        PlayerModel p(Vector2f(0,0));
        p.takeDamage(30);
        QCOMPARE(p.getHp(), Cfg::PLAYER_MAX_HP - 30);
    }
    void takeDamageToZero() {
        PlayerModel p(Vector2f(0,0));
        p.takeDamage(Cfg::PLAYER_MAX_HP);
        QCOMPARE(p.getHp(), 0);
        QVERIFY(p.isDead());
    }
    void takeDamageOverflow() {
        PlayerModel p(Vector2f(0,0));
        p.takeDamage(9999);
        QCOMPARE(p.getHp(), 0);
        QVERIFY(p.isDead());
    }
    void addGold() {
        PlayerModel p(Vector2f(0,0));
        p.addGold(100);
        QCOMPARE(p.getGold(), 100);
        p.addGold(50);
        QCOMPARE(p.getGold(), 150);
    }
    void collectKey() {
        PlayerModel p(Vector2f(0,0));
        QVERIFY(!p.getHasKey());
        p.collectKey();
        QVERIFY(p.getHasKey());
    }
    void setHasKey() {
        PlayerModel p(Vector2f(0,0));
        p.setHasKey(true);
        QVERIFY(p.getHasKey());
        p.setHasKey(false);
        QVERIFY(!p.getHasKey());
    }
    void move() {
        PlayerModel p(Vector2f(0,0));
        p.move(Vector2f(1.0f, 0.0f), 1.0f);
        QVERIFY(p.isMoving());
        QVERIFY(p.getPosition().x > 0.0f);
    }
    void stopMoving() {
        PlayerModel p(Vector2f(0,0));
        p.move(Vector2f(1.0f, 0.0f), 1.0f);
        p.stopMoving();
        QVERIFY(!p.isMoving());
    }
    void moveZeroVector() {
        PlayerModel p(Vector2f(100,100));
        p.move(Vector2f(0,0), 1.0f);
        QVERIFY(!p.isMoving());
        QCOMPARE(p.getPosition().x, 100.0f);
    }
    void facingDirection() {
        PlayerModel p(Vector2f(0,0));
        p.move(Vector2f(1.0f, 0.0f), 0.016f);
        QCOMPARE(p.getFacing(), FacingDirection::RIGHT);
        p.move(Vector2f(-1.0f, 0.0f), 0.016f);
        QCOMPARE(p.getFacing(), FacingDirection::LEFT);
        p.move(Vector2f(0.0f, 1.0f), 0.016f);
        QCOMPARE(p.getFacing(), FacingDirection::DOWN);
        p.move(Vector2f(0.0f, -1.0f), 0.016f);
        QCOMPARE(p.getFacing(), FacingDirection::UP);
    }
    void setMaxHp() {
        PlayerModel p(Vector2f(0,0));
        p.setMaxHp(200);
        QCOMPARE(p.getMaxHp(), 200);
    }
    void setKatanaDamage() {
        PlayerModel p(Vector2f(0,0));
        p.setKatanaDamage(50);
        QCOMPARE(p.getKatana().getDamage(), 50);
    }
    void reloadGun() {
        PlayerModel p(Vector2f(0,0));
        // Tire toutes les balles du chargeur
        for (int i = 0; i < Cfg::GUN_MAG_SIZE; ++i)
            p.shootGun(Vector2f(0,0), Vector2f(100,0));
        // Le rechargement doit marcher sans crash
        p.reloadGun();
        QVERIFY(p.getGun().getMag() > 0 || p.getGun().getReserve() >= 0);
    }
    void update() {
        PlayerModel p(Vector2f(0,0));
        // Doit s'exécuter sans crash
        p.update(0.016f);
    }
};