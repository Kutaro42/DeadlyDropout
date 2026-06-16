#pragma once
#include <QtTest/QtTest>
#include "../DEADLY_DROPOUT/bulletmodel.h"
#include "../DEADLY_DROPOUT/rubblemodel.h"

class TestProjectile : public QObject {
    Q_OBJECT
private slots:
    void bulletInitial() {
        BulletModel b(Vector2f(0,0), Vector2f(1,0));
        QVERIFY(b.isActive());
        QCOMPARE(b.getDamage(), Cfg::BULLET_DAMAGE);
        QCOMPARE(b.getSpeed(),  Cfg::BULLET_SPEED);
    }
    void bulletMoves() {
        BulletModel b(Vector2f(0,0), Vector2f(1,0));
        b.update(1.0f);
        QVERIFY(b.getPosition().x > 0.0f);
        QCOMPARE(b.getPosition().y, 0.0f);
    }
    void bulletMovesVertical() {
        BulletModel b(Vector2f(0,0), Vector2f(0,1));
        b.update(1.0f);
        QCOMPARE(b.getPosition().x, 0.0f);
        QVERIFY(b.getPosition().y > 0.0f);
    }
    void bulletCustomDamage() {
        BulletModel b(Vector2f(0,0), Vector2f(1,0), 99);
        QCOMPARE(b.getDamage(), 99);
    }
    void bulletInactiveDoesNotMove() {
        BulletModel b(Vector2f(100,100), Vector2f(1,0));
        b.setActive(false);
        b.update(1.0f);
        QCOMPARE(b.getPosition().x, 100.0f);
    }
    void rubbleInitial() {
        RubbleModel r(Vector2f(0,0), Vector2f(1,0));
        QVERIFY(r.isActive());
        QCOMPARE(r.getDamage(), Cfg::RUBBLE_DAMAGE);
        QCOMPARE(r.getSpeed(),  Cfg::RUBBLE_SPEED);
    }
    void rubbleMoves() {
        RubbleModel r(Vector2f(0,0), Vector2f(1,0));
        r.update(0.1f);
        QVERIFY(r.getPosition().x > 0.0f);
    }
    void bulletCollider() {
        BulletModel b(Vector2f(10,10), Vector2f(1,0));
        QRectF c = b.getCollider();
        QCOMPARE(c.x(), 10.0);
        QCOMPARE(c.y(), 10.0);
    }
    void directionNormalized() {
        BulletModel b(Vector2f(0,0), Vector2f(3,4)); // pas normalisé en entrée
        Vector2f d = b.getDirection();
        float len = d.length();
        QVERIFY(qAbs(len - 1.0f) < 1e-5f);
    }
};
