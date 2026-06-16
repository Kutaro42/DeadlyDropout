/**
 * @file zombiestdmodel.cpp
 * @brief Implementation of the basic zombie's behavior and artificial intelligence.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombiestdmodel.h"
#include "playermodel.h"

// GDD stats: 20 HP, 90 px/s, 7 min damage, 400 px detection range, 5 gold
ZombieStdModel::ZombieStdModel(Vector2f pos)
    : Enemy(pos, Cfg::ZOMBIE_MAX_HP, Cfg::ZOMBIE_SPEED,
            Cfg::ZOMBIE_DMG_MIN, Cfg::ZOMBIE_DETECT_R,
            static_cast<float>(Cfg::ENTITY_SIZE),
            static_cast<float>(Cfg::ENTITY_SIZE),
            Cfg::ZOMBIE_GOLD_MIN)
{}

void ZombieStdModel::updateAIState(Vector2f playerPos) {
    // The decision to switch to CHASING is driven by
    // chasePersistTimer managed in LevelController.
    // We only revert to IDLE when the timer has expired.
    if (chasePersistTimer > 0.0f)
        aiState = EnemyAIState::CHASING;
    else
        aiState = EnemyAIState::IDLE;
}

void ZombieStdModel::chase(Vector2f playerPos, float dt) {
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

void ZombieStdModel::update(float dt, Vector2f playerPos) {
    if (!isActive() && aiState != EnemyAIState::DEAD) return;

    // Handle transient states (HURT, DEAD) — short-circuits the AI if active.
    if (tickTransientStates(dt)) return;

    // Count down the attack cooldown
    if (attackTimer > 0.0f)
        attackTimer -= dt;

    updateAIState(playerPos);

    if (aiState == EnemyAIState::CHASING)
        chase(playerPos, dt);
    else
        velocity = Vector2f(0.0f, 0.0f);
}

void ZombieStdModel::attack(PlayerModel* player) {
    if (!player || attackTimer > 0.0f) return;

    // Random damage between ZOMBIE_DMG_MIN and ZOMBIE_DMG_MAX
    int range  = Cfg::ZOMBIE_DMG_MAX - Cfg::ZOMBIE_DMG_MIN;
    int damage = Cfg::ZOMBIE_DMG_MIN + (rand() % (range + 1));

    player->takeDamage(damage);
    attackTimer = Cfg::ZOMBIE_ATK_CD;  // resets the cooldown
}
