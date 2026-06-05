/**
 * @file playermodel.cpp
 * @brief Implémentation du modèle complet gérant les données et comportements du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "playermodel.h"
#include <cmath>

PlayerModel::PlayerModel(Vector2f startPos)
    : Entity(startPos,
             static_cast<float>(Cfg::ENTITY_SIZE),
             static_cast<float>(Cfg::ENTITY_SIZE))
    , hp(Cfg::PLAYER_MAX_HP)
    , maxHp(Cfg::PLAYER_MAX_HP)
    , speed(Cfg::PLAYER_SPEED)
    , speedBoosted(false)
    , gold(0)
    , hasKey(false)
    , moving(false)
    , facingDir(FacingDirection::DOWN)
    , katana()      // initialisé avec les constantes Cfg
{}

void PlayerModel::update(float dt) {
    katana.update(dt);  // délègue timers cooldown + anim à Katana
}

bool PlayerModel::getBoostState() {
    return speedBoosted;  // délègue timers cooldown + anim à Katana
}

void PlayerModel::move(Vector2f inputDir, float dt) {
    if (inputDir.length() == 0.0f) {
        moving = false;
        return;
    }

    moving = true;
    Vector2f dir = inputDir.normalized();
    position = position + (dir * speed * dt);

    // FacingDirection : axe dominant du vecteur d'entrée
    if (std::abs(inputDir.x) >= std::abs(inputDir.y))
        facingDir = (inputDir.x > 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
    else
        facingDir = (inputDir.y > 0) ? FacingDirection::DOWN  : FacingDirection::UP;
}

// ─── Lance l'attaque — reste ici car c'est une action du joueur ──────────────
bool PlayerModel::launchKatana() {
    return katana.tryAttack(position, width, height, facingDir);
}

void PlayerModel::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}
