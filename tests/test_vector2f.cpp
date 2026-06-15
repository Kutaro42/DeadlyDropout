#pragma once
#include <QtTest/QtTest>
#include "vector2f.h"

class TestVector2f : public QObject {
    Q_OBJECT
private slots:
    void defaultConstructor() {
        Vector2f v;
        QCOMPARE(v.x, 0.0f);
        QCOMPARE(v.y, 0.0f);
    }
    void paramConstructor() {
        Vector2f v(3.0f, 4.0f);
        QCOMPARE(v.x, 3.0f);
        QCOMPARE(v.y, 4.0f);
    }
    void length() {
        Vector2f v(3.0f, 4.0f);
        QCOMPARE(v.length(), 5.0f);
    }
    void lengthZero() {
        Vector2f v(0.0f, 0.0f);
        QCOMPARE(v.length(), 0.0f);
    }
    void normalized() {
        Vector2f v(3.0f, 4.0f);
        Vector2f n = v.normalized();
        QVERIFY(qAbs(n.length() - 1.0f) < 1e-5f);
        QVERIFY(qAbs(n.x - 0.6f) < 1e-5f);
        QVERIFY(qAbs(n.y - 0.8f) < 1e-5f);
    }
    void normalizedZero() {
        Vector2f v(0.0f, 0.0f);
        Vector2f n = v.normalized();
        QCOMPARE(n.x, 0.0f);
        QCOMPARE(n.y, 0.0f);
    }
    void addition() {
        Vector2f a(1.0f, 2.0f), b(3.0f, 4.0f);
        Vector2f c = a + b;
        QCOMPARE(c.x, 4.0f);
        QCOMPARE(c.y, 6.0f);
    }
    void subtraction() {
        Vector2f a(5.0f, 7.0f), b(2.0f, 3.0f);
        Vector2f c = a - b;
        QCOMPARE(c.x, 3.0f);
        QCOMPARE(c.y, 4.0f);
    }
    void scalarMultiply() {
        Vector2f v(2.0f, 3.0f);
        Vector2f r = v * 2.0f;
        QCOMPARE(r.x, 4.0f);
        QCOMPARE(r.y, 6.0f);
    }
    void toQPointF() {
        Vector2f v(1.5f, 2.5f);
        QPointF p = v.toQPointF();
        QCOMPARE(p.x(), 1.5);
        QCOMPARE(p.y(), 2.5);
    }
};