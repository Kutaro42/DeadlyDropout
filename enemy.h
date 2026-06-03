#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "player.h"

class Enemy : public Entity {
protected:
    int hp;
    int maxHp;
    float speed;
    int baseDamage;
    float attackCooldown;
    float attackTimer;
    float detectionRadius;
    int baseGoldDrop;

public:
    Enemy(Vector2f pos, int maxHp, float spd, int dmg, float det, float w, float h, int goldDrop);
    
    virtual void updateEnemy(float dt, Vector2f playerPos) = 0;
    virtual void attack(Player* p) = 0;

    void takeDamage(int amount);

    bool isDead() const ;
    int getGoldDrop() const ;
    bool detectsPlayer(Vector2f playerPos) const ;

};

#endif // ENEMY_H
