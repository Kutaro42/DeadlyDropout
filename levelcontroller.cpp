#include "levelcontroller.h"
#include "enemy.h"
#include "wallmodel.h"
#include "exitdoormodel.h"
#include <QRectF>

LevelController::LevelController()
    : levelJustCompleted(false)
{}

// ── Point d'entrée principal ──────────────────────────────────────
void LevelController::update(float dt,
                               PlayerModel& player,
                               LevelModel&  level)
{
    levelJustCompleted = false;

    updateEnemies(dt, player, level);
    distributeGold(player, level);

    resolvePlayerWallCollisions(player, level);
    resolveEnemyWallCollisions(level);

    checkPlayerEnemyCollisions(player, level);

    if (player.isKatanaActive())
        checkSlashZoneHits(player, level);

    checkExitCondition(player, level);
}

// ── IA : mise à jour de chaque ennemi actif ───────────────────────
void LevelController::updateEnemies(float dt,
                                     PlayerModel& player,
                                     LevelModel&  level)
{
    for (Enemy* e : level.getEnemies())
        if (e->isActive())
            e->update(dt, player.getPosition());
}

// ── Gold : distribué une seule fois à la mort de chaque ennemi ────
void LevelController::distributeGold(PlayerModel& player,
                                      LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() && !e->hasDroppedGold()) {
            player.addGold(e->getGoldDrop());
            e->markGoldDropped();
        }
    }
}

// ── Résolution AABB générique ─────────────────────────────────────
void LevelController::resolveAABB(Entity& mover, const QRectF& obstacle)
{
    QRectF m = mover.getCollider();
    if (!m.intersects(obstacle)) return;

    QRectF inter = m.intersected(obstacle);

    if (inter.width() < inter.height()) {
        // Correction sur X (mur vertical)
        double corr = (m.center().x() < obstacle.center().x())
                      ? -inter.width() : inter.width();
        mover.setPosition(Vector2f(
            mover.getPosition().x + static_cast<float>(corr),
            mover.getPosition().y));
    } else {
        // Correction sur Y (mur horizontal)
        double corr = (m.center().y() < obstacle.center().y())
                      ? -inter.height() : inter.height();
        mover.setPosition(Vector2f(
            mover.getPosition().x,
            mover.getPosition().y + static_cast<float>(corr)));
    }
}

// ── Collisions joueur / murs ──────────────────────────────────────
void LevelController::resolvePlayerWallCollisions(PlayerModel& player,
                                                   LevelModel&  level)
{
    for (WallModel* w : level.getWalls())
        resolveAABB(player, w->getCollider());
}

// ── Collisions ennemis / murs ─────────────────────────────────────
void LevelController::resolveEnemyWallCollisions(LevelModel& level) {
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive()) continue;
        for (WallModel* w : level.getWalls())
            resolveAABB(*e, w->getCollider());
    }
}

// ── Contact joueur / ennemi → attaque ─────────────────────────────
void LevelController::checkPlayerEnemyCollisions(PlayerModel& player,
                                                  LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive()) continue;
        if (player.getCollider().intersects(e->getCollider()))
            e->attack(&player);
    }
}

// ── SlashZone katana / ennemis ────────────────────────────────────
void LevelController::checkSlashZoneHits(PlayerModel& player,
                                          LevelModel&  level)
{
    QRectF slash = player.getSlashZone();
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive()) continue;
        if (slash.intersects(e->getCollider()))
            e->takeDamage(player.getKatana().getDamage());
    }
}

// ── Condition de victoire ─────────────────────────────────────────
void LevelController::checkExitCondition(PlayerModel& player,
                                          LevelModel&  level)
{
    ExitDoorModel* exitDoor = level.getExitDoor();
    if (!exitDoor) return;
    // AJOUT : ne pas ouvrir si aucun ennemi n'a jamais existé
    if (level.getEnemies().isEmpty()) return;

    // La sortie s'ouvre quand tous les ennemis sont vaincus
    if (level.allEnemiesDefeated() && !exitDoor->isOpen())
        exitDoor->openDoor();

    // Victoire si joueur entre dans la sortie ouverte
    if (exitDoor->isOpen() &&
        player.getCollider().intersects(exitDoor->getCollider()) && player.getHasKey())
    {
        levelJustCompleted = true;
    }
    
}
