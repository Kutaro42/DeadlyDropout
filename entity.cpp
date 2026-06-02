#include "entity.h"

Entity::Entity(Vector2f pos, float w, float h) 
    : position(pos), velocity(0, 0), active(true) {
    // Le collider est centré sur la position
    collider = QRectF(-w/2, -h/2, w, h);
}

Vector2f Entity::getPosition() const { return position; }

void Entity::setPosition(const Vector2f& pos) { position = pos; }

QRectF Entity::getCollider() const {
    // Retourne la hitbox absolue dans le monde
    return QRectF(position.x + collider.x(), position.y + collider.y(), 
                  collider.width(), collider.height());
}

bool Entity::isActive() const { return active; }

void Entity::setActive(bool a) { active = a; }