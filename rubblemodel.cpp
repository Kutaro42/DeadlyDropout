/**
 * @file rubblemodel.cpp
 * @brief Implémentation du gravat (délègue tout à Projectile).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "rubblemodel.h"

RubbleModel::RubbleModel(Vector2f pos, Vector2f dir, int damage, float speed)
    : Projectile(pos, Cfg::RUBBLE_SIZE, dir, speed, damage)
{}
