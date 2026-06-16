/**
 * @file enemy.cpp
 * @brief Implementation of common mechanics (damage, detection) for the Enemy class.
 */

#include "enemy.h"

Enemy::Enemy(Vector2f pos, int maxHp, float spd, int dmg,
             float detRadius, float w, float h, int gold)
    : Entity(pos, w, h),
      hp(maxHp), maxHp(maxHp),
      speed(spd), baseDamage(dmg),
      attackCooldown(Cfg::ZOMBIE_ATK_CD), attackTimer(0.0f),
      detectionRadius(detRadius),
      goldDrop(gold), goldAlreadyDropped(false),
      aiState(EnemyAIState::IDLE),
      velocity(0.0f, 0.0f),
      hitThisSlash(false),
      hurtTimer(0.0f),
      deadTimer(0.0f),
      chasePersistTimer(0.0f)
{}

void Enemy::takeDamage(int amount) {
    // Ignore damage if you're already dying
    if (aiState == EnemyAIState::DEAD) return;

    hp -= amount;
    if (hp <= 0) {
        hp = 0;
        triggerDeath();
    } else {
        // Switch to HURT mode (stops CHASING/ATTACKING)
        aiState   = EnemyAIState::HURT;
        hurtTimer = Cfg::ENEMY_HURT_DUR;
        velocity  = Vector2f(0.0f, 0.0f);
    }
}

void Enemy::triggerDeath() {
    aiState   = EnemyAIState::DEAD;
    deadTimer = Cfg::ENEMY_DEAD_DUR;
    velocity  = Vector2f(0.0f, 0.0f);
    onDeath();
}

bool Enemy::tickTransientStates(float dt) {
    if (aiState == EnemyAIState::DEAD) {
        deadTimer -= dt;
        if (deadTimer <= 0.0f) {
            deadTimer = 0.0f;
            active    = false; // Actual deactivation after the animation
        }
        return true;
    }

    if (aiState == EnemyAIState::HURT) {
        hurtTimer -= dt;
        if (hurtTimer <= 0.0f) {
            hurtTimer = 0.0f;
            aiState   = EnemyAIState::CHASING; // Back to CHASING after
        }
        return true;
    }

    return false;
}

bool Enemy::detectsPlayer(Vector2f playerPos) const {
    // Calculate the Euclidean distance between the center of the enemy sprite and the player's position.
    float dx   = playerPos.x - (position.x + width  / 2.0f);
    float dy   = playerPos.y - (position.y + height / 2.0f);
    float dist = std::sqrt(dx * dx + dy * dy);
    return dist <= detectionRadius;
}

bool Enemy::tickChaseTimer(float dt) {
    if (chasePersistTimer <= 0.0f) return false;
    chasePersistTimer -= dt;
    if (chasePersistTimer < 0.0f) chasePersistTimer = 0.0f;
    return chasePersistTimer > 0.0f;
}
