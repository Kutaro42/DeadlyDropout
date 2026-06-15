#pragma once
#include <QtTest/QtTest>
#include "exitdoormodel.h"
#include "standarddoormodel.h"
#include "bossdoormodel.h"
#include "playermodel.h"

class TestDoors : public QObject {
    Q_OBJECT
private slots:
    // ── ExitDoor ──────────────────────────────────────────────
    void exitDoorInitiallyClosed() {
        ExitDoorModel d(Vector2f(0,0), 128.0f, 128.0f);
        QVERIFY(!d.isOpen());
    }
    void exitDoorOpen() {
        ExitDoorModel d(Vector2f(0,0), 128.0f, 128.0f);
        d.openDoor();
        QVERIFY(d.isOpen());
    }
    void exitDoorCollider() {
        ExitDoorModel d(Vector2f(10,20), 128.0f, 64.0f);
        QRectF c = d.getCollider();
        QCOMPARE(c.x(),      10.0);
        QCOMPARE(c.y(),      20.0);
        QCOMPARE(c.width(),  128.0);
        QCOMPARE(c.height(), 64.0);
    }

    // ── StandardDoor ──────────────────────────────────────────
    void standardDoorInitiallyClosed() {
        StandardDoorModel d(Vector2f(0,0), 64.0f, 128.0f);
        QVERIFY(!d.isOpen());
    }
    void standardDoorOpenOnContact() {
        StandardDoorModel d(Vector2f(0,0), 64.0f, 64.0f);
        PlayerModel p(Vector2f(0,0));
        bool opened = d.tryOpen(p);
        QVERIFY(opened);
        QVERIFY(d.isOpen());
    }
    void standardDoorNoReopenAlreadyOpen() {
        StandardDoorModel d(Vector2f(0,0), 64.0f, 64.0f);
        PlayerModel p(Vector2f(0,0));
        d.tryOpen(p);
        bool second = d.tryOpen(p);
        QVERIFY(!second);
    }
    void standardDoorNoOpenWhenFar() {
        StandardDoorModel d(Vector2f(0,0), 64.0f, 64.0f);
        PlayerModel p(Vector2f(9999,9999));
        bool opened = d.tryOpen(p);
        QVERIFY(!opened);
        QVERIFY(!d.isOpen());
    }
    void standardDoorOpenDoor() {
        StandardDoorModel d(Vector2f(0,0), 64.0f, 64.0f);
        d.openDoor();
        QVERIFY(d.isOpen());
    }

    // ── BossDoor ──────────────────────────────────────────────
    void bossDoorInitiallyUnlocked() {
        BossDoorModel d(Vector2f(0,0), 128.0f, 64.0f);
        QVERIFY(!d.isLocked());
    }
    void bossDoorTraverseTeleports() {
        BossDoorModel d(Vector2f(0,0), 128.0f, 64.0f);
        PlayerModel p(Vector2f(0,0));
        Vector2f teleport;
        bool ok = d.tryTraverse(p.getCollider(), teleport);
        QVERIFY(ok);
        QVERIFY(d.isLocked());
    }
    void bossDoorLockedAfterTraverse() {
        BossDoorModel d(Vector2f(0,0), 128.0f, 64.0f);
        PlayerModel p(Vector2f(0,0));
        Vector2f tp;
        d.tryTraverse(p.getCollider(), tp);
        // Deuxième tentative refusée
        bool second = d.tryTraverse(p.getCollider(), tp);
        QVERIFY(!second);
    }
    void bossDoorNoTraverseWhenFar() {
        BossDoorModel d(Vector2f(0,0), 128.0f, 64.0f);
        PlayerModel p(Vector2f(9999,9999));
        Vector2f tp;
        bool ok = d.tryTraverse(p.getCollider(), tp);
        QVERIFY(!ok);
        QVERIFY(!d.isLocked());
    }
};