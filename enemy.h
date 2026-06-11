/**
 * @file enemy.h
 * @brief Déclaration de la classe abstraite Enemy, gérant les comportements des adversaires.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "config.h"

class PlayerModel; // forward — évite la dépendance circulaire

// ═══════════════════════════════════════════════════════════════════
//  Enemy — classe de base abstraite pour tous les ennemis.
//  Hérite de Entity. Sous-classes : ZombieStdModel, (ThrowerModel…)
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Enemy
 * @brief Classe de base définissant la structure et les mécaniques d'une entité hostile.
 */
class Enemy : public Entity {
protected:
    int          hp;                 ///< Points de vie actuels de l'ennemi.
    int          maxHp;              ///< Points de vie maximum.
    float        speed;              ///< Vitesse de déplacement linéaire.
    int          baseDamage;         ///< Dégâts de base infligés au joueur.
    float        attackCooldown;     ///< Temps requis entre deux attaques.
    float        attackTimer;        ///< Timer interne gérant le délai de l'attaque.
    float        detectionRadius;    ///< Rayon de la zone d'aggro/détection.
    int          goldDrop;           ///< Récompense en or octroyée à la mort.
    bool         goldAlreadyDropped; ///< Drapeau anti-doublon pour le loot d'or.
    bool         isBoss = false;
    EnemyAIState aiState;            ///< État de la machine à états de l'IA.
    Vector2f     velocity;           ///< Vecteur directionnel de déplacement actuel.
    bool         hitThisSlash;       ///< Empêche les dégâts multiples par attaque.
    float        hurtTimer;          ///< Temps restant dans l'état HURT.
    float        deadTimer;          ///< Temps restant dans l'état DEAD avant désactivation.

public:
    /**
     * @brief Constructeur de l'ennemi.
     * @param pos Position de spawn.
     * @param maxHp Points de vie.
     * @param speed Vitesse de déplacement.
     * @param dmg Dégâts.
     * @param detRadius Rayon de détection.
     * @param w Largeur.
     * @param h Hauteur.
     * @param gold Or looté à la mort.
     */
    Enemy(Vector2f pos, int maxHp, float speed, int dmg,
          float detRadius, float w, float h, int gold);
    virtual ~Enemy() = default;

    // ─── Interface virtuelle pure ──────────────────────────────
    
    /**
     * @brief Met à jour le comportement cognitif et spatial de l'ennemi.
     * @param dt Delta-time en secondes.
     * @param playerPos Coordonnées actuelles du joueur.
     */
    virtual void update(float dt, Vector2f playerPos) = 0;
    
    /**
     * @brief Déclenche une attaque contre le joueur.
     * @param player Pointeur vers l'instance du joueur ciblé.
     */
    virtual void attack(PlayerModel* player) = 0;

    // ─── Comportements communs ────────────────────────────────
    
    /**
     * @brief Réduit les points de vie de l'ennemi. Passe en état HURT,
     *        ou déclenche l'animation de mort si les PV tombent à zéro.
     * @param amount Quantité de dégâts subis.
     */
    void takeDamage(int amount);

    /**
     * @brief Passe l'ennemi en état DEAD et démarre le timer d'animation de mort.
     *        L'ennemi sera effectivement désactivé (active = false) une fois le
     *        timer écoulé, via tickDeadState().
     */
    void triggerDeath();

    /**
     * @brief Décrémente les timers HURT et DEAD ; doit être appelé en début
     *        de chaque update() des sous-classes.
     * @param dt Delta-time en secondes.
     * @return true si l'ennemi est en état HURT ou DEAD (les sous-classes
     *         doivent alors court-circuiter leur propre logique d'IA).
     */
    bool tickTransientStates(float dt);
    
    /**
     * @brief Vérifie si la position du joueur est à l'intérieur du cône/rayon de détection.
     * @param playerPos Position du joueur.
     * @return true si détecté, false sinon.
     */
    bool detectsPlayer(Vector2f playerPos) const;

    // Appelé par LevelController pour distribuer le gold une seule fois
    bool  hasDroppedGold()  const { return goldAlreadyDropped; }
    void  markGoldDropped()       { goldAlreadyDropped = true; }

    bool  getHitThisSlash() const { return hitThisSlash;       }
    void  setHitThisSlash(bool b) { hitThisSlash = b;          }

    // ─── Getters ──────────────────────────────────────────────
    int          getHp()       const { return hp;       }
    int          getMaxHp()    const { return maxHp;    }
    int          getGoldDrop() const { return goldDrop; }
    bool         getisBoss()   const { return isBoss;   }
    EnemyAIState getAIState()  const { return aiState;  }
    Vector2f     getVelocity() const { return velocity; }

    // Délégation Entity::update (sans playerPos — no-op pour les ennemis)
    void update(float dt) override { Q_UNUSED(dt); }
};

#endif // ENEMY_H
