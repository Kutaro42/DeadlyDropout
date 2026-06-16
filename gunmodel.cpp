/**
 * @file gunmodel.cpp
 * @brief Implementation of the semi-automatic pistol logic.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#include "gunmodel.h"

GunModel::GunModel(int magSz, int reserveSz)
    : magSize(magSz)
    , mag(magSz)
    , reserve(reserveSz)
    , shootCooldown(Cfg::GUN_SHOOT_CD)
    , shootTimer(0.0f)
    , baseDamage(Cfg::BULLET_DAMAGE)
    , critChance(Cfg::BULLET_CRIT_CHANCE)
{
    // Sound effects are initialised here so each GunModel instance has its own
    // independent QSoundEffect objects, preventing concurrent-playback conflicts
    // if multiple gun instances ever coexist.
    emptyMagSound = new QSoundEffect();
    emptyMagSound->setSource(QUrl("qrc:/images/Sound/click.wav"));
    emptyMagSound->setVolume(0.5f);
    gunFireSound = new QSoundEffect();
    gunFireSound->setSource(QUrl("qrc:/images/Sound/gunFireSound.wav"));
    gunFireSound->setVolume(0.5f);
}

GunModel::~GunModel()
{
    for (BulletModel* b : bullets) delete b;
    bullets.clear();
    // deleteLater() is used (instead of delete) because QSoundEffect may be
    // in the middle of playing when the destructor runs; deleteLater() defers
    // deletion to the Qt event loop, preventing a use-after-free crash.
    if (emptyMagSound) {
        emptyMagSound->deleteLater();
    }
    if (gunFireSound) {
        gunFireSound->deleteLater();
    }
}

// ─── Update: cooldown + bullet movement ─────────────────────────────
void GunModel::update(float dt)
{
    if (shootTimer > 0.0f) {
        shootTimer -= dt;
        if (shootTimer < 0.0f) shootTimer = 0.0f;
    }

    for (BulletModel* b : bullets)
        if (b->isActive()) b->update(dt);
}

// ─── Damage calculation (GDD §ammo rules) ───────────────────────────
int GunModel::computeDamage() const
{
    int range  = Cfg::BULLET_BONUS_MAX - Cfg::BULLET_BONUS_MIN;
    int bonus  = Cfg::BULLET_BONUS_MIN + (rand() % (range + 1));
    int dmg    = baseDamage + bonus;

    // Critical hit: 2 % chance (GDD base), ×2 damage multiplier.
    float roll = static_cast<float>(rand()) / RAND_MAX;
    if (roll < critChance)
        dmg = static_cast<int>(dmg * Cfg::BULLET_CRIT_MULT);

    return dmg;
}

// ─── Fire ────────────────────────────────────────────────────────────
bool GunModel::tryShoot(Vector2f muzzlePos, Vector2f targetPos)
{
    if (shootTimer > 0.0f) return false;   // Cooldown not yet elapsed
    if (mag <= 0 && !emptyMagSound->isPlaying()) emptyMagSound->play();
    if (mag <= 0)           return false;   // Magazine empty

    Vector2f dir = (targetPos - muzzlePos).normalized();
    bullets.append(new BulletModel(muzzlePos, dir, computeDamage()));
    gunFireSound->play();
    --mag;
    shootTimer = shootCooldown;
    return true;
}

// ─── Reload (GDD §ammo rules) ────────────────────────────────────────
void GunModel::reload()
{
    int needed = magSize - mag;        // Free slots in the magazine
    if (needed <= 0 || reserve <= 0) return;

    // Transfer only as many rounds as needed (or available), preserving
    // existing magazine rounds as specified in the GDD ammo reload rule.
    int transferred = (reserve >= needed) ? needed : reserve;
    mag     += transferred;
    reserve -= transferred;
}

// ─── Memory cleanup ──────────────────────────────────────────────────
void GunModel::pruneInactiveBullets()
{
    for (int i = bullets.size() - 1; i >= 0; --i) {
        if (!bullets[i]->isActive()) {
            delete bullets[i];
            bullets.removeAt(i);
        }
    }
}