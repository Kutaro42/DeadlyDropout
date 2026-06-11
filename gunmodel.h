/**
 * @file gunmodel.h
 * @brief Modèle du pistolet semi-automatique du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#ifndef GUNMODEL_H
#define GUNMODEL_H

#include <QVector>
#include "bulletmodel.h"
#include "config.h"
#include "vector2f.h"
#include <QSoundEffect>

/**
 * @class GunModel
 * @brief Gère la logique de tir, le chargeur, la réserve de munitions et le cooldown (GDD).
 *
 * Règles GDD :
 *  - Semi-auto : 1 clic = 1 tir, cooldown 0.5 s.
 *  - Chargeur (mag) + réserve générale (reserve).
 *  - Touche R pour recharger : complète le chargeur depuis la réserve.
 *  - Dégâts : 12 base + aléatoire 2-5 + critique 2 % × 2.
 *  - Vitesse balle : 500 px/s.
 *
 * Les BulletModel créés sont possédés par GunModel et mis à jour à chaque tick.
 * LevelController récupère getBullets() pour détecter les collisions.
 */
class GunModel : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur.
     * @param magSize     Taille du chargeur (Cfg::GUN_MAG_SIZE par défaut).
     * @param reserveSize Munitions en réserve initiale (2 × magSize par défaut).
     */
    explicit GunModel(int magSize     = Cfg::GUN_MAG_SIZE,
                      int reserveSize = Cfg::GUN_MAG_SIZE * 2);

    ~GunModel();

    // ─── Mise à jour (cooldown + déplacement balles) ──────────
    /**
     * @brief Avance les timers et les positions de toutes les balles actives.
     * @param dt Delta-time en secondes.
     */
    void update(float dt);

    // ─── Actions joueur ───────────────────────────────────────
    /**
     * @brief Tente de tirer une balle si le cooldown est prêt et le chargeur non vide.
     * @param muzzlePos  Position de la bouche du pistolet dans la scène.
     * @param targetPos  Position du curseur souris dans la scène (cible).
     * @return true si un tir a été effectué.
     */
    bool tryShoot(Vector2f muzzlePos, Vector2f targetPos);

    /**
     * @brief Recharge le chargeur depuis la réserve générale (touche R, GDD).
     * Les balles déjà dans le chargeur sont conservées, on complète jusqu'à magSize.
     */
    void reload();

    // ─── Accès aux projectiles ────────────────────────────────
    QVector<BulletModel*>& getBullets() { return bullets; }

    /**
     * @brief Supprime de la liste les balles inactives et libère leur mémoire.
     * Appelé par LevelController après traitement des collisions.
     */
    void pruneInactiveBullets();

    // ─── Getters HUD ──────────────────────────────────────────
    int  getMag()      const { return mag;      } ///< Balles restantes dans le chargeur.
    int  getMagSize()  const { return magSize;  } ///< Taille maximale du chargeur.
    int  getReserve()  const { return reserve;  } ///< Balles en réserve générale.
    bool isReady()     const { return shootTimer <= 0.0f; } ///< Cooldown écoulé ?
    float getCooldownRatio() const {
        return (shootCooldown > 0.0f) ? (shootTimer / shootCooldown) : 0.0f;
    } ///< Ratio cooldown gun : 1.0=vient de tirer, 0.0=prêt.

    // ─── Setters upgrades ─────────────────────────────────────
    void addReserve(int amount)        { reserve += amount; }
    void setShootCooldown(float cd)    { shootCooldown = cd; }
    void setBaseDamage(int d)          { baseDamage = d; }
    void setCritChance(float c)        { critChance = c; }

private:
    QVector<BulletModel*> bullets;   ///< Balles actuellement en vol.
    QSoundEffect *emptyMagSound;

    int   magSize;       ///< Capacité maximale du chargeur.
    int   mag;           ///< Balles restantes dans le chargeur.
    int   reserve;       ///< Munitions en réserve générale.

    float shootCooldown; ///< Durée entre deux tirs (secondes).
    float shootTimer;    ///< Temps restant avant le prochain tir autorisé.

    int   baseDamage;    ///< Dégâts de base (avant bonus et critique).
    float critChance;    ///< Probabilité de coup critique [0..1].

    /// Calcule les dégâts d'une balle (base + bonus aléatoire + critique éventuel).
    int computeDamage() const;
};

#endif // GUNMODEL_H
