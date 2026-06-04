/**
 * @file wallmodel.cpp
 * @brief Implémentation de la classe WallModel représentant les murs physiques du jeu.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "wallmodel.h"

WallModel::WallModel(Vector2f pos, float w, float h)
    : Entity(pos, w, h) {}

void WallModel::update(float dt) { Q_UNUSED(dt); }