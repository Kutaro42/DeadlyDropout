#include "gamemodel.h"

GameModel::GameModel()
    : player(nullptr),
      level(new LevelModel()),
      currentLevelIndex(0)
{}

GameModel::~GameModel() {
    delete player;
    delete level;
}

void GameModel::initPlayer(Vector2f spawnPos) {
    delete player;
    player = new PlayerModel(spawnPos);
}
