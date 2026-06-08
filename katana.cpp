/**
 * @file katana.cpp
 * @brief Implémentation du comportement, des timers et du périmètre d'attaque du Katana.
 */

#include "katana.h"
#include <cmath>
#include <algorithm>

Katana::Katana()
    : itsDamage(Cfg::KATANA_DAMAGE)
    , itsRange(static_cast<int>(Cfg::KATANA_SLASH_D))
    , itsCooldown(Cfg::KATANA_COOLDOWN)
    , itsReady(true)
    , itsCooldownTimer(0.0f)
    , itsAnimTimer(0.0f)
    , itsSlashZone()
    , itsAttackAngle(0.0f)
{}

void Katana::update(float dt) {
    if (itsAnimTimer > 0.0f) {
        itsAnimTimer -= dt;
        if (itsAnimTimer < 0.0f) itsAnimTimer = 0.0f;
    }
    if (!itsReady) {
        itsCooldownTimer -= dt;
        if (itsCooldownTimer <= 0.0f) {
            itsCooldownTimer = 0.0f;
            itsReady         = true;
        }
    }
}

bool Katana::tryAttack(Vector2f playerPos, float playerW, float playerH,
                       float angleRad)
{
    if (!itsReady) return false;

    itsReady         = false;
    itsCooldownTimer = itsCooldown;
    itsAnimTimer     = Cfg::KATANA_ANIM_DUR;
    itsAttackAngle   = angleRad;

    computeSlashZone(playerPos, playerW, playerH, angleRad);
    return true;
}

void Katana::computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                               float angleRad)
{
    // ── Centre de la hitbox du joueur ────────────────────────────────────────
    double cx = static_cast<double>(playerPos.x) + playerW  / 2.0;
    double cy = static_cast<double>(playerPos.y) + playerH / 2.0;

    double cosA = std::cos(static_cast<double>(angleRad));
    double sinA = std::sin(static_cast<double>(angleRad));

    // ── Demi-dimensions du rectangle de slash en coordonnées locales ─────────
    // L'axe local X est l'axe d'attaque (profondeur), Y est l'axe perpendiculaire.
    // Le rectangle local s'étend de [0, depth] sur X et [-halfW, +halfW] sur Y.
    double depth  = static_cast<double>(itsRange);
    double halfW  = static_cast<double>(Cfg::KATANA_SLASH_W) / 2.0;

    // Décalage du bord proche : on part du bord de la hitbox joueur dans la
    // direction de l'attaque, pas du centre, pour éviter de frapper derrière soi.
    double halfDiag = std::sqrt(static_cast<double>(playerW)  * playerW / 4.0
                               + static_cast<double>(playerH) * playerH / 4.0);
    double startOffset = halfDiag;

    // ── 4 coins du rectangle de slash en coordonnées monde (après rotation) ──
    // Coins locaux : (startOffset, -halfW), (startOffset, +halfW),
    //               (startOffset+depth, -halfW), (startOffset+depth, +halfW)
    const double localX[4] = { startOffset,         startOffset,
                                startOffset + depth, startOffset + depth };
    const double localY[4] = { -halfW,  halfW,  -halfW,  halfW };

    double minX =  1e18, minY =  1e18;
    double maxX = -1e18, maxY = -1e18;

    for (int i = 0; i < 4; ++i) {
        // Rotation : x' = cosA*lx - sinA*ly + cx
        //            y' = sinA*lx + cosA*ly + cy
        double wx = cosA * localX[i] - sinA * localY[i] + cx;
        double wy = sinA * localX[i] + cosA * localY[i] + cy;
        minX = std::min(minX, wx);
        minY = std::min(minY, wy);
        maxX = std::max(maxX, wx);
        maxY = std::max(maxY, wy);
    }

    itsSlashZone = QRectF(minX, minY, maxX - minX, maxY - minY);
}
