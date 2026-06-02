#include "gamecontroller.h"

GameController::GameController(QObject* parent) 
    : QObject(parent), itsGameView(nullptr), itsGameModel(nullptr), gameTimer(nullptr) {}

GameController::~GameController() {
    delete itsGameView;
    delete itsGameModel;
    delete gameTimer;
}

void GameController::init() {
    itsGameView = new GameView();
    itsGameModel = new GameModel(itsGameView->getGameScene());
    
    itsGameModel->initLevel();
    itsGameView->show();

    // Configuration du Timer de rafraîchissement global à ~60 FPS
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameController::gameLoop);
    gameTimer->start(16); // 16 ms par tick
}

void GameController::gameLoop() {
    float dt = 0.016f; 

    if (itsGameModel && itsGameView) {
        // Transmission de la direction d'entrée clavier normalisée au joueur mobile
        Vector2f dir = itsGameView->getInputDirection().normalized();
        if (itsGameModel->getPlayer()) {
            itsGameModel->getPlayer()->setVelocity(dir);
        }

        // Exécution d'un tick du moteur physique et logique
        itsGameModel->updateGame(dt);
    }
}