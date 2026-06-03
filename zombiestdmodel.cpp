#include "zombiestdmodel.h"
#include <cstdlib>

// Spécifications du GDD : 20 HP, Vitesse 90, Dégâts 7-12, Détection 400px
ZombieStdModel::ZombieStdModel(Vector2f pos) 
    : Enemy(pos, 20, 90.0f, 7, 400.0f, 64.0f, 64.0f, 5) {}

void ZombieStdModel::update(float dt, Vector2f playerPos) {
    // Méthode générique inutilisée, on passe par updateEnemy
}

void ZombieStdModel::updateEnemy(float dt, Vector2f playerPos) {
    if (!isActive()) return;

    if (attackTimer > 0) {
        attackTimer -= dt;
    }

    // Si le joueur est dans la zone de détection, on le suit à l'aide du vecteur de vitesse
    if (detectsPlayer(playerPos)) {
        Vector2f direction = (playerPos - position).normalized();
        velocity = direction;
        position = position + (velocity * speed * dt);
    } else {
        velocity = Vector2f(0, 0);
    }
}

void ZombieStdModel::attack(Player* p) {
    if (attackTimer <= 0 && p) {
        // Dégâts de base + variation aléatoire définie par le GDD
        int randomDamage = baseDamage + (std::rand() % 6); 
        p->takeDamage(randomDamage);
        attackTimer = attackCooldown; // Réinitialisation du cooldown à 1.5s
    }
}
