/**
 * @file entity.h
 * @brief Déclaration de la classe de base Entity.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <QRectF>
#include "vector2f.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  Entity — classe de base abstraite pour toutes les entités du jeu.
//  Contient position, dimensions et collider (AABB).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Entity
 * @brief Classe de base abstraite représentant tout objet interactif ou physique dans le jeu.
 */
class Entity {
protected:
    Vector2f position; ///< Coordonnées 2D de l'entité.
    float    width;    ///< Largeur physique de l'entité en pixels.
    float    height;   ///< Hauteur physique de l'entité en pixels.
    bool     active;   ///< État de l'entité (faux si morte ou détruite).

public:
    /**
     * @brief Constructeur de l'entité.
     * @param pos Position initiale.
     * @param w Largeur.
     * @param h Hauteur.
     */
    Entity(Vector2f pos, float w, float h);
    
    /**
     * @brief Destructeur virtuel.
     */
    virtual ~Entity() = default;

    /**
     * @brief Mise à jour de la logique interne de l'entité.
     * @param dt Temps écoulé depuis la dernière mise à jour (en secondes).
     */
    virtual void update(float dt);

    // ─── Getters / Setters ────────────────────────────────────
    Vector2f getPosition()  const { return position; }
    float    getWidth()     const { return width;    }
    float    getHeight()    const { return height;   }
    bool     isActive()     const { return active;   }

    void setActive(bool a)           { active   = a;   }
    void setPosition(Vector2f pos)   { position = pos; }

    /**
     * @brief Obtient la zone de collision de l'entité.
     * @return Rectangle délimitant la hitbox (AABB) de l'entité.
     */
    QRectF getCollider() const;
};

#endif // ENTITY_H