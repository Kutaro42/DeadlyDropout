/**
 * @file playermodel.cpp
 * @brief Implementation of the comprehensive model that manages player data and behavior.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "playermodel.h"

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
    // ─── Sounds Initialisation ─────────────────────────────────
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

    /*  The cardinal direction is updated via the keyboard only if the katana
    is not currently in motion. During an attack, the direction is locked
    to the angle of attack so that the sprite does not "rotate" while slashing.*/
    if (!katana.isAnimActive()) {
        if (std::abs(inputDir.x) >= std::abs(inputDir.y))
            facingDir = (inputDir.x > 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
        else
            facingDir = (inputDir.y > 0) ? FacingDirection::DOWN : FacingDirection::UP;
    }
}

bool PlayerModel::launchKatana(float angleRad) {
    /* Updates the display orientation BEFORE attempting the attack,
    so that the sprite is properly oriented even if the katana is on cooldown.*/
    FacingDirection newFacing = angleToFacing(angleRad);

    bool launched = katana.tryAttack(position, width, height, angleRad);
    if (launched)
        facingDir = newFacing;

    return launched;
}

void PlayerModel::takeDamage(int amount) {
    bool wasDead = isDead(); 

    hp -= amount;
    if (hp < 0) hp = 0;

    if (!wasDead) {
        if (isDead()) {
            deathSound->play();
        } else {
            damageSound->play();
        }
    }
}

// ---- Angle to degree conversion ----------------------------------------
// Divides the circle into four 90° quadrants centered on each axis.
//   [-45°, +45°)   → RIGHT  (0 rad)
//   [+45°, +135°)  → DOWN   (π/2 rad, Y grows downward in Qt)
//   [+135°, -135°) → LEFT   (±π rad)
//   [-135°, -45°)  → UP
FacingDirection PlayerModel::angleToFacing(float angleRad) {
    // Normalize to the interval [-π, π]
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
