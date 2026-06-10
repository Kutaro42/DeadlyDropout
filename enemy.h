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
    bool         isBoss = false;     ///< Repère pour identifier le boss.
    EnemyAIState aiState;            ///< État de la machine à états de l'IA.
    Vector2f     velocity;           ///< Vecteur directionnel de déplacement actuel.
    bool         hitThisSlash;       ///< Empêche les dégâts multiples par attaque.

public:
    /**
     * @brief Constructeur de l'ennemi.
     * @param pos: Position de spawn.
     * @param maxHp: Points de vie.
     * @param speed: Vitesse de déplacement.
     * @param dmg: Dégâts.
     * @param detRadius: Rayon de détection.
     * @param w: Largeur.
     * @param h: Hauteur.
     * @param gold: Or looté à la mort.
     */
    Enemy(Vector2f pos, int maxHp, float speed, int dmg,
          float detRadius, float w, float h, int gold);
    virtual ~Enemy() = default;

    // ─── Interface virtuelle pure ──────────────────────────────
    
    /**
     * @brief Met à jour le comportement cognitif et spatial de l'ennemi.
     * @param dt: Delta-time en secondes.
     * @param playerPos: Coordonnées actuelles du joueur.
     */
    virtual void update(float dt, Vector2f playerPos) = 0;
    
    /**
     * @brief Déclenche une attaque contre le joueur.
     * @param player: Pointeur vers l'instance du joueur ciblé.
     */
    virtual void attack(PlayerModel* player) = 0;

    // ─── Comportements communs ────────────────────────────────
    
    /**
     * @brief Réduit les points de vie de l'ennemi et gère sa destruction.
     * @param amount: Quantité de dégâts subis.
     */
    void takeDamage(int amount);
    
    /**
     * @brief Vérifie si la position du joueur est à l'intérieur du cône/rayon de détection.
     * @param playerPos: Position du joueur.
     * @return true si détecté, false sinon.
     */
    bool detectsPlayer(Vector2f playerPos) const;

    /**
     * @brief Vérifie si un ennemi mort a donné de l'or.
     * @return true si gold déjà donné, false sinon.
     */
    bool  hasDroppedGold()  const { return goldAlreadyDropped; }

    /**
     * @brief Marque l'ennemi comme ayant déjà donné de l'or.
     */
    void  markGoldDropped()       { goldAlreadyDropped = true; }

    /**
     * @brief Setter qui change le booléen enemyHit entre true et false.
     * @param enemyHit: booléen qui indique si l'ennemie est touché ou non.
     */
    void  setHitThisSlash(bool enemyHit) { hitThisSlash = enemyHit;          }


    // Getteur

    /**
     * @brief Getteur pour accèder a hpi.
     * @return int hp: les pv de l'ennemi.
     */
    int          getHp()       const { return hp;       }

    /**
     * @brief Getteur pour accèder a maxHp.
     * @return int maxHp: les pv max de l'ennemi.
     */
    int          getMaxHp()    const { return maxHp;    }

    /**
     * @brief Getteur pour accèder a goldDrop
     * @return int goldDrop: le gold donné par l'ennemi.
     */
    int          getGoldDrop() const { return goldDrop; }

    /**
     * @brief Getteur pour accèder au booléen isBoss.
     * @return bool getisBoss: booléen qui indique si l'ennemi est un boss.
     */
    bool         getisBoss()   const { return isBoss;   }

    /**
     * @brief Getteur pour accèder a l'état de l'ennemi.
     * @return EnemyAIState aiState: l'état de l'ennemi.
     */
    EnemyAIState getAIState()  const { return aiState;  }

    /**
     * @brief Getteur pour accèder au vecteur velocity.
     * @return Vector2f velocity: vecteur directionnel de l'ennemi.
     */
    Vector2f     getVelocity() const { return velocity; }

    /**
     * @brief Getteur pour accèder au booléen hitThisSlash.
     * @return bool hitThisSlash: booléen qui indique si l'ennemi est touché par l'attaque.
     */
    bool  getHitThisSlash() const { return hitThisSlash;  }




    // Délégation Entity::update (sans playerPos — no-op pour les ennemis)
    /**
     * @brief Exécute la logique de l'IA (mise à jour état, gestion des temps de pause) à chaque tick.
     * @param dt Écart de temps en secondes.
     */
    void update(float dt) override { Q_UNUSED(dt); }
};

#endif // ENEMY_H
