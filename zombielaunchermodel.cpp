/**
 * @file zombielaunchermodel.cpp
 * @brief Implementation of the AI and firing system for the zombie launcher.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombielaunchermodel.h"

ZombieLauncherModel::ZombieLauncherModel(Vector2f pos)
    : Enemy(pos,
            Cfg::LAUNCHER_MAX_HP,
            Cfg::LAUNCHER_SPEED,
            0,                        
            Cfg::LAUNCHER_DETECT_R,
            static_cast<float>(Cfg::ENTITY_SIZE),
            static_cast<float>(Cfg::ENTITY_SIZE),
            Cfg::LAUNCHER_GOLD_MIN)
    , shootTimer(0.0f)
{}

// ---- Distance computation + AI state update --------------------------------
float ZombieLauncherModel::updateAIState(Vector2f playerPos)
{
    float dx   = playerPos.x - (position.x + width  / 2.0f);
    float dy   = playerPos.y - (position.y + height / 2.0f);
    float dist = std::sqrt(dx * dx + dy * dy);

    if (chasePersistTimer <= 0.0f) {
        aiState  = EnemyAIState::IDLE;
        velocity = Vector2f(0.0f, 0.0f);
    } else if (dist > Cfg::LAUNCHER_SHOOT_R) {
        aiState = EnemyAIState::CHASING;
    } else {
        aiState  = EnemyAIState::ATTACKING;
        velocity = Vector2f(0.0f, 0.0f);
    }
    return dist;
}

// ---- Movement toward the player -----------------------------------------
void ZombieLauncherModel::chase(Vector2f playerPos, float dt)
{
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

// ---- Create a rubble projectile aimed at the player -----------------------
void ZombieLauncherModel::shoot(Vector2f playerPos)
{
    // Firing origin: center of the sprite
    Vector2f origin(position.x + width  / 2.0f - Cfg::RUBBLE_SIZE / 2.0f,
                    position.y + height / 2.0f - Cfg::RUBBLE_SIZE / 2.0f);

    Vector2f dir = (playerPos - origin).normalized();
    rubbles.append(new RubbleModel(origin, dir));
}

// ---- Main update (called by LevelController) ─────────────────────────────
void ZombieLauncherModel::update(float dt, Vector2f playerPos)
{
    // Always update and prune in-flight rubble, even if the launcher is dead
    for (RubbleModel* r : rubbles)
        if (r->isActive()) r->update(dt);
    pruneInactiveRubbles();

    if (!isActive() && aiState != EnemyAIState::DEAD) return;
    if (tickTransientStates(dt)) return;

    if (shootTimer > 0.0f) {
        shootTimer -= dt;
        if (shootTimer < 0.0f) shootTimer = 0.0f;
    }

    updateAIState(playerPos);

    switch (aiState) {
    case EnemyAIState::CHASING:
        chase(playerPos, dt);
        break;
    case EnemyAIState::ATTACKING:
        if (shootTimer <= 0.0f) {
            shoot(playerPos);
            shootTimer = Cfg::LAUNCHER_SHOOT_CD;
        }
        break;
    default:
        break;
    }
}

void ZombieLauncherModel::attack(PlayerModel* player) {}

void ZombieLauncherModel::pruneInactiveRubbles()
{
    for (int i = rubbles.size() - 1; i >= 0; --i) {
        if (!rubbles[i]->isActive()) {
            delete rubbles[i];
            rubbles.removeAt(i);
        }
    }
}
