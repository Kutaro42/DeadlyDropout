#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "enemy.h"

class bossModel : public Enemy
{
public:
    bossModel(Vector2f pos);

    void update(float dt, Vector2f playerPos) ;

    void attack(PlayerModel* player);

private:
    void chase(Vector2f playerPos, float dt);

    void updateAIState(Vector2f playerPos);

};

#endif // BOSSMODEL_H
