/**
 * @file rubblemodel.h
 * @brief Description of the rubble model, a projectile fired by the ZombieLauncher.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef RUBBLEMODEL_H
#define RUBBLEMODEL_H

#include "projectile.h"

// ═══════════════════════════════════════════════════════════════════
//  RubbleModel — concrete projectile fired by ZombieLauncherModel.
//  Inherits from Projectile (→ Entity).
//  Straight-line movement delegated to Projectile::update().
//  Deactivates as soon as it hits a wall or the player
//  (via setActive(false) in LevelController).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class RubbleModel
 * @brief Rubble (projectile) fired by the launcher zombie.
 */
class RubbleModel : public Projectile {
public:
    /**
     * @brief Constructs a rubble projectile.
     * @param pos    Starting position (launcher center, adjusted).
     * @param dir    Normalized direction toward the target at the time of firing.
     * @param damage Damage inflicted on the player on impact.
     * @param speed  Speed in pixels/s (default: Cfg::RUBBLE_SPEED).
     */
    RubbleModel(Vector2f pos,
                Vector2f dir,
                int      damage = Cfg::RUBBLE_DAMAGE,
                float    speed  = Cfg::RUBBLE_SPEED);
};

#endif // RUBBLEMODEL_H
