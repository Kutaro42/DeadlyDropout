#pragma once
#include <QtTest/QtTest>
#include "../DEADLY_DROPOUT/katana.h"
#include <cmath>

class TestKatana : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        Katana k;
        QVERIFY(k.isReady());
        QVERIFY(!k.isAnimActive());
        QCOMPARE(k.getDamage(), Cfg::KATANA_DAMAGE);
    }
    void tryAttack() {
        Katana k;
        bool launched = k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        QVERIFY(launched);
        QVERIFY(!k.isReady());
        QVERIFY(k.isAnimActive());
    }
    void cooldownPreventsAttack() {
        Katana k;
        k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        bool second = k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        QVERIFY(!second);
    }
    void updateCooldown() {
        Katana k;
        k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        k.update(Cfg::KATANA_COOLDOWN + 0.1f);
        QVERIFY(k.isReady());
    }
    void slashZoneNonEmpty() {
        Katana k;
        k.tryAttack(Vector2f(100,100), 64.0f, 64.0f, 0.0f);
        QRectF z = k.getSlashZone();
        QVERIFY(z.width() > 0);
        QVERIFY(z.height() > 0);
    }
    void attackAngle() {
        Katana k;
        float angle = static_cast<float>(M_PI / 4.0);
        k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, angle);
        QVERIFY(qAbs(k.getAttackAngle() - angle) < 1e-5f);
    }
    void cooldownRatioAfterAttack() {
        Katana k;
        k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        QVERIFY(k.getCooldownRatio() > 0.0f);
    }
    void cooldownRatioWhenReady() {
        Katana k;
        QCOMPARE(k.getCooldownRatio(), 0.0f);
    }
    void setDamage() {
        Katana k;
        k.setDamage(99);
        QCOMPARE(k.getDamage(), 99);
    }
    void setCooldown() {
        Katana k;
        k.setCooldown(1.0f);
        k.tryAttack(Vector2f(0,0), 64.0f, 64.0f, 0.0f);
        k.update(0.5f);
        QVERIFY(!k.isReady());
        k.update(0.6f);
        QVERIFY(k.isReady());
    }
    void slashZoneDirectionRight() {
        Katana k;
        k.tryAttack(Vector2f(100,100), 64.0f, 64.0f, 0.0f); // droite
        QRectF z = k.getSlashZone();
        // Le centre de la zone doit être à droite du joueur
        QVERIFY(z.center().x() > 100.0 + 64.0/2.0);
    }
    void slashZoneDirectionLeft() {
        Katana k;
        k.tryAttack(Vector2f(100,100), 64.0f, 64.0f, static_cast<float>(M_PI)); // gauche
        QRectF z = k.getSlashZone();
        QVERIFY(z.center().x() < 100.0 + 64.0/2.0);
    }
};
