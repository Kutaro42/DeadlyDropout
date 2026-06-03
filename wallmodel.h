#ifndef WALLMODEL_H
#define WALLMODEL_H

#include "entity.h"

// ═══════════════════════════════════════════════════════════════════
//  WallModel — entité statique servant de mur de salle.
//  Son collider (QRectF) est utilisé par LevelController pour la
//  résolution des collisions AABB.
// ═══════════════════════════════════════════════════════════════════
class WallModel : public Entity {
public:
    WallModel(Vector2f pos, float w, float h);
    // update() vide : les murs sont statiques
    void update(float dt) override;
};

#endif // WALLMODEL_H
