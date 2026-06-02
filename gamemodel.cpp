#include "gamemodel.h"
// #include "player.h" // A inclure quand Player sera fait

GameModel::GameModel(QGraphicsScene* scene) : scene(scene), mapBackground(nullptr) {
}

GameModel::~GameModel() {
    for(auto z : zombiesModel) delete z;
    for(auto v : zombiesView) delete v;
}

void GameModel::loadLevelMap(const QString& imagePath) {
    QPixmap mapImg(imagePath);
    if (!mapImg.isNull()) {
        if (mapBackground) {
            scene->removeItem(mapBackground);
            delete mapBackground;
        }
        mapBackground = new QGraphicsPixmapItem(mapImg);
        mapBackground->setZValue(-1); // Met la carte en arrière-plan
        scene->addItem(mapBackground);
    }
}

void GameModel::addZombie(Vector2f position) {
    ZombieStdModel* zModel = new ZombieStdModel(position);
    ZombieStdView* zView = new ZombieStdView(zModel);
    
    zombiesModel.append(zModel);
    zombiesView.append(zView);
    scene->addItem(zView);
}

void GameModel::updateGame(float dt, Vector2f playerPos) {
    for (int i = 0; i < zombiesModel.size(); ++i) {
        if (zombiesModel[i]->isActive()) {
            zombiesModel[i]->update(dt, playerPos);
            zombiesView[i]->updateView(dt);
            
            // Si Collision entre le zombie et le joueur
            // if (zombiesModel[i]->getCollider().intersects(player->getCollider())) {
            //      zombiesModel[i]->attack(player);
            // }
        } else {
            // Le zombie est mort, on incrémente l'or du joueur et on nettoie la mémoire
            // player->collectGold(zombiesModel[i]->getGoldDrop());
            scene->removeItem(zombiesView[i]);
            delete zombiesView[i];
            delete zombiesModel[i];
            zombiesView.removeAt(i);
            zombiesModel.removeAt(i);
            i--; // Ajuste l'index après suppression
        }
    }
}