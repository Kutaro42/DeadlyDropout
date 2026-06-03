#include "wallmodel.h"

WallModel::WallModel(Vector2f pos, float w, float h)
    : Entity(pos, w, h) {}

void WallModel::update(float dt) { Q_UNUSED(dt); }
