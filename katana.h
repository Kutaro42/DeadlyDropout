/**
 * @file katana.h
 * @brief Declaration of the Katana class, the player's melee weapon.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef KATANA_H
#define KATANA_H

#include <QRectF>
#include "config.h"
#include "vector2f.h"
#include <QSoundEffect>

/**
 * @class Katana
 * @brief Model of the player's melee weapon (GDD Step 5 — Combat rules).
 *
 * Manages weapon statistics (damage, range), its collision zone (SlashZone),
 * and timers for cooldown and animation.
 *
 * Design decision — free-angle slash:
 *  The SlashZone is computed from an arbitrary angle (radians) passed to tryAttack().
 *  This allows attacking in any direction (not just the 4 cardinals), matching the
 *  GDD requirement that the katana targets "enemies within the player's range zone"
 *  oriented toward the mouse cursor.
 *
 *  The AABB returned by getSlashZone() is the bounding box of the oriented slash
 *  rectangle, ensuring compatibility with the existing AABB collision tests in
 *  LevelController without requiring a full OBB intersection algorithm.
 *
 * The attack angle is exposed via getAttackAngle() so PlayerView can orient
 * the slash sprite effect correctly.
 *
 * @version 2.0
 */
class Katana {
public:
    Katana();
    ~Katana();

    /**
     * @brief Advances the cooldown and animation timers.
     *
     * Must be called once per tick so that isReady() and isAnimActive()
     * reflect the current game time.
     *
     * @param dt Delta-time in seconds.
     */
    void update(float dt);

    /**
     * @brief Attempts to launch an attack in the direction given by an angle.
     *
     * Fails silently (returns false) if the weapon is still on cooldown,
     * so the caller does not need to query isReady() separately.
     *
     * @param playerPos  Top-left corner of the player's hitbox in world space.
     * @param playerW    Hitbox width in pixels.
     * @param playerH    Hitbox height in pixels.
     * @param angleRad   Attack direction in radians (atan2 standard: 0 = right, π/2 = down).
     * @return true if the attack was launched (cooldown was ready), false otherwise.
     */
    bool tryAttack(Vector2f playerPos, float playerW, float playerH,
                   float angleRad);

    // ─── Getters ─────────────────────────────────────────────────────
    int    getDamage()        const { return itsDamage;   }
    int    getRange()         const { return itsRange;    }
    bool   isReady()          const { return itsReady;    }

    /**
     * @brief Cooldown progress ratio (1.0 = just attacked, 0.0 = ready).
     *
     * Exposed so MarketController can read the current cooldown duration
     * for the AttackSpeedPowerUp upgrade calculation.
     */
    float  getCooldownRatio() const {
        return (itsCooldown > 0.0f) ? (itsCooldownTimer / itsCooldown) : 0.0f;
    }
    bool   isAnimActive()     const { return itsAnimTimer > 0.0f; }

    /**
     * @brief Returns the AABB bounding box of the oriented slash rectangle.
     *
     * Used by LevelController for AABB-based collision tests with enemies.
     * The AABB over-approximates the true oriented rectangle, which can cause
     * false positives at extreme angles; acceptable for a game with 64 px entities.
     */
    QRectF getSlashZone()     const { return itsSlashZone; }

    /**
     * @brief Returns the angle (radians) of the last triggered attack.
     *
     * Read by PlayerView each frame to orient the slash animation sprite
     * correctly, decoupling the visual effect from the input handling layer.
     */
    float  getAttackAngle()   const { return itsAttackAngle; }

    // ─── Upgrade setters ─────────────────────────────────────────────
    void setDamage(int d)     { itsDamage   = d; }
    void setRange(int r)      { itsRange    = r; }
    void setCooldown(float c) { itsCooldown = c; }

private:
    int    itsDamage;         ///< Base damage per slash (GDD: 30 without upgrades).
    int    itsRange;          ///< Slash reach in pixels (depth of the slash rectangle).
    float  itsCooldown;       ///< Minimum time between attacks in seconds.
    bool   itsReady;          ///< True when the weapon can be used again.

    float  itsCooldownTimer;  ///< Time remaining on the current cooldown.
    float  itsAnimTimer;      ///< Time remaining for the slash animation overlay.

    /// AABB of the oriented slash rectangle (used by LevelController collision tests).
    QRectF itsSlashZone;

    /// Angle (radians) of the last attack; used by PlayerView to orient the slash sprite.
    float  itsAttackAngle;

    QSoundEffect* itsSlashSound; ///< Played once each time tryAttack() succeeds.

    /**
     * @brief Computes the AABB enclosing the oriented slash rectangle for a given angle.
     *
     * The logical slash rectangle has dimensions KATANA_SLASH_W × itsRange.
     * Its near edge starts at the player's hitbox boundary (not at the centre)
     * to avoid hitting enemies directly behind the player.
     * The four corners are rotated by angleRad and their axis-aligned bounding
     * box is computed to produce the final AABB.
     *
     * @param playerPos Top-left corner of the player's hitbox.
     * @param playerW   Hitbox width.
     * @param playerH   Hitbox height.
     * @param angleRad  Attack direction in radians.
     */
    void computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                          float angleRad);
};

#endif // KATANA_H