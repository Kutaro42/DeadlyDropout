#include "wallmodel.h"

WallModel::WallModel(Vector2f pos, float w, float h,
                     int tileIndex, int tilesetCols)
    : Entity(pos, w, h)
    , tileIndex(tileIndex)
    , tilesetCols(tilesetCols)
{}

void WallModel::update(float dt) { Q_UNUSED(dt); }
