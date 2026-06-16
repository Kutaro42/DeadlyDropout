/**
 * @file bulletmodel.cpp
 * @brief Implémentation du modèle de balle (délègue tout à Projectile).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#include "Headers/bulletmodel.h"

BulletModel::BulletModel(Vector2f pos, Vector2f dir, int damage, float speed)
    : Projectile(pos, Cfg::BULLET_SIZE, dir, speed, damage)
{}
