/**
 * @file playermodel.h
 * @brief Déclaration de la classe PlayerModel, le composant Model central du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entity.h"
#include "katana.h"
#include "gunmodel.h"
#include "config.h"
#include <cmath>
#include <QSoundEffect>

// ═══════════════════════════════════════════════════════════════════
//  PlayerModel — état et logique du joueur (couche Model, MVC).
//  Délègue tout ce qui concerne l'arme à l'objet Katana.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerModel
 * @brief Modèle encapsulant les propriétés, ressources et actions du joueur.
 *
 * Gère les PV, la vitesse, l'or, les clés et l'orientation cardinale.
 * Les actions du Katana sont déléguées à l'instance interne.
 *
 * L'orientation cardinale (facingDir) est maintenant déterminée soit par
 * le déplacement clavier, soit par l'angle d'attaque transmis à launchKatana().
 * La règle de priorité est : si le joueur attaque, la direction cardinale est
 * celle la plus proche de l'angle d'attaque, sinon c'est le clavier.
 */
class PlayerModel : public Entity {
private:
    int             hp;
    int             maxHp;
    float           speed;
    bool            speedBoosted;
    int             gold;
    bool            hasKey;
    bool            moving;

    FacingDirection facingDir; ///< Direction cardinale courante (clavier ou attaque).

    Katana          katana;
    GunModel        gun;QSoundEffect* damageSound;
    QSoundEffect* deathSound;

public:
    explicit PlayerModel(Vector2f startPos);
    ~PlayerModel();

    void update(float dt) override;

    // ─── Déplacement ──────────────────────────────────────────────
    void move(Vector2f inputDir, float dt);
    void stopMoving() { moving = false; }

    // ─── Attaque katana ───────────────────────────────────────────
    /**
     * @brief Lance une attaque au katana orientée vers un angle libre.
     *
     * Convertit l'angle en direction cardinale dominante pour choisir le
     * bon sprite, puis délègue à Katana::tryAttack() avec l'angle exact
     * afin que la SlashZone soit orientée précisément vers le curseur.
     *
     * @param angleRad Angle en radians (atan2 standard : 0=droite, π/2=bas).
     * @return true si le coup a été déclenché, false si le katana est en cooldown.
     */
    bool launchKatana(float angleRad);

    // ─── Dégâts ───────────────────────────────────────────────────
    void takeDamage(int amount);
    bool isDead() const { return hp <= 0; }

    // ─── Collecte ─────────────────────────────────────────────────
    void addGold(int amount) { gold += amount; }
    void collectKey()        { hasKey = true;  }

    // ─── Getters ──────────────────────────────────────────────────
    int             getHp()           const { return hp;        }
    int             getMaxHp()        const { return maxHp;     }
    int             getGold()         const { return gold;      }
    bool            getHasKey()       const { return hasKey;    }
    bool            isMoving()        const { return moving;    }
    FacingDirection getFacing()       const { return facingDir; }
    float           getSpeed()        const { return speed; }

    const Katana&   getKatana()       const { return katana;    }
    bool            isKatanaActive()  const { return katana.isAnimActive();   }
    QRectF          getSlashZone()    const { return katana.getSlashZone();   }
    float           getAttackAngle()  const { return katana.getAttackAngle(); }

    GunModel&       getGun()                { return gun; }
    const GunModel& getGun()          const { return gun; }  // Note pour alex Pourquoi deux ?

    /**
     * @brief Tente de tirer une balle vers targetPos (coordonnées scène).
     * @param muzzlePos Position de la bouche du pistolet.
     * @param targetPos Position cible (curseur souris en coordonnées scène).
     * @return true si un tir a été effectué.
     */
    bool shootGun(Vector2f muzzlePos, Vector2f targetPos)
    { return gun.tryShoot(muzzlePos, targetPos); }

    /** @brief Recharge le pistolet (touche R). */
    void reloadGun() { gun.reload(); }

    // ─── Setters ──────────────────────────────────────────────────
    void setMaxHp(int v)          { maxHp = v;  }
    void setHp(int life)          { hp = life;  }
    void setSpeed(float v)        { speed = v;  }
    bool getBoostState();
    void setHasKey(bool key);

    void setKatanaDamage(int v)    { katana.setDamage(v);    }
    void setKatanaRange(int v)     { katana.setRange(v);     }
    void setKatanaCooldown(float v){ katana.setCooldown(v);  }

private:
    /**
     * @brief Convertit un angle radians en FacingDirection cardinale dominante.
     * @param angleRad Angle (atan2 standard).
     * @return La direction cardinale la plus proche.
     */
    static FacingDirection angleToFacing(float angleRad);
};

#endif // PLAYERMODEL_H
