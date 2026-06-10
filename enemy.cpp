/**
 * @file enemy.cpp
 * @brief Implémentation des mécaniques communes (dégâts, détection) pour la classe Enemy.
 */

#include "enemy.h"
#include <cmath>

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
      hitThisSlash(false)
{}

void Enemy::takeDamage(int amount) {
    // retire la vie correspondant a amount et vérifie si l'ennemi et mort pour le marquer
    hp -= amount;
    if (hp <= 0) {
        hp     = 0;
        active = false;
    }
}

bool Enemy::detectsPlayer(Vector2f playerPos) const {
    // Calcul de la distance euclidienne entre le centre du sprite de l'ennemi et la position du joueur.
    float dx   = playerPos.x - (position.x + width  / 2.0f);
    float dy   = playerPos.y - (position.y + height / 2.0f);
    float dist = std::sqrt(dx * dx + dy * dy);
    return dist <= detectionRadius;
}
