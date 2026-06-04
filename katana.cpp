/**
 * @file katana.cpp
 * @brief Implémentation du comportement, des timers et du périmètre d'attaque du Katana.
 */

#include "katana.h"

Katana::Katana()
    : itsDamage(Cfg::KATANA_DAMAGE)
    , itsRange(static_cast<int>(Cfg::KATANA_SLASH_D))
    , itsCooldown(Cfg::KATANA_COOLDOWN)
    , itsReady(true)
    , itsCooldownTimer(0.0f)
    , itsAnimTimer(0.0f)
    , itsSlashZone()
{}

void Katana::update(float dt) {
    // Décompte de la durée de l'animation d'attaque
    if (itsAnimTimer > 0.0f) {
        itsAnimTimer -= dt;
        if (itsAnimTimer < 0.0f) itsAnimTimer = 0.0f;
    }

    // Décompte du verrouillage limitant la fréquence d'attaque (cooldown)
    if (!itsReady) {
        itsCooldownTimer -= dt;
        if (itsCooldownTimer <= 0.0f) {
            itsCooldownTimer = 0.0f;
            itsReady = true;
        }
    }
}

bool Katana::tryAttack(Vector2f playerPos, float playerW, float playerH,
                       FacingDirection dir)
{
    if (!itsReady) return false;

    itsReady         = false;
    itsCooldownTimer = itsCooldown;
    itsAnimTimer     = Cfg::KATANA_ANIM_DUR;

    computeSlashZone(playerPos, playerW, playerH, dir);
    return true;
}

void Katana::computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                               FacingDirection dir)
{
    // Application des règles du Cahier Des Charges (§2.3.2) pour générer l'emprise physique du coup (AABB).
    double px = static_cast<double>(playerPos.x);
    double py = static_cast<double>(playerPos.y);
    double pw = static_cast<double>(playerW);
    double ph = static_cast<double>(playerH);
    double sw = static_cast<double>(Cfg::KATANA_SLASH_W);  // largeur perpendiculaire
    double sd = static_cast<double>(itsRange);             // profondeur (portée)

    switch (dir) {
        case FacingDirection::RIGHT:
            itsSlashZone = QRectF(px + pw,               py + ph/2.0 - sw/2.0, sd, sw);
            break;
        case FacingDirection::LEFT:
            itsSlashZone = QRectF(px - sd,               py + ph/2.0 - sw/2.0, sd, sw);
            break;
        case FacingDirection::DOWN:
            itsSlashZone = QRectF(px + pw/2.0 - sw/2.0, py + ph,               sw, sd);
            break;
        case FacingDirection::UP:
            itsSlashZone = QRectF(px + pw/2.0 - sw/2.0, py - sd,               sw, sd);
            break;
    }
}