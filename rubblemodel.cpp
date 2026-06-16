/**
 * @file rubblemodel.cpp
 * @brief Implementation of the debris (delegates everything to Projectile).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "rubblemodel.h"

RubbleModel::RubbleModel(Vector2f pos, Vector2f dir, int damage, float speed)
    : Projectile(pos, Cfg::RUBBLE_SIZE, dir, speed, damage)
{}
