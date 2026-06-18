/**
 * @file gamemodel.cpp
 * @brief Implementation of the root game model.
 */

#include "Headers/gamemodel.h"

GameModel::GameModel()
    : player(nullptr),
      level(new LevelModel()),
      currentLevelIndex(0)
{
    // player is intentionally nullptr at construction time:
    // the spawn position is only known after LevelBuilder::build() runs.
}

GameModel::~GameModel() {
    delete player;
    delete level;
}

void GameModel::initPlayer(Vector2f spawnPos) {
    // Safely overwrite any existing player instance so that
    // a "restart" scenario does not leak the previous object.
    delete player;
    player = new PlayerModel(spawnPos);

    // Wire key pickup: LevelModel emits sigPlayerKeyObtain when the player
    // walks over the boss-dropped key; GameModel relays it to the player
    // inventory so neither model needs a direct reference to the other.
    connect(level, &LevelModel::sigPlayerKeyObtain, this, &GameModel::obtainKey);
}

void GameModel::obtainKey()
{
    getPlayer()->collectKey();
}

void GameModel::ComputeBulletSpawnDirection(QPointF scenePos) const{
    //if (currentState != AppState::PLAYING) return;
    //if (!itsGameModel || !itsGameModel->getPlayer()) return;


    // Compute the muzzle gun position
    // (player center + shift toward cursor)
    Vector2f ppos = player->getPosition();
    double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0;
    double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0;
    double dx = scenePos.x() - cx;
    double dy = scenePos.y() - cy;
    double angle = std::atan2(dy, dx); // Reuse tangent calculation
    Vector2f muzzle(
        static_cast<float>(cx + std::cos(angle) * Cfg::GUN_OFFSET_X),
        static_cast<float>(cy + std::sin(angle) * Cfg::GUN_OFFSET_X)
        );
    Vector2f target(static_cast<float>(scenePos.x()),
                    static_cast<float>(scenePos.y()));
    player->shootGun(muzzle, target);
}


void GameModel:: ComputeOrientationSlash( QPointF scenePos) const{

        Vector2f ppos = player->getPosition();
        double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0; // Convert and fill Vector2f attributes
        double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0; // *

        double dx    = scenePos.x() - cx;
        double dy    = scenePos.y() - cy;
        float  angle = static_cast<float>(std::atan2(dy, dx)); // Compute arctan of dx/dy → attack direction

        player->launchKatana(angle);
}

Enemy* GameModel::getBoss()
{
    for (Enemy* e : level->getEnemies())
    {
        if (e->getisBoss())
        {
            return e;
        }
    }
    // Returns implicitly nullptr if no boss is found.
    // Undefined behaviour warning suppressed by the compiler in some configs;
    // an explicit return nullptr should be added here.
}
