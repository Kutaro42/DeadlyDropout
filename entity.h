/**
 * @file entity.h
 * @brief Declaration of the Entity base class.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <QRectF>
#include "vector2f.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  Entity — Abstract base class for all game entities.
//  Contains position, dimensions and collider (AABB).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class Entity
 * @brief An abstract base class representing any interactive or physical object in the game.
 */
class Entity {
protected:
    Vector2f position; ///< The entity's 2D coordinates.
    float    width;    ///< The physical width of the entity in pixels.
    float    height;   ///< The physical height of the entity in pixels.
    bool     active;   ///< Status of the entity (false if dead or destroyed).

public:
    /**
     * @brief Constructor of the entity.
     * @param pos : Starting position.
     * @param w : width.
     * @param h : Height.
     */
    Entity(Vector2f pos, float w, float h);
    
    /**
     * @brief Virtual shredder.
     */
    virtual ~Entity() = default;

    /**
     * @brief Update to the entity's internal logic.
     * @param dt : Time elapsed since the last update (in seconds).
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
     * @brief Retrieves the entity's collision bounding box.
     * @return Rectangle délimitant la hitbox (AABB) de l'entité.
     */
    QRectF getCollider() const;
};

#endif // ENTITY_H
