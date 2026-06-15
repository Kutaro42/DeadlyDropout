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
    , katana()
{
    // ─── Initialisation des sons ─────────────────────────────────
    damageSound = new QSoundEffect();
    damageSound->setSource(QUrl("qrc:/images/Sound/damage.wav"));
    damageSound->setVolume(0.5f);

    deathSound = new QSoundEffect();
    deathSound->setSource(QUrl("qrc:/images/Sound/death.wav"));
    deathSound->setVolume(0.5f);
}

PlayerModel::~PlayerModel()
{
    if (damageSound) {
        damageSound->deleteLater();
    }
    if (deathSound) {
        deathSound->deleteLater();
    }
}

void PlayerModel::update(float dt) {
    katana.update(dt);
    gun.update(dt);
}

bool PlayerModel::getBoostState() {
    return speedBoosted;
}

void PlayerModel::setHasKey(bool key) {
    hasKey = key;
}

void PlayerModel::move(Vector2f inputDir, float dt) {
    if (inputDir.length() == 0.0f) {
        moving = false;
        return;
    }
    moving = true;
    Vector2f dir = inputDir.normalized();
    position = position + (dir * speed * dt);

    // La direction cardinale n'est mise à jour par le clavier que si le katana
    // n'est pas en train d'animer (l'attaque a la priorité sur le regard).
    if (!katana.isAnimActive()) {
        if (std::abs(inputDir.x) >= std::abs(inputDir.y))
            facingDir = (inputDir.x > 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
        else
            facingDir = (inputDir.y > 0) ? FacingDirection::DOWN : FacingDirection::UP;
    }
}

bool PlayerModel::launchKatana(float angleRad) {
    // Met à jour la direction cardinale d'affichage AVANT de tenter l'attaque,
    // pour que le sprite soit orienté même si le katana est en cooldown.
    FacingDirection newFacing = angleToFacing(angleRad);

    bool launched = katana.tryAttack(position, width, height, angleRad);
    if (launched)
        facingDir = newFacing;

    return launched;
}

void PlayerModel::takeDamage(int amount) {
    bool wasDead = isDead(); // On vérifie si le joueur était déjà mort avant l'impact

    hp -= amount;
    if (hp < 0) hp = 0;

    // On ne joue les sons que si le joueur n'était pas déjà mort
    if (!wasDead) {
        if (isDead()) {
            deathSound->play(); // Le coup de grâce
        } else {
            damageSound->play(); // Un simple dégât
        }
    }
}

// ─── Conversion angle → cardinal ─────────────────────────────────────────────
// Découpe le cercle en 4 quadrants de 90° centrés sur chaque axe.
//   [-45°, +45°)   → RIGHT  (0 rad)
//   [+45°, +135°)  → DOWN   (π/2 rad, car Y croît vers le bas)
//   [+135°, -135°) → LEFT   (±π rad)
//   [-135°, -45°)  → UP
FacingDirection PlayerModel::angleToFacing(float angleRad) {
    // Normalise dans [-π, π]
    const float pi = static_cast<float>(M_PI);
    while (angleRad >  pi) angleRad -= 2.0f * pi;
    while (angleRad < -pi) angleRad += 2.0f * pi;

    const float quarter = pi / 4.0f;   // 45°

    if (angleRad >= -quarter && angleRad < quarter)
        return FacingDirection::RIGHT;
    if (angleRad >= quarter && angleRad < 3.0f * quarter)
        return FacingDirection::DOWN;
    if (angleRad >= -3.0f * quarter && angleRad < -quarter)
        return FacingDirection::UP;
    return FacingDirection::LEFT;
}
