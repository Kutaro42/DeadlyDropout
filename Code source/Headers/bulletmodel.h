/**
 * @file bulletmodel.h
 * @brief A model of a bullet fired from the player's pistol.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#ifndef BULLETMODEL_H
#define BULLETMODEL_H

#include "projectile.h"
#include "config.h"

/**
 * @class BulletModel
 * @brief Projectile fired by the player (semi-automatic pistol).
 *
 * Inherits from Projectile: moves in a straight line at a constant speed.
 * Deactivates (setActive(false)) upon collision with a wall or an enemy,
 * handled by LevelController::checkBulletCollisions().
 *
 * Damage: 12 base points + adaptive bonus 2–5 + critical hit ×2 (GDD).
 * Speed: 500 px/s (GDD).
 */
class BulletModel : public Projectile
{
public:
    /**
     * @brief Create a bullet
     * @param pos :   Starting position (nozzle).
     * @param dir :   Standard direction towards the mouse cursor.
     * @param damage : Damage already calculated (base + bonus + critical) by GunModel.
     * @param speed : Speed in px/s (default: Cfg::BULLET_SPEED).
     */
    BulletModel(Vector2f pos,
                Vector2f dir,
                int      damage = Cfg::BULLET_DAMAGE,
                float    speed  = Cfg::BULLET_SPEED);
};

#endif // BULLETMODEL_H
