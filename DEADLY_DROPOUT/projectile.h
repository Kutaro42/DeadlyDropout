/**
 * @file projectile.h
 * @brief Déclaration de la classe abstraite Projectile, base commune à tous les projectiles.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  Projectile — classe intermédiaire abstraite entre Entity et les
//  projectiles concrets (RubbleModel, BulletModel…).
//
//  Factorise :
//    - la direction normalisée de déplacement
//    - la vitesse en pixels/s
//    - les dégâts infligés à l'impact
//    - le déplacement en ligne droite (update)
//
//  Les sous-classes n'ont qu'à appeler le constructeur protégé et
//  peuvent surcharger update() si elles ont besoin d'un comportement
//  balistique particulier (gravité, courbe…).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Projectile
 * @brief Classe de base abstraite pour tout projectile se déplaçant en ligne droite.
 */
class Projectile : public Entity {
protected:
    Vector2f direction; ///< Vecteur unitaire de déplacement.
    float    speed;     ///< Vitesse en pixels/s.
    int      damage;    ///< Dégâts infligés au contact de la cible.

    /**
     * @brief Constructeur protégé : seules les sous-classes peuvent instancier.
     * @param pos    Position initiale du projectile.
     * @param size   Taille (largeur = hauteur) du sprite en pixels.
     * @param dir    Direction de tir (sera normalisée).
     * @param speed  Vitesse en pixels/s.
     * @param damage Dégâts infligés à l'impact.
     */
    Projectile(Vector2f pos, float size,
               Vector2f dir, float speed, int damage);

public:
    virtual ~Projectile() = default;

    /**
     * @brief Avance le projectile dans sa direction sur le delta-time fourni.
     *        Peut être surchargé par les sous-classes (ex: trajectoire courbe).
     * @param dt Delta-time en secondes.
     */
    void update(float dt) override;

    // ─── Getters ──────────────────────────────────────────────
    int      getDamage()    const { return damage;    } ///< Dégâts à l'impact.
    float    getSpeed()     const { return speed;     } ///< Vitesse en pixels/s.
    Vector2f getDirection() const { return direction; } ///< Vecteur unitaire de déplacement.
};

#endif // PROJECTILE_H
