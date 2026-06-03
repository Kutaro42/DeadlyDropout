#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity {
private:
    int hp;
    int maxHp;
    int gold;
    float speed;

public:
    Player(Vector2f pos) : Entity(pos, 40.0f, 40.0f), hp(100), maxHp(100), gold(0), speed(150.0f) {}

    void update(float dt) override {
        position = position + (velocity * speed * dt);
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) {
            hp = 0;
            setActive(false);
        }
    }

    void addGold(int amount) { gold += amount; }
    int getGold() const { return gold; }
    int getHp() const { return hp; }
};

#endif // PLAYER_H