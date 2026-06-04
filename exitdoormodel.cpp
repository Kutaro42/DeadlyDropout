/**
 * @file exitdoormodel.cpp
 * @brief Implémentation du comportement de la porte de sortie.
 */

#include "exitdoormodel.h"

ExitDoorModel::ExitDoorModel(Vector2f pos, float w, float h)
    : Entity(pos, w, h), open(false) {}

void ExitDoorModel::update(float dt) { Q_UNUSED(dt); }

bool ExitDoorModel::tryEnter(const PlayerModel& /*player*/) const {
    // Actuellement, la validation dépend uniquement de l'état "open".
    // L'argument player n'est pas exploité, mais la signature permet l'ajout futur de clés d'inventaire.
    return open;
}