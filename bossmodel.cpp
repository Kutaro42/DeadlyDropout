#include "bossmodel.h"
#include "playermodel.h"

bossModel::bossModel(Vector2f pos) :
    Enemy(pos, Cfg::BOSSP1_MAX_HP, Cfg::BOSS_SPEED,
          Cfg::BOSS_DMG, Cfg::BOSS_DETECT_R,
          static_cast<float>(Cfg::ENTITY_SIZE*3),
          static_cast<float>(Cfg::ENTITY_SIZE*3),
          Cfg::BOSS_GOLD_MIN)
{
    isBoss = true;
}

void bossModel::attack(PlayerModel* player)
{
    if (!player || attackTimer > 0.0f) return;

    player->takeDamage(Cfg::BOSS_DMG);
    attackTimer = Cfg::BOSS_ATK_CD;
}

void bossModel::update(float dt, Vector2f playerPos)
{
    if (!isActive()) return;

    // Décompte du cooldown d'attaque
    if (attackTimer > 0.0f)
        attackTimer -= dt;

    updateAIState(playerPos);

    if (aiState == EnemyAIState::CHASING)
        chase(playerPos, dt);
    else
        velocity = Vector2f(0.0f, 0.0f);
}

void bossModel::chase(Vector2f playerPos, float dt)
{
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

void bossModel::updateAIState(Vector2f playerPos)
{
    if (detectsPlayer(playerPos))
        aiState = EnemyAIState::CHASING;
    else
        aiState = EnemyAIState::IDLE;
}
