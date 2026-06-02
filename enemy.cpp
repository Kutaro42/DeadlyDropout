#include "enemy.h"

Enemy::Enemy(Vector2f pos, int maxHp, float spd, int dmg, float det, float w, float h, int goldDrop)
    : Entity(pos, w, h), hp(maxHp), maxHp(maxHp), speed(spd), baseDamage(dmg), 
      attackCooldown(1.5f), attackTimer(0.0f), detectionRadius(det), baseGoldDrop(goldDrop) {
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
        setActive(false); // Destruction de l'entité du niveau
    }
}

bool Enemy::isDead() const {
    return hp <= 0;
}

int Enemy::getGoldDrop() const {
    return baseGoldDrop;
}

bool Enemy::detectsPlayer(Vector2f playerPos) const {
    float distance = (playerPos - position).length();
    return distance <= detectionRadius;
}