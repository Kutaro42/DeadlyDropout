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
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerModel — état et logique du joueur (couche Model, MVC).
//  Délègue tout ce qui concerne l'arme à l'objet Katana.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerModel
 * @brief Modèle encapsulant les propriétés, ressources et actions du joueur.
 * * Gère les PV, la vitesse, l'or, les clés et l'orientation cardinale. Les actions du Katana sont déléguées à l'instance interne.
 */
class PlayerModel : public Entity {
private:
    int             hp;        ///< Points de vie actuels (100 de base).
    int             maxHp;     ///< Maximum des points de vie (modifiable via upgrades).
    float           speed;     ///< Vitesse de déplacement normée (300 px/s de base).
    bool           speedBoosted; ///< Vitesse boostée ?
    int             gold;      ///< Nombre de Golds collectés, utilisés dans le market.
    bool            hasKey;    ///< Statut de possession de la clé pour terminer le niveau.
    bool            moving;    ///< Indicateur d'état précisant si le joueur est en mouvement.

    FacingDirection facingDir; ///< Direction du regard du joueur (Haut, Bas, Gauche, Droite).

    // ─── Arme ─────────────────────────────────────────────────
    Katana          katana;    ///< Instance gérant les mécaniques de l'arme de mêlée.

public:


    /**
     * @brief Constructeur initialisant les attributs avec les valeurs par défaut du projet.
     * @param startPos Position initiale sur la carte.
     */
    explicit PlayerModel(Vector2f startPos);

    // ─── Mise à jour (timers katana) ──────────────────────────
    /**
     * @brief Mise à jour de l'état logique du joueur et de ses timers (Katana).
     * @param dt Delta-time en secondes écoulé depuis le dernier appel.
     */
    void update(float dt) override;

    // ─── Déplacement ──────────────────────────────────────────
    /**
     * @brief Calcule et applique le vecteur de déplacement.
     * @param inputDir Vecteur de direction brut entré par le joueur (clavier).
     * @param dt Delta-time en secondes pour adapter la vitesse au taux de rafraîchissement.
     */
    void move(Vector2f inputDir, float dt);
    
    /**
     * @brief Force l'arrêt du statut de déplacement du joueur.
     */
    void stopMoving() { moving = false; }

    // ─── Attaque katana (reste dans PlayerModel, délègue à Katana) ──
    /**
     * @brief Sollicite le modèle d'arme Katana pour effectuer un coup (Slash).
     * @return true si le coup s'est bien lancé (cooldown prêt), false sinon.
     */
    bool launchKatana();

    // ─── Dégâts ───────────────────────────────────────────────
    /**
     * @brief Applique une diminution sur la jauge de vie du joueur.
     * @param amount Valeur des dégâts pris.
     */
    void takeDamage(int amount);
    
    /**
     * @brief Indique si le joueur est mort.
     * @return true si les PV sont à 0 ou en-dessous, false sinon.
     */
    bool isDead() const { return hp <= 0; }

    // ─── Collecte ─────────────────────────────────────────────
    /**
     * @brief Incrémente le montant d'or possédé.
     * @param amount Nombre de pièces à ajouter.
     */
    void addGold(int amount) { gold += amount; }
    
    /**
     * @brief Attribue la clé de fin de niveau au joueur.
     */
    void collectKey()        { hasKey = true;  }

    // ─── Getters ──────────────────────────────────────────────
    int             getHp()           const { return hp;       }  ///< Retourne les PV actuels.
    int             getMaxHp()        const { return maxHp;    }  ///< Retourne le plafond des PV.
    int             getGold()         const { return gold;     }  ///< Retourne le solde de Golds.
    bool            getHasKey()       const { return hasKey;   }  ///< Retourne vrai si la clé est possédée.
    bool            isMoving()        const { return moving;   }  ///< Retourne l'état de mouvement.
    FacingDirection getFacing()       const { return facingDir;}  ///< Retourne l'orientation du joueur.

    // Accès katana (LevelController pour la SlashZone, PlayerView pour l'anim)
    const Katana&   getKatana()       const { return katana;   }  ///< Permet d'accéder aux données du Katana en lecture seule.
    bool            isKatanaActive()  const { return katana.isAnimActive(); } ///< Retourne si l'attaque est en cours d'animation.
    QRectF          getSlashZone()    const { return katana.getSlashZone(); } ///< Récupère la boîte de collision du Katana.

    // ─── Setters upgrade ──────────────────────────────────────
    void setMaxHp(int v)        { maxHp = v; }            ///< Met à jour la jauge maximum de PV.
    void setSpeed(float v)      { speed = v; }            ///< Modifie la vitesse linéaire globale.
    bool getBoostState();
    
    // upgrades katana délégués
    void setKatanaDamage(int v)   { katana.setDamage(v);   }  ///< Augmente/baisse les dégâts du Katana.
    void setKatanaRange(int v)    { katana.setRange(v);    }  ///< Ajuste la distance d'effet de l'arme.
    void setKatanaCooldown(float v){ katana.setCooldown(v);}  ///< Ajuste le délai d'attaque.
};

#endif // PLAYERMODEL_H
