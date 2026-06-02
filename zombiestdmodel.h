#ifndef ZOMBIESTDMODEL_H
#define ZOMBIESTDMODEL_H

#include "enemy.h"

class ZombieStdModel : public Enemy {
public:
    ZombieStdModel(Vector2f pos);
    
    void update(float dt, Vector2f playerPos) override;
    void attack(Player* p) override;
};

#endif // ZOMBIESTDMODEL_H