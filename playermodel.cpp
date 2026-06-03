#include "playermodel.h"
#include <cmath>

PlayerModel::PlayerModel(Vector2f startPos)
    : Entity(startPos,
             static_cast<float>(Cfg::ENTITY_SIZE),
             static_cast<float>(Cfg::ENTITY_SIZE)),
      hp(Cfg::PLAYER_MAX_HP), maxHp(Cfg::PLAYER_MAX_HP),
      speed(Cfg::PLAYER_SPEED),
      gold(0), hasKey(false), moving(false),
      katanaDamage(Cfg::KATANA_DAMAGE),
      katanaSlashW(Cfg::KATANA_SLASH_W),
      katanaSlashD(Cfg::KATANA_SLASH_D),
      katanaCooldownTimer(0.0f), katanaAnimTimer(0.0f),
      katanaActive(false),
      facingDir(FacingDirection::DOWN)
{}

void PlayerModel::update(float dt) {
    // ─── Décompte timer animation katana ──────────────────────
    if (katanaActive) {
        katanaAnimTimer -= dt;
        if (katanaAnimTimer <= 0.0f) {
            katanaActive    = false;
            katanaAnimTimer = 0.0f;
        }
    }
    // ─── Décompte cooldown katana ─────────────────────────────
    if (katanaCooldownTimer > 0.0f)
        katanaCooldownTimer -= dt;
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

bool PlayerModel::launchKatana() {
    if (katanaCooldownTimer > 0.0f) return false;

    katanaActive        = true;
    katanaAnimTimer     = Cfg::KATANA_ANIM_DUR;
    katanaCooldownTimer = Cfg::KATANA_COOLDOWN;
    updateSlashZone();
    return true;
}

void PlayerModel::updateSlashZone() {
    // Centre du sprite joueur
    double px = static_cast<double>(position.x);
    double py = static_cast<double>(position.y);
    double pw = static_cast<double>(width);
    double ph = static_cast<double>(height);
    double sw = static_cast<double>(katanaSlashW);
    double sd = static_cast<double>(katanaSlashD);

    switch (facingDir) {
        case FacingDirection::RIGHT:
            slashZone = QRectF(px + pw,           py + ph/2.0 - sw/2.0, sd, sw);
            break;
        case FacingDirection::LEFT:
            slashZone = QRectF(px - sd,            py + ph/2.0 - sw/2.0, sd, sw);
            break;
        case FacingDirection::DOWN:
            slashZone = QRectF(px + pw/2.0 - sw/2.0, py + ph,            sw, sd);
            break;
        case FacingDirection::UP:
            slashZone = QRectF(px + pw/2.0 - sw/2.0, py - sd,            sw, sd);
            break;
    }
}

void PlayerModel::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}
