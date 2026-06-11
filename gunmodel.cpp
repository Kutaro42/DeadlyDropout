/**
 * @file gunmodel.cpp
 * @brief Implémentation de la logique du pistolet semi-automatique.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#include "gunmodel.h"
#include <cstdlib>
#include <cmath>

GunModel::GunModel(int magSz, int reserveSz)
    : magSize(magSz)
    , mag(magSz)
    , reserve(reserveSz)
    , shootCooldown(Cfg::GUN_SHOOT_CD)
    , shootTimer(0.0f)
    , baseDamage(Cfg::BULLET_DAMAGE)
    , critChance(Cfg::BULLET_CRIT_CHANCE)
{
    emptyMagSound = new QSoundEffect(this);
    emptyMagSound->setSource(QUrl("qrc:/images/Sound/click.wav"));
    emptyMagSound->setVolume(0.5f);
}

GunModel::~GunModel()
{
    for (BulletModel* b : bullets) delete b;
    bullets.clear();
}

// ─── Update : cooldown + déplacement des balles ───────────────────
void GunModel::update(float dt)
{
    if (shootTimer > 0.0f) {
        shootTimer -= dt;
        if (shootTimer < 0.0f) shootTimer = 0.0f;
    }

    for (BulletModel* b : bullets)
        if (b->isActive()) b->update(dt);
}

// ─── Calcul des dégâts (GDD §règles munitions) ───────────────────
int GunModel::computeDamage() const
{
    int range  = Cfg::BULLET_BONUS_MAX - Cfg::BULLET_BONUS_MIN;
    int bonus  = Cfg::BULLET_BONUS_MIN + (rand() % (range + 1));
    int dmg    = baseDamage + bonus;

    // Coup critique : 2 % de chance, ×2 dégâts
    float roll = static_cast<float>(rand()) / RAND_MAX;
    if (roll < critChance)
        dmg = static_cast<int>(dmg * Cfg::BULLET_CRIT_MULT);

    return dmg;
}

// ─── Tir ─────────────────────────────────────────────────────────
bool GunModel::tryShoot(Vector2f muzzlePos, Vector2f targetPos)
{
    if (shootTimer > 0.0f) return false;   // cooldown non écoulé
    if (mag <= 0 && !emptyMagSound->isPlaying())          emptyMagSound->play();
    if (mag <= 0)           return false;   // chargeur vide

    Vector2f dir = (targetPos - muzzlePos).normalized();
    bullets.append(new BulletModel(muzzlePos, dir, computeDamage()));

    --mag;
    shootTimer = shootCooldown;
    return true;
}

// ─── Rechargement (GDD §règles munitions) ────────────────────────
void GunModel::reload()
{
    int needed = magSize - mag;        // places libres dans le chargeur
    if (needed <= 0 || reserve <= 0) return;

    int transferred = (reserve >= needed) ? needed : reserve;
    mag     += transferred;
    reserve -= transferred;
}

// ─── Nettoyage mémoire ────────────────────────────────────────────
void GunModel::pruneInactiveBullets()
{
    for (int i = bullets.size() - 1; i >= 0; --i) {
        if (!bullets[i]->isActive()) {
            delete bullets[i];
            bullets.removeAt(i);
        }
    }
}
