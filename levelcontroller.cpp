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

    resolveEnemyEnemyCollisions(level);
    checkAndResolvePlayerEnemyCollisions(player, level);

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

// ── Contact joueur / ennemi → attaque puis séparation physique ───────────────
// L'attaque est vérifiée AVANT la correction de position : l'ennemi inflige
// ses dégâts dès qu'il touche le joueur, puis est repoussé pour éviter
// le chevauchement visuel.
void LevelController::checkAndResolvePlayerEnemyCollisions(PlayerModel& player,
                                                           LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive()) continue;
        if (player.getCollider().intersects(e->getCollider())) {
            e->attack(&player);
            resolveAABB(*e, player.getCollider());
        }
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

// ── Séparation ennemi / ennemi ─────────────────────────────────────
// On répartit la correction à 50/50 pour éviter qu'un seul des deux
// soit repoussé et garde un comportement symétrique.
void LevelController::resolveEnemyEnemyCollisions(LevelModel& level)
{
    QVector<Enemy*>& enemies = level.getEnemies();
    int n = enemies.size();
    for (int i = 0; i < n; ++i) {
        if (!enemies[i]->isActive()) continue;
        for (int j = i + 1; j < n; ++j) {
            if (!enemies[j]->isActive()) continue;

            QRectF a = enemies[i]->getCollider();
            QRectF b = enemies[j]->getCollider();
            if (!a.intersects(b)) continue;

            QRectF inter = a.intersected(b);
            float hx = 0.0f, hy = 0.0f;

            if (inter.width() < inter.height()) {
                float corr = static_cast<float>(inter.width()) * 0.5f;
                hx = (a.center().x() < b.center().x()) ? -corr : corr;
            } else {
                float corr = static_cast<float>(inter.height()) * 0.5f;
                hy = (a.center().y() < b.center().y()) ? -corr : corr;
            }

            Vector2f pa = enemies[i]->getPosition();
            Vector2f pb = enemies[j]->getPosition();
            enemies[i]->setPosition(Vector2f(pa.x + hx, pa.y + hy));
            enemies[j]->setPosition(Vector2f(pb.x - hx, pb.y - hy));
        }
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
