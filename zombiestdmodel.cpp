/**
 * @file zombiestdmodel.cpp
 * @brief Implémentation du comportement et de l'intelligence artificielle du zombie de base.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombiestdmodel.h"
#include "playermodel.h"
#include <cstdlib>

// Stats GDD : 20 HP, 90 px/s, 7 dégâts min, portée 400 px, 5 gold
ZombieStdModel::ZombieStdModel(Vector2f pos)
    : Enemy(pos, Cfg::ZOMBIE_MAX_HP, Cfg::ZOMBIE_SPEED,
            Cfg::ZOMBIE_DMG_MIN, Cfg::ZOMBIE_DETECT_R,
            static_cast<float>(Cfg::ENTITY_SIZE),
            static_cast<float>(Cfg::ENTITY_SIZE),
            Cfg::ZOMBIE_GOLD_MIN)
{}

void ZombieStdModel::updateAIState(Vector2f playerPos) {
    if (detectsPlayer(playerPos))
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
    if (!isActive()) return;

    // Décompte du cooldown d'attaque
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

    // Dégâts aléatoires entre ZOMBIE_DMG_MIN et ZOMBIE_DMG_MAX
    int range  = Cfg::ZOMBIE_DMG_MAX - Cfg::ZOMBIE_DMG_MIN;
    int damage = Cfg::ZOMBIE_DMG_MIN + (rand() % (range + 1));

    player->takeDamage(damage);
    attackTimer = Cfg::ZOMBIE_ATK_CD;  // réinitialise le cooldown
}
