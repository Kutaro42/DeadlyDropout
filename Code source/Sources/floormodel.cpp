/**
 * @file floormodel.cpp
 * @brief Implémentation de FloorModel.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "Headers/floormodel.h"

FloorModel::FloorModel(Vector2f pos, float w, float h,
                       int tileIndex, int tilesetCols)
    : Entity(pos, w, h)
    , tileIndex(tileIndex)
    , tilesetCols(tilesetCols)
{}

void FloorModel::update(float dt) { Q_UNUSED(dt); }
