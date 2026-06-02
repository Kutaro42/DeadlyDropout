#include "gamemodel.h"
#include <QDebug>

GameModel::GameModel(QGraphicsScene* scene) : scene(scene), player(nullptr), playerView(nullptr) {}

GameModel::~GameModel() {
    delete player;
    for (auto z : zombiesModel) delete z;
    for (auto v : zombiesView) delete v;
}

void GameModel::initLevel() {
    // Matrice de niveau encodée à l'avance : 1 = Mur, 0 = Sol, 2 = Joueur, 3 = Zombie
    std::vector<std::vector<int>> mapLayout = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 1},
        {1, 0, 3, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    currentLevel.setMatrix(mapLayout);

    Vector2f playerSpawn;
    std::vector<Vector2f> zombieSpawns;

    // Décodage et affichage via le composant Template
    currentLevel.decodeLevel(scene, playerSpawn, zombieSpawns);

    // Initialisation du joueur
    player = new Player(playerSpawn);
    playerView = new QGraphicsRectItem(-20, -20, 40, 40);
    playerView->setBrush(Qt::blue);
    scene->addItem(playerView);
    playerView->setPos(player->getPosition().toQPointF());

    // Génération des zombies décodés
    for (const auto& pos : zombieSpawns) {
        ZombieStdModel* zModel = new ZombieStdModel(pos);
        ZombieStdView* zView = new ZombieStdView(zModel);
        zombiesModel.append(zModel);
        zombiesView.append(zView);
        scene->addItem(zView);
    }
}

void GameModel::updateGame(float dt) {
    if (!player || !player->isActive()) return;

    // Mise à jour du joueur
    player->update(dt);
    playerView->setPos(player->getPosition().toQPointF());

    Vector2f playerPos = player->getPosition();

    // Mise à jour et gestion des Hitbox / Combats des Zombies
    for (int i = 0; i < zombiesModel.size(); ++i) {
        if (zombiesModel[i]->isActive()) {
            zombiesModel[i]->updateEnemy(dt, playerPos);
            zombiesView[i]->updateView(dt);

            // Attaque et dégâts au contact des Hitbox
            if (zombiesModel[i]->getCollider().intersects(player->getCollider())) {
                zombiesModel[i]->attack(player);
                qDebug() << "Aïe! PV restants du Joueur :" << player->getHp();
            }
        } else {
            // Le zombie meurt : Incrémentation du Gold et destruction d'entité
            player->addGold(zombiesModel[i]->getGoldDrop());
            qDebug() << "Zombie éliminé! Or accumulé :" << player->getGold();

            scene->removeItem(zombiesView[i]);
            delete zombiesView[i];
            delete zombiesModel[i];
            zombiesView.removeAt(i);
            zombiesModel.removeAt(i);
            i--; // Ajustement de l'index du conteneur
        }
    }
}