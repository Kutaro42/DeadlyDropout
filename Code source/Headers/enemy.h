/**
 * @file enemy.h
 * @brief Declaration of the abstract class `Enemy`, which manages enemy behavior.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "config.h"
#include "playermodel.h"

// ═══════════════════════════════════════════════════════════════════
//  Enemy — abstract base class for all enemies.
//  Inherits from Entity. Subclasses: ZombieStdModel, (ThrowerModel…)
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Enemy
 * @brief Base class defining the structure and mechanics of a hostile entity.
 */
class Enemy : public Entity {
protected:
    int          hp;                 ///< Enemy's current health points.
    int          maxHp;              ///< Maximum health points.
    float        speed;              ///< Linear travel speed.
    int          baseDamage;         ///< Base damage dealt to the player.
    float        attackCooldown;     ///< Time required between two attacks.
    float        attackTimer;        ///< Internal timer that controls the attack duration.
    float        detectionRadius;    ///< Range of the aggro/detection zone.
    int          goldDrop;           ///< A golden reward granted upon death.
    bool         goldAlreadyDropped; ///< Anti-duplicate flag for gold loot.
    bool         isBoss = false;     ///< Point to the boss
    EnemyAIState aiState;            ///< State of the AI state machine.
    Vector2f     velocity;           ///< Current direction of motion.
    bool         hitThisSlash;       ///< Prevents multiple hits per attack.
    float        hurtTimer;          ///< Time remaining in the HURT state.
    float        deadTimer;          ///< Time remaining in the DEAD state before deactivation.
    float        chasePersistTimer;  ///< Time remaining before returning to IDLE after loss of sight.

public:
    /**
     * @brief Enemy builder.
     * @param pos : Spawn position.
     * @param maxHP : Health Points.
     * @param speed : Travel speed.
     * @param dmg : Damage.
     * @param detRadius : Detection range.
     * @param w : Width.
     * @param h : Height.
     * @param gold : Gold dropped upon death.
     */
    Enemy(Vector2f pos, int maxHp, float speed, int dmg,
          float detRadius, float w, float h, int gold);
    virtual ~Enemy() = default;

    // ─── Pure virtual interface ──────────────────────────────
    
    /**
     * @brief Updates the enemy's cognitive and spatial behavior.
     * @param dt : Delta time in seconds.
     * @param playerPos : The player's current contact information.
     */
    virtual void update(float dt, Vector2f playerPos) = 0;
    
    /**
     * @brief Launches an attack against the player.
     * @param player : A pointer to the target player's instance.
     */
    virtual void attack(PlayerModel* player) = 0;

    /**
     * @brief Hook called only once when the state changes to DEAD.
     *        Default empty implementation; subclasses can override
     *        to trigger specific effects (sound, particles, etc.).
     */
    virtual void onDeath() {}

    // ─── Common behaviors ────────────────────────────────
    
    /**
     * @brief Reduces the enemy's health. Puts the enemy in the HURT state,
     *        or triggers the death animation if health drops to zero.
     * @param amount : Amount of damage taken.
     */
    void takeDamage(int amount);

    /**
     * @brief Sets the enemy to the DEAD state and starts the death animation timer.
     *        The enemy will be effectively deactivated (active = false) once the
     *        timer has elapsed, via tickDeadState().
     */
    void triggerDeath();

    /**
     * @brief Decrements the HURT and DEAD timers; must be called at the beginning
     *        of each update() method in subclasses.
     * @param dt : Delta time in seconds.
     * @return true if the enemy is in the HURT or DEAD state (subclasses must then
     *         override their own AI logic).
     */
    bool tickTransientStates(float dt);
    
    /**
     * @brief Checks whether the player's position is within the detection cone/radius.
     * @param playerPos Player's position.
     * @return true if detected, false otherwise.
     */
    bool detectsPlayer(Vector2f playerPos) const;

    /** @brief Indicates that the player is visible this tick (resets the persistence timer). */
    void refreshChaseTimer() { chasePersistTimer = Cfg::ENEMY_CHASE_PERSIST; }

    /** @brief Decrement the persistence timer and return to IDLE if it has expired.
 *  @param dt : Delta time.
 *  @return true if the enemy is still in CHASING/ATTACKING mode. */
    bool tickChaseTimer(float dt);

    // Called by LevelController to distribute the gold once
    bool  hasDroppedGold()  const { return goldAlreadyDropped; }
    void  markGoldDropped()       { goldAlreadyDropped = true; }

    bool  getHitThisSlash() const { return hitThisSlash;       }
    void  setHitThisSlash(bool b) { hitThisSlash = b;          }

    // ─── Getters ──────────────────────────────────────────────
    int          getHp()       const { return hp;       }
    int          getMaxHp()    const { return maxHp;    }
    int          getGoldDrop() const { return goldDrop; }
    bool         getisBoss()   const { return isBoss;   }
    EnemyAIState getAIState()  const { return aiState;  }
    Vector2f     getVelocity() const { return velocity; }

    // Delegate Entity::update (without playerPos — no-op for enemies)
    void update(float dt) override { Q_UNUSED(dt); }
};

#endif // ENEMY_H
