#ifndef LEVELMODEL_H
#define LEVELMODEL_H

#include <QVector>
#include "wallmodel.h"
#include "exitdoormodel.h"
#include "vector2f.h"

class Enemy; // forward

// ═══════════════════════════════════════════════════════════════════
//  LevelModel — état d'un niveau (couche Model, MVC).
//  Contient les modèles de murs, ennemis, sortie et position spawn.
//  Peuplé par LevelBuilder lors du chargement du niveau.
// ═══════════════════════════════════════════════════════════════════
class LevelModel {
private:
    QVector<WallModel*>  walls;
    QVector<Enemy*>      enemies;
    ExitDoorModel*       exitDoor;
    Vector2f             playerSpawnPos;
    int                  levelId;

public:
    LevelModel();
    ~LevelModel();

    // Vide tous les conteneurs et libère la mémoire
    void clear();

    // ─── Ajout d'entités (utilisé par LevelBuilder) ───────────
    void addWall(WallModel* w);
    void addEnemy(Enemy* e);
    void setExitDoor(ExitDoorModel* d);
    void setPlayerSpawnPos(Vector2f pos);
    void setLevelId(int id);

    // ─── Accesseurs ───────────────────────────────────────────
    QVector<WallModel*>& getWalls()           { return walls;           }
    QVector<Enemy*>&     getEnemies()         { return enemies;         }
    ExitDoorModel*       getExitDoor()  const { return exitDoor;        }
    Vector2f             getSpawnPos()  const { return playerSpawnPos;  }
    int                  getLevelId()   const { return levelId;         }

    // Retourne true si tous les ennemis du niveau sont inactifs
    bool allEnemiesDefeated() const;
};

#endif // LEVELMODEL_H
