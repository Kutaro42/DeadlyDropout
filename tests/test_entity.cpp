#pragma once
#include <QtTest/QtTest>
#include "../DEADLY_DROPOUT/entity.h"

class TestEntity : public QObject {
    Q_OBJECT
private slots:
    void constructor() {
        Entity e(Vector2f(10.0f, 20.0f), 64.0f, 64.0f);
        QCOMPARE(e.getPosition().x, 10.0f);
        QCOMPARE(e.getPosition().y, 20.0f);
        QCOMPARE(e.getWidth(),  64.0f);
        QCOMPARE(e.getHeight(), 64.0f);
        QVERIFY(e.isActive());
    }
    void setPosition() {
        Entity e(Vector2f(0,0), 32.0f, 32.0f);
        e.setPosition(Vector2f(100.0f, 200.0f));
        QCOMPARE(e.getPosition().x, 100.0f);
        QCOMPARE(e.getPosition().y, 200.0f);
    }
    void setActive() {
        Entity e(Vector2f(0,0), 32.0f, 32.0f);
        e.setActive(false);
        QVERIFY(!e.isActive());
        e.setActive(true);
        QVERIFY(e.isActive());
    }
    void collider() {
        Entity e(Vector2f(10.0f, 20.0f), 64.0f, 32.0f);
        QRectF c = e.getCollider();
        QCOMPARE(c.x(),      10.0);
        QCOMPARE(c.y(),      20.0);
        QCOMPARE(c.width(),  64.0);
        QCOMPARE(c.height(), 32.0);
    }
    void colliderIntersection() {
        Entity a(Vector2f(0,0), 64.0f, 64.0f);
        Entity b(Vector2f(32,32), 64.0f, 64.0f);
        QVERIFY(a.getCollider().intersects(b.getCollider()));
    }
    void colliderNoIntersection() {
        Entity a(Vector2f(0,0), 64.0f, 64.0f);
        Entity b(Vector2f(100,100), 64.0f, 64.0f);
        QVERIFY(!a.getCollider().intersects(b.getCollider()));
    }
};
