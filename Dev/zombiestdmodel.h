#ifndef ZOMBIESTDMODEL_H
#define ZOMBIESTDMODEL_H

#include "enemy.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdModel — zombie standard (itération 1).
//  IA : IDLE → CHASING (détection) → ATTACKING (contact).
//  Stats (GDD) : 20 HP · 90 px/s · 7~12 dégâts · portée 400 px.
// ═══════════════════════════════════════════════════════════════════
class ZombieStdModel : public Enemy {
public:
    explicit ZombieStdModel(Vector2f pos);

    void update(float dt, Vector2f playerPos) override;
    void attack(PlayerModel* player) override;

private:
    void chase(Vector2f playerPos, float dt);
    void updateAIState(Vector2f playerPos);
};

#endif // ZOMBIESTDMODEL_H
