#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"

class Projectile : public Entity {
private:
    Vector2f        direction;
    unsigned short  damage;
    float           speed;

public:
    /**
     * @brief Construit un projectile de type Gravat.
     */
    Projectile (Vector2f pos,
                Vector2f dir,
                unsigned short      damage = 1,      // Dégâts de base
                float    speed  = 250.0f); // Vitesse en pixels/s

    /**
     * @brief Fait avancer le gravat en ligne droite.
     */
    void update(float dt) override;

    // Getters
    unsigned short      getDamage()    const { return damage;    }
    Vector2f getDirection() const { return direction; }
    float    getSpeed()     const { return speed;     }


};

#endif // PROJECTILE_H
