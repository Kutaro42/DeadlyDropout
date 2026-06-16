/**
 * @file entity.cpp
 * @brief Implementation of the basic mechanisms of the Entity class.
 */

#include "entity.h"

Entity::Entity(Vector2f pos, float w, float h)
    : position(pos), width(w), height(h), active(true) {}

void Entity::update(float dt) { 
}

QRectF Entity::getCollider() const {
    // Use of DOUBLE to ensure accurate collision calculations
    return QRectF(static_cast<double>(position.x),
                  static_cast<double>(position.y),
                  static_cast<double>(width),
                  static_cast<double>(height));
}
