/**
 * @file playermodel.h
 * @brief Declaration of the PlayerModel class, the central model component for the player.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entity.h"
#include "katana.h"
#include "gunmodel.h"
#include "config.h"
#include <cmath>
#include <QSoundEffect>

// ═══════════════════════════════════════════════════════════════════
//  PlayerModel — player state and logic (Model layer, MVC).
//  Delegates all weapon-related logic to the Katana object.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerModel
 * @brief Model encapsulating the player's properties, resources and actions.
 *
 * Manages HP, speed, gold, keys and cardinal orientation.
 * Katana actions are delegated to the internal instance.
 *
 * The cardinal orientation (facingDir) is determined either by
 * keyboard movement or by the attack angle passed to launchKatana().
 * Priority rule: if the player is attacking, the cardinal direction is
 * the one closest to the attack angle; otherwise it follows the keyboard.
 */
class PlayerModel : public Entity {
private:
    int             hp;
    int             maxHp;
    float           speed;
    bool            speedBoosted;
    int             gold;
    bool            hasKey;
    bool            moving;

    FacingDirection facingDir; ///< Current cardinal direction (keyboard or attack).

    Katana          katana;
    GunModel        gun;
    QSoundEffect* damageSound;
    QSoundEffect* deathSound;

public:
    // ---- Katana attack -----------------------------------------------------
    /**
     * @brief PlayerModel constructor.
     * @param startPos Player starting position.
     */
    PlayerModel(Vector2f startPos);
    /**
     * @brief PlayerModel destructor, releases sound resources.
     */
    ~PlayerModel();

    /**
     * @brief Updates the player state (cooldowns, animations, etc.).
     * @param dt Delta time.
     */
    void update(float dt) override;

    // ---- Movement -----------------------------------------------------
    /**
    * @brief Moves the player according to an input direction and delta time.
    * @param inputDir Raw input direction (e.g. keyboard), not normalized.
    */
    void move(Vector2f inputDir, float dt);
    void stopMoving() { moving = false; }

    // ---- Katana attack -----------------------------------------------------
    /**
     * @brief Launches a katana attack aimed at a free angle.
     *
     * Converts the angle into the dominant cardinal direction to pick the
     * correct sprite, then delegates to Katana::tryAttack() with the exact angle
     * so that the SlashZone is oriented precisely toward the cursor.
     *
     * @param angleRad Angle in radians (standard atan2: 0=right, π/2=down).
     * @return true if the attack was triggered, false if the katana is on cooldown.
     */
    bool launchKatana(float angleRad);

    // ---- HP and player state management -----------------------------
    /**
     * @brief Applies damage to the player.
     * @param amount Amount of damage to inflict.
     */
    void takeDamage(int amount);
    bool isDead() const { return hp <= 0; }

    // ---- Gold and key management ----------------------------------------
    /**
     * @brief Adds gold to the player.
     * @param amount Amount of gold to add.
     */
    void addGold(int amount) { gold += amount; }
    /**
     * @brief Sets hasKey to true, indicating the player has collected a key.
     */
    void collectKey()        { hasKey = true;  }

    // ---- Getters -----------------------------------------------------
    int             getHp()           const { return hp;        }
    int             getMaxHp()        const { return maxHp;     }
    int             getGold()         const { return gold;      }
    bool            getHasKey()       const { return hasKey;    }
    bool            isMoving()        const { return moving;    }
    FacingDirection getFacing()       const { return facingDir; }
    float           getSpeed()        const { return speed; }

    const Katana&   getKatana()       const { return katana;    }
    bool            isKatanaActive()  const { return katana.isAnimActive();   }
    QRectF          getSlashZone()    const { return katana.getSlashZone();   }
    float           getAttackAngle()  const { return katana.getAttackAngle(); }

    GunModel&       getGun()                { return gun; }
    const GunModel& getGun()          const { return gun; }  // Note for alex: Why two?

    /**
     * @brief Attempts to fire a bullet toward targetPos (scene coordinates).
     * @param muzzlePos Position of the gun muzzle.
     * @param targetPos Target position (mouse cursor in scene coordinates).
     * @return true if a shot was fired.
     */
    bool shootGun(Vector2f muzzlePos, Vector2f targetPos)
    { return gun.tryShoot(muzzlePos, targetPos); }

    /** @brief Reloads the gun (R key). */
    void reloadGun() { gun.reload(); }

    // ---- Setters -----------------------------------------------------
    void setMaxHp(int v)          { maxHp = v;  }
    void setHp(int life)          { hp = life;  }
    void setSpeed(float v)        { speed = v;  }
    bool getBoostState();
    void setHasKey(bool key);

    void setKatanaDamage(int v)    { katana.setDamage(v);    }
    void setKatanaRange(int v)     { katana.setRange(v);     }
    void setKatanaCooldown(float v){ katana.setCooldown(v);  }

private:
    /**
     * @brief Converts a radian angle into the dominant cardinal FacingDirection.
     * @param angleRad Angle (standard atan2).
     * @return The closest cardinal direction.
     */
    static FacingDirection angleToFacing(float angleRad);
};

#endif // PLAYERMODEL_H
