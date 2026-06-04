#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "config.h"

class PlayerModel; // forward — évite la dépendance circulaire

// ═══════════════════════════════════════════════════════════════════
//  Enemy — classe de base abstraite pour tous les ennemis.
//  Hérite de Entity. Sous-classes : ZombieStdModel, (ThrowerModel…)
// ═══════════════════════════════════════════════════════════════════
class Enemy : public Entity {
protected:
    int          hp;
    int          maxHp;
    float        speed;
    int          baseDamage;
    float        attackCooldown;
    float        attackTimer;
    float        detectionRadius;
    int          goldDrop;
    bool         goldAlreadyDropped; // évite le double-comptage
    EnemyAIState aiState;
    Vector2f     velocity;

public:
    Enemy(Vector2f pos, int maxHp, float speed, int dmg,
          float detRadius, float w, float h, int gold);
    virtual ~Enemy() = default;

    // ─── Interface virtuelle pure ──────────────────────────────
    // update(dt, playerPos) : IA + déplacement
    virtual void update(float dt, Vector2f playerPos) = 0;
    // attack : inflige des dégâts au joueur
    virtual void attack(PlayerModel* player) = 0;

    // ─── Comportements communs ────────────────────────────────
    void takeDamage(int amount);
    bool detectsPlayer(Vector2f playerPos) const;

    // Appelé par LevelController pour distribuer le gold une seule fois
    bool  hasDroppedGold()  const { return goldAlreadyDropped; }
    void  markGoldDropped()       { goldAlreadyDropped = true; }

    // ─── Getters ──────────────────────────────────────────────
    int          getHp()       const { return hp;       }
    int          getMaxHp()    const { return maxHp;    }
    int          getGoldDrop() const { return goldDrop; }
    EnemyAIState getAIState()  const { return aiState;  }
    Vector2f     getVelocity() const { return velocity;  }

    // Délégation Entity::update (sans playerPos — no-op pour les ennemis)
    void update(float dt) override { Q_UNUSED(dt); }
};

#endif // ENEMY_H
