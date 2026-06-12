/**
 * @file zombielaunchermodel.cpp
 * @brief Implémentation de l'IA et du système de tir du zombie lanceur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombielaunchermodel.h"
#include "playermodel.h"
#include <cmath>

ZombieLauncherModel::ZombieLauncherModel(Vector2f pos)
    : Enemy(pos,
            Cfg::LAUNCHER_MAX_HP,
            Cfg::LAUNCHER_SPEED,
            0,                        // pas de dégâts au corps-à-corps
            Cfg::LAUNCHER_DETECT_R,
            static_cast<float>(Cfg::ENTITY_SIZE),
            static_cast<float>(Cfg::ENTITY_SIZE),
            Cfg::LAUNCHER_GOLD_MIN)
    , shootTimer(0.0f)
{}

// ─── Calcul de distance + mise à jour de l'état IA ───────────────
float ZombieLauncherModel::updateAIState(Vector2f playerPos)
{
    float dx   = playerPos.x - (position.x + width  / 2.0f);
    float dy   = playerPos.y - (position.y + height / 2.0f);
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > Cfg::LAUNCHER_DETECT_R) {
        aiState  = EnemyAIState::IDLE;
        velocity = Vector2f(0.0f, 0.0f);
    } else if (dist > Cfg::LAUNCHER_SHOOT_R) {
        // Détecté mais hors portée de tir → poursuivre
        aiState = EnemyAIState::CHASING;
    } else {
        // À portée de tir → rester immobile et tirer
        aiState  = EnemyAIState::ATTACKING;
        velocity = Vector2f(0.0f, 0.0f);
    }
    return dist;
}

// ─── Déplacement vers le joueur ───────────────────────────────────
void ZombieLauncherModel::chase(Vector2f playerPos, float dt)
{
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

// ─── Création d'un gravat dirigé vers le joueur ───────────────────
void ZombieLauncherModel::shoot(Vector2f playerPos)
{
    // Origine du tir : centre du sprite
    Vector2f origin(position.x + width  / 2.0f - Cfg::RUBBLE_SIZE / 2.0f,
                    position.y + height / 2.0f - Cfg::RUBBLE_SIZE / 2.0f);

    Vector2f dir = (playerPos - origin).normalized();
    rubbles.append(new RubbleModel(origin, dir));
}

// ─── Mise à jour principale (appelée par LevelController) ─────────
void ZombieLauncherModel::update(float dt, Vector2f playerPos)
{
    // Toujours mettre à jour et purger les rubbles en vol,
    // même si le launcher est mort
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

// ─── Attaque de contact (non utilisée, satisfait l'interface) ─────
void ZombieLauncherModel::attack(PlayerModel* /*player*/) {}

// ─── Nettoyage des gravats inactifs ───────────────────────────────
void ZombieLauncherModel::pruneInactiveRubbles()
{
    for (int i = rubbles.size() - 1; i >= 0; --i) {
        if (!rubbles[i]->isActive()) {
            delete rubbles[i];
            rubbles.removeAt(i);
        }
    }
}
