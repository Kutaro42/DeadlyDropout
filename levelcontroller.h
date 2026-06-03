#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include "levelmodel.h"
#include "playermodel.h"
#include <QRectF>

// ═══════════════════════════════════════════════════════════════════
//  LevelController — orchestre la logique d'un niveau :
//    - Mise à jour de l'IA ennemie
//    - Résolution des collisions AABB (joueur/murs, ennemis/murs)
//    - Détection des hits de la SlashZone (katana)
//    - Distribution du gold à la mort d'un ennemi
//    - Ouverture de la sortie + détection de victoire
// ═══════════════════════════════════════════════════════════════════
class LevelController {
public:
    LevelController();

    // Point d'entrée principal, appelé à chaque tick par GameController
    void update(float dt, PlayerModel& player, LevelModel& level);

    // Flag lu par GameController après chaque update()
    bool levelJustCompleted;

private:
    // ─── Sous-étapes de mise à jour ───────────────────────────
    void updateEnemies(float dt, PlayerModel& player, LevelModel& level);
    void distributeGold(PlayerModel& player, LevelModel& level);
    void resolvePlayerWallCollisions(PlayerModel& player, LevelModel& level);
    void resolveEnemyWallCollisions(LevelModel& level);
    void checkPlayerEnemyCollisions(PlayerModel& player, LevelModel& level);
    void checkSlashZoneHits(PlayerModel& player, LevelModel& level);
    void checkExitCondition(PlayerModel& player, LevelModel& level);

    // ─── Résolution AABB générique ────────────────────────────
    // Repousse 'mover' hors du rectangle 'obstacle'.
    // Modifie directement la position de l'entité.
    void resolveAABB(Entity& mover, const QRectF& obstacle);
};

#endif // LEVELCONTROLLER_H
