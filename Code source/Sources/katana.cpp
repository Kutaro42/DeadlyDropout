/**
 * @file katana.cpp
 * @brief Implementation of Katana: behaviour, timers and attack-zone computation.
 */

#include "Headers/katana.h"
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
{
    // ─── Slash sound initialisation ─────────────────────────────────
    itsSlashSound = new QSoundEffect();
    // Adjust the filename if the asset is stored under a different name.
    itsSlashSound->setSource(QUrl("qrc:/images/Sound/katana.wav"));
    itsSlashSound->setVolume(0.5f);
}

Katana::~Katana()
{
    if (itsSlashSound) {
        itsSlashSound->deleteLater();
    }
}

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
    if (!itsReady) return false; // Weapon still on cooldown; silently reject

    // Commit the attack: reset cooldown and animation timers.
    itsReady         = false;
    itsCooldownTimer = itsCooldown;
    itsAnimTimer     = Cfg::KATANA_ANIM_DUR;
    itsAttackAngle   = angleRad;

    // ─── Play attack sound ──────────────────────────────────────────
    itsSlashSound->play();

    // Compute the slash collision zone (remainder of code unchanged).
    computeSlashZone(playerPos, playerW, playerH, angleRad);
    return true;
}

void Katana::computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                               float angleRad)
{
    // ── Player hitbox centre ─────────────────────────────────────────────────
    double cx = static_cast<double>(playerPos.x) + playerW  / 2.0;
    double cy = static_cast<double>(playerPos.y) + playerH / 2.0;

    double cosA = std::cos(static_cast<double>(angleRad));
    double sinA = std::sin(static_cast<double>(angleRad));

    // ── Slash rectangle dimensions in local space ────────────────────────────
    // Local X = attack axis (depth), local Y = perpendicular axis.
    // The rectangle spans [0, depth] on X and [-halfW, +halfW] on Y.
    double depth  = static_cast<double>(itsRange);
    double halfW  = static_cast<double>(Cfg::KATANA_SLASH_W) / 2.0;

    // Offset the near edge of the slash to the boundary of the player hitbox
    // (not the centre) so the weapon cannot hit enemies behind the player.
    double halfDiag = std::sqrt(static_cast<double>(playerW)  * playerW / 4.0
                               + static_cast<double>(playerH) * playerH / 4.0);
    double startOffset = halfDiag;

    // ── Rotate the 4 rectangle corners to world space ─────────────────────────
    // Local corners: (startOffset, ±halfW), (startOffset+depth, ±halfW)
    const double localX[4] = { startOffset,         startOffset,
                                startOffset + depth, startOffset + depth };
    const double localY[4] = { -halfW,  halfW,  -halfW,  halfW };

    double minX =  1e18, minY =  1e18;
    double maxX = -1e18, maxY = -1e18;

    for (int i = 0; i < 4; ++i) {
        // Standard 2D rotation around the player centre:
        // x' = cosA*lx - sinA*ly + cx
        // y' = sinA*lx + cosA*ly + cy
        double wx = cosA * localX[i] - sinA * localY[i] + cx;
        double wy = sinA * localX[i] + cosA * localY[i] + cy;
        minX = std::min(minX, wx);
        minY = std::min(minY, wy);
        maxX = std::max(maxX, wx);
        maxY = std::max(maxY, wy);
    }

    // Store the AABB enclosing the rotated rectangle for use by LevelController.
    itsSlashZone = QRectF(minX, minY, maxX - minX, maxY - minY);
}
