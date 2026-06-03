#ifndef ENTITY_H
#define ENTITY_H

#include <QRectF>
#include "vector2f.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  Entity — classe de base abstraite pour toutes les entités du jeu.
//  Contient position, dimensions et collider (AABB).
// ═══════════════════════════════════════════════════════════════════
class Entity {
protected:
    Vector2f position;
    float    width;
    float    height;
    bool     active;

public:
    Entity(Vector2f pos, float w, float h);
    virtual ~Entity() = default;

    // Mise à jour logique (vide par défaut pour entités statiques)
    virtual void update(float dt);

    // ─── Getters / Setters ────────────────────────────────────
    Vector2f getPosition()  const { return position; }
    float    getWidth()     const { return width;    }
    float    getHeight()    const { return height;   }
    bool     isActive()     const { return active;   }

    void setActive(bool a)           { active   = a;   }
    void setPosition(Vector2f pos)   { position = pos; }

    // Retourne la hitbox pour la détection de collision (AABB)
    QRectF getCollider() const;
};

#endif // ENTITY_H
