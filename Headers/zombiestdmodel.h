/**
 * @file zombiestdmodel.h
 * @brief Declaration of the ZombieStdModel class, which handles the zombie's data and logic.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIESTDMODEL_H
#define ZOMBIESTDMODEL_H

#include "enemy.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdModel — standard zombie (iteration 1).
//  AI: IDLE → CHASING (detection) → ATTACKING (contact).
//  Stats (GDD): 20 HP · 90 px/s · 7~12 damage · detection range 400 px.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieStdModel
 * @brief Business logic of a "standard zombie" enemy implementing the Enemy parent class.
 * * Equipped with a simple state machine (IDLE/CHASING) and able to deal random damage to the player.
 */
class ZombieStdModel : public Enemy {
public:
    /**
     * @brief Constructor configuring the zombie's characteristics.
     * * Assigns 20 HP, 90 px/s and a detection zone as per the GDD.
     * @param pos Initial spawn position (X, Y) of the enemy.
     */
    explicit ZombieStdModel(Vector2f pos);

    /**
     * @brief Runs the AI logic (state update, cooldown management) every tick.
     * @param dt Time delta in seconds.
     * @param playerPos Player position used as the target.
     */
    void update(float dt, Vector2f playerPos) override;
    
    /**
     * @brief Triggered on player-enemy collision, dealing 7 to 12 damage.
     * @param player Model pointer targeting the player affected by the hit.
     */
    void attack(PlayerModel* player) override;

private:
    /**
     * @brief Drives the straight-line approach toward the player if detected.
     * @param playerPos Current coordinates of the target.
     * @param dt Delta-time to normalize the movement.
     */
    void chase(Vector2f playerPos, float dt);
    
    /**
     * @brief Analyzes the distance to the player to toggle the enemy's internal state between idle (IDLE) and chase (CHASING).
     * @param playerPos Player coordinates tested against the detection radius.
     */
    void updateAIState(Vector2f playerPos);
};

#endif // ZOMBIESTDMODEL_H
