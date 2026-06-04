/**
 * @file gamemodel.cpp
 * @brief Implémentation du modèle racine.
 */

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
    // On s'assure d'écraser proprement l'ancienne instance si elle existe.
    delete player;
    player = new PlayerModel(spawnPos);
    connect(level,&LevelModel::sigPlayerKeyObtain,this,&GameModel::obtainKey);
}
void GameModel::obtainKey()
{
    getPlayer()->collectKey();
}
