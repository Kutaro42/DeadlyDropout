/**
 * @file bulletmodel.cpp
 * @brief Implementation of the ball model (delegates everything to Projectile).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#include "Headers/bulletmodel.h"

BulletModel::BulletModel(Vector2f pos, Vector2f dir, int damage, float speed)
    : Projectile(pos, Cfg::BULLET_SIZE, dir, speed, damage)
{}
