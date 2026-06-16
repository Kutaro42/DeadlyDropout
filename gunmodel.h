/**
 * @file gunmodel.h
 * @brief Model of the player's semi-automatic pistol.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#ifndef GUNMODEL_H
#define GUNMODEL_H

#include <QVector>
#include "bulletmodel.h"
#include "config.h"
#include "vector2f.h"
#include <QSoundEffect>

/**
 * @class GunModel
 * @brief Manages firing logic, magazine, ammo reserve and shoot cooldown (GDD Step 5).
 *
 * GDD ammunition rules:
 *  - Semi-auto: 1 click = 1 shot, 0.5 s cooldown between shots.
 *  - Magazine (mag) + general reserve (reserve).
 *  - Press R to reload: fills the magazine from reserve, preserving existing rounds.
 *  - Damage: 12 base + random 2–5 bonus + 2 % critical chance × 2 multiplier.
 *  - Bullet speed: 500 px/s.
 *
 * BulletModel instances are owned by GunModel and updated every tick.
 * LevelController accesses getBullets() to resolve collision with enemies.
 *
 * Sound effects are allocated here rather than in the view because the trigger
 * condition (successful shot / empty mag) is a model-level event, and playing
 * audio in the view would require it to poll the model state every frame.
 */
class GunModel : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs GunModel with configurable magazine and reserve sizes.
     * @param magSize     Magazine capacity (Cfg::GUN_MAG_SIZE by default).
     * @param reserveSize Initial reserve ammo (2 × magSize by default).
     */
    explicit GunModel(int magSize     = Cfg::GUN_MAG_SIZE,
                      int reserveSize = Cfg::GUN_MAG_SIZE * 2);

    ~GunModel();

    /**
     * @brief Advances the shoot timer and moves all active bullets.
     *
     * Must be called once per game-loop tick so that cooldown tracking and
     * bullet positions stay in sync with the rest of the simulation.
     *
     * @param dt Delta-time in seconds.
     */
    void update(float dt);

    /**
     * @brief Attempts to fire one bullet if the cooldown has elapsed and the mag is non-empty.
     *
     * Returns false (and plays an empty-mag click) when no shot can be fired.
     * This pattern avoids exposing internal timer state to callers.
     *
     * @param muzzlePos World-space position of the gun barrel tip.
     * @param targetPos World-space position of the mouse cursor (aim target).
     * @return true if a bullet was successfully created.
     */
    bool tryShoot(Vector2f muzzlePos, Vector2f targetPos);

    /**
     * @brief Reloads the magazine from the general reserve (GDD ammo rules).
     *
     * Preserves rounds already in the mag and only transfers what is needed
     * to fill it, so the player does not waste ammo by reloading mid-magazine.
     */
    void reload();

    /** @brief Returns the live bullet list for collision processing by LevelController. */
    QVector<BulletModel*>& getBullets() { return bullets; }

    /**
     * @brief Removes inactive bullets from the list and frees their memory.
     *
     * Called by GunView after updating bullet positions so that dead bullets
     * are not processed again in the next tick.
     */
    void pruneInactiveBullets();

    // ─── HUD getters ──────────────────────────────────────────
    int   getMag()      const { return mag;      } ///< Rounds remaining in the magazine.
    int   getMagSize()  const { return magSize;  } ///< Maximum magazine capacity.
    int   getReserve()  const { return reserve;  } ///< Rounds in the general reserve.
    bool  isReady()     const { return shootTimer <= 0.0f; } ///< True when the cooldown has elapsed.

    /**
     * @brief Cooldown progress ratio for HUD display (1.0 = just fired, 0.0 = ready).
     */
    float getCooldownRatio() const {
        return (shootCooldown > 0.0f) ? (shootTimer / shootCooldown) : 0.0f;
    }
    float getShootCooldown() const { return shootCooldown; }
    int   getBaseDamage()    const { return baseDamage;    }
    float getCritChance()    const { return critChance;    }

    // ─── Upgrade setters ──────────────────────────────────────
    void addReserve(int amount)        { reserve += amount; }
    void setShootCooldown(float cd)    { shootCooldown = cd; }
    void setBaseDamage(int d)          { baseDamage = d; }
    void setCritChance(float c)        { critChance = c; }

private:
    QVector<BulletModel*> bullets;   ///< All bullets currently in flight (active or awaiting cleanup).
    QSoundEffect* emptyMagSound;     ///< Played when the player tries to fire an empty magazine.
    QSoundEffect* gunFireSound;      ///< Played on every successful shot.

    int   magSize;       ///< Maximum rounds the magazine can hold.
    int   mag;           ///< Rounds currently in the magazine.
    int   reserve;       ///< Rounds held in the general reserve.

    float shootCooldown; ///< Minimum time between shots in seconds (GDD: 0.5 s base).
    float shootTimer;    ///< Time remaining before the next shot is allowed.

    int   baseDamage;    ///< Base damage before random bonus and critical multiplier.
    float critChance;    ///< Probability of a critical hit [0..1] (GDD: 2 % base).

    /**
     * @brief Computes final bullet damage: base + random bonus + optional critical.
     *
     * Called once per shot so that each bullet has its own independent damage roll,
     * matching the GDD's "random law" specification for ammunition damage.
     *
     * @return Final damage value for the new bullet.
     */
    int computeDamage() const;
};

#endif // GUNMODEL_H