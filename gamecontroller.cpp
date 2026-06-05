/**
 * @file gamecontroller.cpp
 * @brief Implémentation du contrôleur de la boucle de jeu.
 */

#include "gamecontroller.h"
#include <Qt>
#include <algorithm>
#include <qDebug>
#include <config.h>

GameController::GameController(QObject* parent): QObject(parent), itsGameView( new GameView()),itsGameModel(new GameModel()),
      levelCtrl(new LevelController()),
      currentState(AppState::MENU),
      currentLevelIndex(0)
{}

GameController::~GameController() {
    delete itsGameView;
    delete itsGameModel;
    delete levelCtrl;
}

void GameController::init() {
    // Liaison des clics souris via des expressions lambda pour une gestion immédiate des actions d'attaque.

    // ── Liaison du clic droit ────────────────────────KATANA
    connect(itsGameView, &GameView::sig_rightClicked, this, [this]() {
        // Sécurité : on ne lance l'attaque que si on est en train de jouer
        if (currentState == AppState::PLAYING && itsGameModel && itsGameModel->getPlayer()) {
            itsGameModel->getPlayer()->launchKatana();
        }
    });

    // ── Liaison du clic gauche ────────────────────────GUN
    connect(itsGameView, &GameView::sig_leftClicked, this, [this]() {
        // Sécurité : on ne lance l'attaque que si on est en train de jouer
        if (currentState == AppState::PLAYING && itsGameModel && itsGameModel->getPlayer()) {
            //itsGameModel->gun->launchfire(); // Pour le pistolet
            qDebug("pan !");
        }
    });

    // ── Chargement du niveau 1 ────────────────────────────────
    loadLevel(0);

    // ── Liaison vue ↔ modèle ──────────────────────────────────
    itsGameView->initWithModel(itsGameModel);

    // ── Affichage ─────────────────────────────────────────────
    itsGameView->setFocus();

    // ── Démarrage de la boucle de jeu (QTimer ~60 FPS) ────────
    connect(&gameTimer, &QTimer::timeout,
            this,       &GameController::slt_tick);
    elapsedTimer.start();
    gameTimer.start(Cfg::TIMER_MS);

    currentState = AppState::PLAYING;
}

void GameController::loadLevel(int index) {
    // Sécurité : boucle modulaire pour éviter les dépassements d'index
    index = index % LevelData::LEVEL_COUNT;
    currentLevelIndex = index;

    const LevelDescriptor* desc = LevelData::ALL_LEVELS[index];

    //  Nettoyage impératif de la scène pour éviter les fuites de mémoire et chevauchements
    if (itsGameView) itsGameView->clearScene();

    LevelBuilder::build(*desc,
                         *itsGameModel->getLevel(),
                         itsGameView->getScene());

    Vector2f spawn = itsGameModel->getLevel()->getSpawnPos();
    if (itsGameModel->getPlayer()==nullptr)
    {
    itsGameModel->initPlayer(spawn);
    }
    else
    {
        itsGameModel->getPlayer()->setPosition(spawn);
    }
    itsGameModel->setCurrentLevelIndex(index);
}

void GameController::slt_tick() {
    if (currentState != AppState::PLAYING) return;

    // Calcul du Delta-time. Plafonné à 50ms pour empêcher les téléportations massives lors d'un pic de latence (lag spike).
    float dt = static_cast<float>(elapsedTimer.restart()) / 1000.0f;
    dt = std::min(dt, 0.05f);

    handleInput(dt);
    itsGameModel->getPlayer()->update(dt);
    levelCtrl->update(dt, *itsGameModel->getPlayer(),
                           *itsGameModel->getLevel());
    checkStateTransitions();

    itsGameView->updateAllViews(dt, itsGameModel);
    itsGameView->followPlayer(itsGameModel);
}

void GameController::handleInput(float dt) {
    const QSet<int>& keys  = itsGameView->getPressedKeys();
    PlayerModel* player = itsGameModel->getPlayer();
    if (!player) return;

    // Somme vectorielle des inputs pour autoriser les déplacements diagonaux.
    Vector2f dir(0.0f, 0.0f);
    if (keys.contains(Qt::Key_Z) || keys.contains(Qt::Key_Up))    dir.y -= 1.0f;
    if (keys.contains(Qt::Key_S) || keys.contains(Qt::Key_Down))  dir.y += 1.0f;
    if (keys.contains(Qt::Key_Q) || keys.contains(Qt::Key_Left))  dir.x -= 1.0f;
    if (keys.contains(Qt::Key_D) || keys.contains(Qt::Key_Right)) dir.x += 1.0f;

    if (dir.length() > 0.0f)
        player->move(dir, dt);
    else
        player->stopMoving();


    if (keys.contains(Qt::Key_G))
    {
        player->setSpeed(1000);
    }
    else if (keys.contains(Qt::Key_H))
    {
        player->setSpeed(Cfg::PLAYER_SPEED);
    }
}

void GameController::checkStateTransitions() {
    if (itsGameModel->getPlayer()->isDead()) {
        transitionTo(AppState::GAME_OVER);
    } else if (levelCtrl->levelJustCompleted) {
        int nextIndex = currentLevelIndex + 1;
        if (nextIndex < LevelData::LEVEL_COUNT) {
            loadLevel(nextIndex);
            itsGameView->initWithModel(itsGameModel);
        } else {
            transitionTo(AppState::VICTORY);
        }
    }
}

void GameController::transitionTo(AppState state) {
    currentState = state;
    gameTimer.stop();

    QString msg;
    switch (state) {
        case AppState::GAME_OVER:
            msg = "GAME OVER\n\nAppuyez sur Échap pour quitter";
            break;
        case AppState::VICTORY:
            msg = "VICTOIRE !\n\nNiveau terminé — Échap pour quitter";
            break;
        default: break;
    }
    itsGameView->showOverlay(msg);
}
