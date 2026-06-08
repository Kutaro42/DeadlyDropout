/**
 * @file gamecontroller.cpp
 * @brief Implémentation du contrôleur de la boucle de jeu.
 */

#include "gamecontroller.h"
#include <Qt>
#include <algorithm>
#include <cmath>
#include <qDebug>
#include "config.h"

GameController::GameController(QObject* parent)
    : QObject(parent)
    , itsGameView(new GameView())
    , itsGameModel(new GameModel())
    , levelCtrl(new LevelController())
    , currentState(AppState::MENU)
    , currentLevelIndex(0)
{}

GameController::~GameController() {
    delete itsGameView;
    delete itsGameModel;
    delete levelCtrl;
}

void GameController::init() {

    // ── Clic droit → attaque katana orientée vers le curseur ─────────────────
    // Le signal transporte désormais la position de la souris en coordonnées scène,
    // ce qui permet de calculer l'angle exact joueur → curseur.
    connect(itsGameView, &GameView::sig_rightClicked,
            this, [this](QPointF scenePos) {
        if (currentState != AppState::PLAYING) return;
        if (!itsGameModel || !itsGameModel->getPlayer()) return;

        PlayerModel* player = itsGameModel->getPlayer();

        // Centre de la hitbox du joueur en coordonnées scène
        Vector2f ppos = player->getPosition();
        double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0;
        double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0;

        // Angle en radians entre le centre joueur et le curseur
        double dx    = scenePos.x() - cx;
        double dy    = scenePos.y() - cy;
        float  angle = static_cast<float>(std::atan2(dy, dx));

        player->launchKatana(angle);
    });

    // ── Clic gauche → pistolet (futur) ───────────────────────────────────────
    connect(itsGameView, &GameView::sig_leftClicked, this, [this]() {
        if (currentState == AppState::PLAYING && itsGameModel && itsGameModel->getPlayer()) {
            qDebug("pan !");
        }
    });

    // ── Boutons fin de partie ────────────────────────────────────────────────
    connect(itsGameView, &GameView::sigRestartRq, this, &GameController::sigRestart);
    connect(itsGameView, &GameView::sigQuitRq,    this, &GameController::sigBackMenu);

    // ── Chargement du niveau 1 ───────────────────────────────────────────────
    loadLevel(0);

    // ── Liaison vue ↔ modèle ─────────────────────────────────────────────────
    itsGameView->initWithModel(itsGameModel);

    // ── Focus clavier ────────────────────────────────────────────────────────
    itsGameView->setFocus();

    // ── Boucle de jeu (~60 FPS) ──────────────────────────────────────────────
    connect(&gameTimer, &QTimer::timeout, this, &GameController::slt_tick);
    elapsedTimer.start();
    gameTimer.start(Cfg::TIMER_MS);

    currentState = AppState::PLAYING;
}

void GameController::loadLevel(int index) {
    index = index % LevelData::LEVEL_COUNT;
    currentLevelIndex = index;

    const LevelDescriptor* desc = LevelData::ALL_LEVELS[index];

    if (itsGameView) itsGameView->clearScene();

    LevelBuilder::build(*desc,
                        *itsGameModel->getLevel(),
                        itsGameView->getScene());

    Vector2f spawn = itsGameModel->getLevel()->getSpawnPos();
    if (itsGameModel->getPlayer() == nullptr)
        itsGameModel->initPlayer(spawn);
    else
        itsGameModel->getPlayer()->setPosition(spawn);

    itsGameModel->setCurrentLevelIndex(index);
}

void GameController::slt_tick() {
    if (currentState != AppState::PLAYING) return;

    float dt = static_cast<float>(elapsedTimer.restart()) / 1000.0f;
    dt = std::min(dt, 0.05f);

    handleInput(dt);
    itsGameModel->getPlayer()->update(dt);
    levelCtrl->update(dt, *itsGameModel->getPlayer(), *itsGameModel->getLevel());
    checkStateTransitions();

    itsGameView->updateAllViews(dt, itsGameModel);
    itsGameView->followPlayer(itsGameModel);
}

void GameController::handleInput(float dt) {
    const QSet<int>& keys  = itsGameView->getPressedKeys();
    PlayerModel*     player = itsGameModel->getPlayer();
    if (!player) return;

    Vector2f dir(0.0f, 0.0f);
    if (keys.contains(Qt::Key_Z) || keys.contains(Qt::Key_Up))    dir.y -= 1.0f;
    if (keys.contains(Qt::Key_S) || keys.contains(Qt::Key_Down))  dir.y += 1.0f;
    if (keys.contains(Qt::Key_Q) || keys.contains(Qt::Key_Left))  dir.x -= 1.0f;
    if (keys.contains(Qt::Key_D) || keys.contains(Qt::Key_Right)) dir.x += 1.0f;

    if (dir.length() > 0.0f)
        player->move(dir, dt);
    else
        player->stopMoving();

    if (keys.contains(Qt::Key_U)) {
        if (!player->getBoostState())
            player->setSpeed(Cfg::PLAYER_SPEED_BOOSTED);
        else
            player->setSpeed(Cfg::PLAYER_SPEED);
    }
    if (keys.contains(Qt::Key_H))
        player->setSpeed(Cfg::PLAYER_SPEED);
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
        case AppState::GAME_OVER: msg = "GAME OVER"; break;
        case AppState::VICTORY:   msg = "VICTOIRE";  break;
        default: break;
    }
    itsGameView->showOverlay(msg);
}
