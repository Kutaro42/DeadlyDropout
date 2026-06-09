/**
 * @file standarddoormodel.cpp
 * @brief Implémentation du comportement de la porte standard.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "standarddoormodel.h"
#include "playermodel.h"

StandardDoorModel::StandardDoorModel(Vector2f pos, float w, float h)
    : GlobalDoor(pos, w, h), open(false)
{}

void StandardDoorModel::update(float dt) { Q_UNUSED(dt); }

bool StandardDoorModel::tryOpen(const PlayerModel& player)
{
    if (open) return false;
    if (getCollider().intersects(player.getCollider())) {
        open = true;
        return true;
    }
    return false;
}
