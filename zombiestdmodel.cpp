#include "zombiestdmodel.h"
#include <cstdlib> // Pour le rand des dégâts

// D'après le GDD : 20 HP, 90 px/s, 7~12 dégâts, hitbox 64x64, portée de détection 400px
ZombieStdModel::ZombieStdModel(Vector2f pos)
    : Enemy(pos, 20, 90.0f, 7, 400.0f, 64.0f, 64.0f, 5) { // 5 gold par défaut
}

void ZombieStdModel::update(float dt, Vector2f playerPos) {
    if (!isActive()) return;

    // Gestion du cooldown d'attaque (1.5s d'après le GDD)
    if (attackTimer > 0) {
        attackTimer -= dt;
    }

    // Déplacement et suivi du joueur
    if (detectsPlayer(playerPos)) {
        Vector2f direction = (playerPos - position).normalized();
        velocity = direction * speed;
        position = position + (velocity * dt);
    } else {
        velocity = Vector2f(0, 0); // Reste immobile s'il ne détecte pas le joueur
    }
}

void ZombieStdModel::attack(Player* p) {
    // Si l'attaque est prête et que les hitbox se touchent
    if (attackTimer <= 0) {
        // Dégâts aléatoires entre 7 et 12 (GDD)
        int randomDamage = baseDamage + (rand() % 6); 
        // p->takeDamage(randomDamage); // Décommenter quand Player sera implémenté
        attackTimer = attackCooldown; // Réinitialise le timer à 1.5s
    }
}