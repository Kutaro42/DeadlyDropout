/**
 * @file exitdoormodel.cpp
 * @brief Implementation of the exit door behavior.
 */

#include "Headers/exitdoormodel.h"

ExitDoorModel::ExitDoorModel(Vector2f pos, float w, float h)
    : GlobalDoor(pos, w, h), open(false) {}

void ExitDoorModel::update(float dt) { dt; }
