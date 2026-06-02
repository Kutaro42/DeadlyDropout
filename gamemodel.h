#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QVector>
#include "zombiestdview.h"

class GameModel {
private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* mapBackground;
    QVector<ZombieStdModel*> zombiesModel;
    QVector<ZombieStdView*> zombiesView;

public:
    GameModel(QGraphicsScene* scene);
    ~GameModel();

    // Applique la texture de base (PNG) pour le niveau
    void loadLevelMap(const QString& imagePath);
    
    void addZombie(Vector2f position);
    void updateGame(float dt, Vector2f playerPos);
    
    // Gère la mort d'un zombie (incrémente l'or du joueur)
    void handleZombieDeath(int zombieIndex, class Player* player);
};

#endif // GAMEMODEL_H