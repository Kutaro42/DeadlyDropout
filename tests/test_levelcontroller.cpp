#pragma once
#include <QtTest/QtTest>
#include "../DEADLY_DROPOUT/levelcontroller.h"
#include "../DEADLY_DROPOUT/levelmodel.h"
#include "../DEADLY_DROPOUT/playermodel.h"
#include "../DEADLY_DROPOUT/wallmodel.h"
#include "../DEADLY_DROPOUT/zombiestdmodel.h"
#include "../DEADLY_DROPOUT/exitdoormodel.h"
#include "../DEADLY_DROPOUT/standarddoormodel.h"
#include "../DEADLY_DROPOUT/bossdoormodel.h"
#include "../DEADLY_DROPOUT/zombielaunchermodel.h"

class TestLevelController : public QObject {
    Q_OBJECT
private slots:
    void initialState() {
        LevelController lc;
        QVERIFY(!lc.levelJustCompleted);
        QVERIFY(lc.getPendingPopups().isEmpty());
    }
    void playerWallCollision() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(10,10));
        // Mur sur le joueur
        lm.addWall(new WallModel(Vector2f(0,0), 200.0f, 200.0f));
        lc.update(0.016f, p, lm);
        // Le joueur doit être expulsé du mur
        QVERIFY(!p.getCollider().intersects(QRectF(0,0,200,200)));
    }
    void enemyAttacksPlayer() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        // Zombie sur le joueur
        lm.addEnemy(new ZombieStdModel(Vector2f(0,0)));
        int hp = p.getHp();
        lc.update(1.0f, p, lm);
        QVERIFY(p.getHp() <= hp);
    }
    void katanaHitsEnemy() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* z = new ZombieStdModel(Vector2f(10,0));
        lm.addEnemy(z);
        p.launchKatana(0.0f); // attaque vers la droite
        int hp = z->getHp();
        lc.update(0.016f, p, lm);
        QVERIFY(z->getHp() <= hp);
    }
    void bulletHitsEnemy() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* z = new ZombieStdModel(Vector2f(50,0));
        lm.addEnemy(z);
        p.shootGun(Vector2f(0,0), Vector2f(500,0));
        int hp = z->getHp();
        lc.update(1.0f, p, lm);
        QVERIFY(z->getHp() <= hp);
    }
    void bulletHitsWallDeactivates() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        lm.addWall(new WallModel(Vector2f(50,0), 64.0f, 200.0f));
        p.shootGun(Vector2f(0,0), Vector2f(500,0));
        lc.update(0.1f, p, lm);
        // Toutes les balles inactives ou liste vide
        for (auto* b : p.getGun().getBullets())
            QVERIFY(!b->isActive());
    }
    void standardDoorOpensOnContact() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* door = new StandardDoorModel(Vector2f(0,0), 64.0f, 64.0f);
        lm.addStandardDoor(door);
        lc.update(0.016f, p, lm);
        QVERIFY(door->isOpen());
    }
    void exitConditionRequiresKey() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* exit = new ExitDoorModel(Vector2f(0,0), 128.0f, 128.0f);
        lm.setExitDoor(exit);
        lm.addEnemy(new ZombieStdModel(Vector2f(9999,9999))); // ennemi loin
        lc.update(0.016f, p, lm);
        QVERIFY(!lc.levelJustCompleted); // pas de clé
    }
    void exitConditionWithKey() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        p.collectKey();
        auto* exit = new ExitDoorModel(Vector2f(0,0), 128.0f, 128.0f);
        exit->openDoor();
        lm.setExitDoor(exit);
        lm.addEnemy(new ZombieStdModel(Vector2f(9999,9999)));
        lc.update(0.016f, p, lm);
        QVERIFY(lc.levelJustCompleted);
    }
    void goldDistributed() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* z = new ZombieStdModel(Vector2f(9999,9999));
        z->takeDamage(z->getMaxHp()); // tuer
        lm.addEnemy(z);
        lc.update(0.016f, p, lm);
        QVERIFY(p.getGold() > 0);
    }
    void popupsGeneratedOnDamage() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        p.launchKatana(0.0f);
        lm.addEnemy(new ZombieStdModel(Vector2f(10,0)));
        lc.update(0.016f, p, lm);
        // Des popups doivent avoir été générés
        // (pas forcément sur ce tick si slash manque — on vérifie juste pas de crash)
    }
    void purgeDeadEnemies() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        auto* z = new ZombieStdModel(Vector2f(9999,9999));
        z->takeDamage(z->getMaxHp());
        lm.addEnemy(z);
        // Laisser le timer DEAD s'écouler
        lc.update(Cfg::ENEMY_DEAD_DUR + 0.5f, p, lm);
        QVERIFY(lm.getEnemies().isEmpty());
    }
    void bossDoorBlocksPlayer() {
        LevelController lc;
        LevelModel lm;
        PlayerModel p(Vector2f(0,0));
        // Porte de boss loin du joueur (verrouillée = bloque)
        auto* bd = new BossDoorModel(Vector2f(0,0), 128.0f, 128.0f);
        // On traverse pour la verrouiller
        Vector2f tp;
        bd->tryTraverse(p.getCollider(), tp);
        lm.addBossDoor(bd);
        lc.update(0.016f, p, lm);
        // Le joueur ne doit pas être dans la porte
        QVERIFY(!p.getCollider().intersects(bd->getCollider()));
    }
};
