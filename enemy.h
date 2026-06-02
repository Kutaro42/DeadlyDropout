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
    Enemy(Vector2f pos, int maxHp, float spd, int dmg, float det, float w, float h, int goldDrop)
        : Entity(pos, w, h), hp(maxHp), maxHp(maxHp), speed(spd), baseDamage(dmg), 
          attackCooldown(1.5f), attackTimer(0.0f), detectionRadius(det), baseGoldDrop(goldDrop) {}
    
    virtual void updateEnemy(float dt, Vector2f playerPos) = 0;
    virtual void attack(Player* p) = 0;

    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) {
            hp = 0;
            setActive(false);
        }
    }

    bool isDead() const { return hp <= 0; }
    int getGoldDrop() const { return baseGoldDrop; }
    bool detectsPlayer(Vector2f playerPos) const {
        return (playerPos - position).length() <= detectionRadius;
    }
};

#endif // ENEMY_H