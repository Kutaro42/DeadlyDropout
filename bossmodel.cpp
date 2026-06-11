#include "bossmodel.h"
#include "playermodel.h"

bossModel::bossModel(Vector2f pos) :
    Enemy(pos, Cfg::BOSSP1_MAX_HP, Cfg::BOSS_SPEED,
          Cfg::BOSS_DMG, Cfg::BOSS_DETECT_R,
          static_cast<float>(Cfg::ENTITY_SIZE*3),
          static_cast<float>(Cfg::ENTITY_SIZE*3),
          Cfg::BOSS_GOLD_MIN)
{
    // Booléen indiquant que cette ennemie est le boss dans le vecteur d'ennemis.
    isBoss = true;
}

void bossModel::attack(PlayerModel* player)
{
    // Vérifcation que cooldown est bien écoulé avant une attaque.
    if (attackTimer > 0.0f) return;

    // inflige les dégât au joueur et lance le cooldown.
    player->takeDamage(Cfg::BOSS_DMG);
    attackTimer = Cfg::BOSS_ATK_CD;
}

void bossModel::update(float dt, Vector2f playerPos)
{
    //Vérifie que le boss est en vie (ou en train de mourir) pour ne pas update un ennemi déjà désactivé.
    if (!isActive() && aiState != EnemyAIState::DEAD) return;

    // Gestion des états transitoires (HURT, DEAD) — court-circuite l'IA si actif.
    if (tickTransientStates(dt)) return;

    // Décompte du cooldown d'attaque.
    if (attackTimer > 0.0f)
        attackTimer -= dt;

    // Appel de la vérification si un joueur est proche.
    updateAIState(playerPos);

    // Déplacement si proche du joueur.
    if (aiState == EnemyAIState::CHASING)
        chase(playerPos, dt);
    else
        velocity = Vector2f(0.0f, 0.0f);
}

void bossModel::chase(Vector2f playerPos, float dt)
{
    // Calcul du vecteur de déplacement pour déplacer suivre le joueur.
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

void bossModel::updateAIState(Vector2f playerPos)
{
    // Alterne entre CHASING et IDLE selon proximité du joueur
    if (detectsPlayer(playerPos))
        aiState = EnemyAIState::CHASING;
    else
        aiState = EnemyAIState::IDLE;
}
