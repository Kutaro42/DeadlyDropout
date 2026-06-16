/**
 * @file zombielaunchermodel.h
 * @brief Description of the rubble-throwing zombie model.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIELAUNCHERMODEL_H
#define ZOMBIELAUNCHERMODEL_H

#include "enemy.h"
#include "rubblemodel.h"
#include <QVector>
#include "playermodel.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieLauncherModel — launcher zombie (iteration 2).
//
//  AI:
//    IDLE      → no action (outside detection range)
//    CHASING   → player detected BUT out of firing range:
//                the enemy chases to get closer
//    ATTACKING → player within firing range:
//                the enemy stays STILL and fires a rubble
//                every LAUNCHER_SHOOT_CD seconds
//
//  Projectile ownership:
//    Created RubbleModel instances are stored in an internal vector.
//    LevelController retrieves them via getRubbles() to update them
//    and detect their collisions with the player/walls.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieLauncherModel
 * @brief Ranged enemy that fires rubble at the player.
 */
class ZombieLauncherModel : public Enemy {
public:
    /**
     * @brief Constructor.
     * @param pos Spawn position in the level.
     */
    explicit ZombieLauncherModel(Vector2f pos);
    ~ZombieLauncherModel() override {
        for (RubbleModel* r : rubbles) delete r;
        rubbles.clear();
    }

    // ---- Enemy interface --------------------------------------------
    /**
     * @brief Updates the AI and projectiles every tick.
     * @param dt        Delta-time in seconds.
     * @param playerPos Current player position.
     */
    void update(float dt, Vector2f playerPos) override;

    /**
     * @brief Melee attack (not used in practice: the enemy fires at range).
     * @param player Target player.
     */
    void attack(PlayerModel* player) override;

    // ---- Projectile access -----------------------------------------
    /**
     * @brief Returns the list of active rubble projectiles fired by this enemy.
     * @return Reference to the vector of RubbleModel*.
     */
    QVector<RubbleModel*>& getRubbles() { return rubbles; }

    /**
     * @brief Removes from the list any rubble that is no longer active.
     * Must be called by LevelController after processing collisions.
     */
    void pruneInactiveRubbles();

private:
    QVector<RubbleModel*> rubbles;  ///< In-flight rubble fired by this enemy.
    float shootTimer;               ///< Time remaining before the next allowed shot.

    // ---- AI sub-methods -----------------------------------------
    /**
     * @brief Computes the distance to the player and updates the AI state.
     * @param playerPos Player position.
     * @return Euclidean distance between the enemy and the player.
     */
    float updateAIState(Vector2f playerPos);

    /**
     * @brief Moves the enemy toward the player.
     * @param playerPos Target.
     * @param dt        Delta-time.
     */
    void chase(Vector2f playerPos, float dt);

    /**
     * @brief Creates a new RubbleModel directed toward the player.
     * @param playerPos Firing target.
     */
    void shoot(Vector2f playerPos);
};

#endif // ZOMBIELAUNCHERMODEL_H
