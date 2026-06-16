/**
 * @file projectile.cpp
 * @brief Implementation of the abstract Projectile class.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "projectile.h"

Projectile::Projectile(Vector2f pos, float size,
                       Vector2f dir, float speed, int damage)
    : Entity(pos, size, size)
    , direction(dir.normalized())
    , speed(speed)
    , damage(damage)
{}

void Projectile::update(float dt)
{
    if (!active) return;
    position = position + (direction * speed * dt);
}
