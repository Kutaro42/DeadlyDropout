/**
 * @file entity.cpp
 * @brief Implémentation des mécanismes de base de la classe Entity.
 */

#include "Headers/entity.h"

Entity::Entity(Vector2f pos, float w, float h)
    : position(pos), width(w), height(h), active(true) {}

void Entity::update(float dt) { 
    // Méthode vide par défaut. Évite aux entités statiques de devoir l'implémenter inutilement.
    Q_UNUSED(dt); 
}

QRectF Entity::getCollider() const {
    // Conversion en double requise par l'API QRectF de Qt pour assurer les calculs de collisions précis.
    return QRectF(static_cast<double>(position.x),
                  static_cast<double>(position.y),
                  static_cast<double>(width),
                  static_cast<double>(height));
}
