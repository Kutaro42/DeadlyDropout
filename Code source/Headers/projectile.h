/**
 * @file projectile.h
 * @brief Declaration of the abstract class `Projectile`, the common base for all projectiles.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  Projectile — abstract intermediate class between Entity and
//  concrete projectiles (RubbleModel, BulletModel…).
//
//  Factors out:
//    - normalized movement direction
//    - speed in pixels/s
//    - damage inflicted on impact
//    - straight-line movement (update)
//
//  Subclasses only need to call the protected constructor and
//  can override update() if they require specific ballistic
//  behavior (gravity, curve…).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Projectile
 * @brief Abstract base class for any projectile moving in a straight line.
 */
class Projectile : public Entity {
protected:
    Vector2f direction; ///< Unit displacement vector.
    float    speed;     ///< Speed in pixels/s.
    int      damage;    ///< Damage inflicted on contact with the target.

    /**
     * @brief Protected constructor: only subclasses can instantiate.
     * @param pos    Initial position of the projectile.
     * @param size   Size (width = height) of the sprite in pixels.
     * @param dir    Firing direction (will be normalized).
     * @param speed  Speed in pixels/s.
     * @param damage Damage inflicted on impact.
     */
    Projectile(Vector2f pos, float size,
               Vector2f dir, float speed, int damage);

public:
    virtual ~Projectile() = default;

    /**
     * @brief Advances the projectile in its direction over the given delta-time.
     *        Can be overridden by subclasses (e.g. curved trajectory).
     * @param dt Delta-time in seconds.
     */
    void update(float dt) override;

    // ---- Getters -----------------------------------------------------
    int      getDamage()    const { return damage;    } ///< Damage on impact.
    float    getSpeed()     const { return speed;     } ///< Speed in pixels/s.
    Vector2f getDirection() const { return direction; } ///< Unit displacement vector.
};

#endif // PROJECTILE_H
