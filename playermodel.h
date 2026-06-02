#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entity.h"
#include "config.h"
#include "katana.h"
#include "Vector2f.h"

class PlayerModel : public Entity {
    int lifePoints;
    DirectionSprite PlayerDirectionSprite;
    katana itsKatana;
    int gold;
    int maxLP;
    float walkSpeed;
    bool hasKey;
    Vector2f direction;

public:
    PlayerModel(Vector2f startPos);
    ~PlayerModel();

    void update(float dt);
    void move(Vector2f dir, float dt);
    void takeDamage(int amount);
    void collectKey();
    void addGold(int amount);
    void removeGold(int amount);
    bool isDead();
    int getHp();
    int getMaxHp();
    float getSpeed();
    int getGold() const;
    bool getHasKey();
    Vector2f getDirection();

    void setSpeed(int s);
    void setMaxHp(int mhp);
    void katanaAttack();
};

#endif // PLAYERMODEL_H