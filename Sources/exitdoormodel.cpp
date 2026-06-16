/**
 * @file exitdoormodel.cpp
 * @brief Implémentation du comportement de la porte de sortie.
 */

#include "Headers/exitdoormodel.h"

ExitDoorModel::ExitDoorModel(Vector2f pos, float w, float h)
    : GlobalDoor(pos, w, h), open(false) {}

void ExitDoorModel::update(float dt) { Q_UNUSED(dt); }
