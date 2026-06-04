#include "levelmodel.h"
#include "enemy.h"

LevelModel::LevelModel()
    : exitDoor(nullptr), levelId(0)
{}

LevelModel::~LevelModel() { clear(); }

void LevelModel::clear() {
    for (auto* w : walls)   delete w;
    for (auto* e : enemies) delete e;
    delete exitDoor;

    walls.clear();
    enemies.clear();
    exitDoor = nullptr;
}

void LevelModel::addWall(WallModel* w)         { walls.append(w);   }
void LevelModel::addEnemy(Enemy* e)            { enemies.append(e); }
void LevelModel::setExitDoor(ExitDoorModel* d) { exitDoor = d;      }
void LevelModel::setPlayerSpawnPos(Vector2f p) { playerSpawnPos = p;}
void LevelModel::setLevelId(int id)            { levelId = id;      }

bool LevelModel::allEnemiesDefeated() {
    for (const auto* e : enemies)
        if (e->isActive()) return false;
        emit sigPlayerKeyObtain();//on envoie le signal
    return true;
}
