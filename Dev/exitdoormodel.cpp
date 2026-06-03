#include "exitdoormodel.h"

ExitDoorModel::ExitDoorModel(Vector2f pos, float w, float h)
    : Entity(pos, w, h), open(false) {}

void ExitDoorModel::update(float dt) { Q_UNUSED(dt); }

bool ExitDoorModel::tryEnter(const PlayerModel& /*player*/) const {
    return open;
}
