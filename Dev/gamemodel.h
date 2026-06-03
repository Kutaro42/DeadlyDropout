#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "playermodel.h"
#include "levelmodel.h"

// ═══════════════════════════════════════════════════════════════════
//  GameModel — état global de la partie (couche Model, MVC).
//  Possède le PlayerModel et le LevelModel courant.
//  Initialisé par GameController après le chargement du niveau.
// ═══════════════════════════════════════════════════════════════════
class GameModel {
private:
    PlayerModel* player;
    LevelModel*  level;
    int          currentLevelIndex;

public:
    GameModel();
    ~GameModel();

    // Crée/recrée le joueur au point de spawn donné
    void initPlayer(Vector2f spawnPos);

    void setCurrentLevelIndex(int i) { currentLevelIndex = i; }

    PlayerModel* getPlayer() const { return player; }
    LevelModel*  getLevel()  const { return level;  }
    int          getLevelIndex()   const { return currentLevelIndex; }
};

#endif // GAMEMODEL_H
