#include "levelmodel.h"
#include "floormodel.h"
#include "enemy.h"
#include "standarddoormodel.h"
#include "bossdoormodel.h"
#include "enemy.h"

LevelModel::LevelModel()
    : exitDoor(nullptr), levelId(0)
{}

LevelModel::~LevelModel() { clear(); }

void LevelModel::clear() {
    for (auto* w : walls)      delete w;
    for (auto* f : floorTiles) delete f;
    for (auto* e : enemies)    delete e;
    for (auto* d : standardDoors) delete d;
    for (auto* d : bossDoors)   delete d;
    delete exitDoor;

    walls.clear();
    floorTiles.clear();
    enemies.clear();
    standardDoors.clear();
    bossDoors.clear();
    exitDoor = nullptr;
}

void LevelModel::addWall(WallModel* w)         { walls.append(w);       }
void LevelModel::addFloorTile(FloorModel* f)   { floorTiles.append(f);  }
void LevelModel::addEnemy(Enemy* e)            { enemies.append(e);     }
void LevelModel::addStandardDoor(StandardDoorModel* d) { standardDoors.append(d); }
void LevelModel::addBossDoor(BossDoorModel* d)         { bossDoors.append(d);      }
void LevelModel::setExitDoor(ExitDoorModel* d) { exitDoor = d;      }
void LevelModel::setPlayerSpawnPos(Vector2f p) { playerSpawnPos = p;}
void LevelModel::setLevelId(int id)            { levelId = id;      }

bool LevelModel::bossDefeat() {
    if (keyAlreadyGranted) return true;
    for (const auto* e : enemies)
        if (e->getisBoss() && !e->isActive())
        {
            emit sigPlayerKeyObtain();
        }
    return false;

}
