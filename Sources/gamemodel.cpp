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
