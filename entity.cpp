#include "entity.h"

Entity::Entity(Vector2f pos, float w, float h)
    : position(pos), width(w), height(h), active(true) {}

void Entity::update(float dt) { Q_UNUSED(dt); }

QRectF Entity::getCollider() const {
    return QRectF(static_cast<double>(position.x),
                  static_cast<double>(position.y),
                  static_cast<double>(width),
                  static_cast<double>(height));
}
