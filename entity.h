#ifndef ENTITY_H
#define ENTITY_H

#include "vector2f.h"
#include <QRectF>

class Entity {
protected:
    Vector2f position;
    Vector2f velocity;
    QRectF collider; // Remplace CollisionShape pour être compatible Qt
    bool active;

public:
    Entity(Vector2f pos, float w, float h);
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

    Vector2f getPosition() const;
    void setPosition(const Vector2f& pos);
    
    QRectF getCollider() const;
    bool isActive() const;
    void setActive(bool a);
};

#endif // ENTITY_H