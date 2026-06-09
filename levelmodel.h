/**
 * @file levelmodel.h
 * @brief Modèle de données stockant l'état complet d'un niveau.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef LEVELMODEL_H
#define LEVELMODEL_H

#include <QVector>
#include <QObject>
#include "wallmodel.h"
#include "floormodel.h"
#include "exitdoormodel.h"
#include "standarddoormodel.h"
#include "vector2f.h"

class Enemy; // forward declaration

/**
 * @class LevelModel
 * @brief Conteneur de données pour les entités d'un niveau.
 * * Stocke les murs, les ennemis, la porte de sortie et les positions initiales.
 * Ce modèle est peuplé par le LevelBuilder au moment du chargement de la carte.
 * * @version 1.0
 */
class LevelModel : public QObject{
    Q_OBJECT
private:
    QVector<WallModel*>         walls;          ///< Liste des murs constituant la structure du niveau.
    QVector<FloorModel*>        floorTiles;     ///< Liste des tiles de sol décoratives (sans collision).
    QVector<Enemy*>             enemies;        ///< Liste des ennemis présents dans le niveau.
    QVector<StandardDoorModel*> standardDoors;  ///< Liste des portes standards du niveau.
    ExitDoorModel* exitDoor;       ///< Modèle de la porte de fin de niveau.
    Vector2f             playerSpawnPos; ///< Coordonnées d'apparition du joueur.
    int                  levelId;        ///< Identifiant unique du niveau courant.

public:
    /**
     * @brief Constructeur par défaut.
     */
    LevelModel();

    /**
     * @brief Destructeur. Libère la mémoire des entités stockées.
     */
    ~LevelModel();

    /**
     * @brief Vide tous les conteneurs et supprime les entités en mémoire.
     */
    void clear();

    // ─── Ajout d'entités (utilisé par LevelBuilder) ───────────
    void addWall(WallModel* w);                 ///< @param w Pointeur vers le mur à ajouter.
    void addFloorTile(FloorModel* f);           ///< @param f Pointeur vers la tile de sol à ajouter.
    void addEnemy(Enemy* e);                    ///< @param e Pointeur vers l'ennemi à ajouter.
    void addStandardDoor(StandardDoorModel* d); ///< @param d Pointeur vers la porte standard à ajouter.
    void setExitDoor(ExitDoorModel* d);         ///< @param d Pointeur vers la porte de sortie.
    void setPlayerSpawnPos(Vector2f pos);       ///< @param pos Les coordonnées de spawn.
    void setLevelId(int id);                    ///< @param id L'identifiant du niveau.

    // ─── Accesseurs ───────────────────────────────────────────
    QVector<WallModel*>&         getWalls()         { return walls;           } ///< @return La liste des murs.
    QVector<FloorModel*>&        getFloorTiles()    { return floorTiles;      } ///< @return La liste des tiles de sol.
    QVector<Enemy*>&             getEnemies()       { return enemies;         } ///< @return La liste des ennemis.
    QVector<StandardDoorModel*>& getStandardDoors() { return standardDoors;   } ///< @return La liste des portes standards.
    ExitDoorModel* getExitDoor()  const { return exitDoor;        } ///< @return La porte de sortie.
    Vector2f             getSpawnPos()  const { return playerSpawnPos;  } ///< @return La position de spawn du joueur.
    int                  getLevelId()   const { return levelId;         } ///< @return L'identifiant du niveau.

    /**
     * @brief Vérifie si tous les monstres du niveau sont morts.
     * @return true si tous les ennemis sont inactifs, false sinon.
     */
    bool bossDefeat() ;
    signals:
    void sigPlayerKeyObtain();//attribue la clé au joueur par appel
};

#endif // LEVELMODEL_H
