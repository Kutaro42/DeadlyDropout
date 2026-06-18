/**
 * @file gamecontroller.cpp
 * @brief Implementation of the main game-loop controller.
 */

#include "Headers/gamecontroller.h"
#include "Headers/musiccontroller.h"
#include <QDebug>
#include "Headers/config.h"
#include "Headers/marketcontroller.h"

GameController::GameController(QObject* parent)
    : QObject(parent)
    , itsGameView(new GameView())
    , itsGameModel(new GameModel())
    , levelCtrl(new LevelController())
    , currentState(AppState::MENU)
    , currentLevelIndex(0)
    , itsMarketController(new MarketController(nullptr))
{}

GameController::~GameController() {
    delete itsGameView;
    delete itsGameModel;
    delete levelCtrl;
    delete itsMarketController;
}

void GameController::init(bool startMusic) {
    // ---- Pause Overlay --------------------------------------------------
    connect(itsGameView, &GameView::sigPauseMenu, this, &GameController::sltPauseMenu);

    // -- Right click → katana slash oriented on the cursor ---------------
    // The angle is computed on the model and pass by here to rather than in PlayerModel because
    // it requires the scene-space cursor position, which only the view knows about.
    connect(itsGameView, &GameView::sig_rightClicked,this, [this](QPointF scenePos){
        itsGameModel->ComputeOrientationSlash(scenePos);
    }); //revise on M.SAVARIT advices


    // -- Escape key → pause ----------------------------------------------
    connect(itsGameView, &GameView::sig_escapePressed, this, &GameController::togglePause);

    // -- Left click → Gun fire -------------------------------------------
    // Muzzle position is offset from the player center along the aim direction
    // so that the bullet spawns at the barrel tip rather than at the player's feet.
    connect(itsGameView, &GameView::sig_leftClicked, this, [this](QPointF scenePos) {
        if (currentState != AppState::PLAYING) return;
        if (!itsGameModel || !itsGameModel->getPlayer()) return;

        PlayerModel* player = itsGameModel->getPlayer();

        // Compute the muzzle gun position
        // (player center + shift toward cursor)
        Vector2f ppos = player->getPosition();
        double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0;
        double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0;
        double dx = scenePos.x() - cx;
        double dy = scenePos.y() - cy;
        double angle = std::atan2(dy, dx); // Reuse tangent calculation
        Vector2f muzzle(
            static_cast<float>(cx + std::cos(angle) * Cfg::GUN_OFFSET_X),
            static_cast<float>(cy + std::sin(angle) * Cfg::GUN_OFFSET_X)
            );
        Vector2f target(static_cast<float>(scenePos.x()),
                        static_cast<float>(scenePos.y()));
        player->shootGun(muzzle, target);
    });

    // -- ENDGAME BUTTONS -----------------------------------------------
    connect(itsGameView, &GameView::sigRestartRq, this, &GameController::sigRestart);
    connect(itsGameView, &GameView::sigQuitRq,    this, &GameController::sigBackMenu);

    // --- Pause menu music buttons (delegated to view) -----------------
    connect(itsGameView, &GameView::sigToggleMusic,
            this, []{ MusicController::instance().togglePause(); });
    connect(itsGameView, &GameView::sigNextTrack,
            this, []{ MusicController::instance().nextTrack(); });

    // --- Level registry initialisation --------------------------------
    // MUST be called before loadLevel(); the registry count() is used as
    // a modulo divisor — calling loadLevel() on an empty registry causes SIGFPE.
    LevelRegistry::instance().init();

    // --- Load first level -------------------------------------------
    // currentLevelIndex is 0 by default (initialiser list); explicit for clarity.
    loadLevel(currentLevelIndex);

    // --- Link View ↔ Model ------------------------------------------
    itsGameView->initWithModel(itsGameModel);

    // --- Keyboard focus -----------------------------------------------
    itsGameView->setFocus();

    // --- Game loop (~60 FPS) ----------------------------------------
    connect(&gameTimer, &QTimer::timeout, this, &GameController::slt_tick);
    elapsedTimer.start();
    gameTimer.start(Cfg::TIMER_MS);

    currentState = AppState::PLAYING;

    // --- In-game music ------------------------------------------------
    // startMusic = false when restarting to avoid cutting the current track
    if (startMusic)
        MusicController::instance().playRandomGame();
}

void GameController::togglePause() {
    // If playing, display pause overlay; if paused, resume and reset the timer
    // to avoid a large dt spike on the first tick after unpausing.
    if (currentState == AppState::PLAYING) {
        currentState = AppState::PAUSED;
        itsGameView->showPauseOverlay();
    }
    else if (currentState == AppState::PAUSED) {
        currentState = AppState::PLAYING;
        itsGameView->hidePauseOverlay();
        elapsedTimer.restart();
    }
}

/// Intercepts the "back to menu" signal from the pause overlay.
void GameController::sltPauseMenu() {
    itsGameView->hidePauseOverlay();
    emit sigBackMenu();
}

void GameController::loadLevel(int index) {
    // -- Validate index against the registry -------------------------
    LevelRegistry& reg = LevelRegistry::instance();
    index = index % reg.count();
    currentLevelIndex = index;

    const LevelDescriptor* desc = reg.descriptorAt(index);

    if (itsGameView) itsGameView->clearScene();

    LevelBuilder::build(*desc,
                        *itsGameModel->getLevel(),
                        itsGameView->getScene());

    // Reposition the player at the spawn defined by the level descriptor.
    // initPlayer is only called once (first run); subsequent levels just move
    // the existing player to preserve accumulated upgrades and gold.
    Vector2f spawn = itsGameModel->getLevel()->getSpawnPos();
    if (itsGameModel->getPlayer() == nullptr)
        itsGameModel->initPlayer(spawn);
    else
        itsGameModel->getPlayer()->setPosition(spawn);

    itsGameModel->setCurrentLevelIndex(index);
}

void GameController::slt_tick() {
    // Skip processing entirely when paused (just reset the timer to avoid
    // accumulating a large dt burst when unpausing).
    if (currentState == AppState::PAUSED) {
        elapsedTimer.restart();
        return;
    }

    if (currentState != AppState::PLAYING) return;

    // Compute delta-time; cap at 50 ms to prevent physics tunnelling on
    // lag spikes (e.g. window resize, OS scheduler jitter).
    float dt = static_cast<float>(elapsedTimer.restart()) / 1000.0f;
    dt = std::min(dt, 0.05f);

    handleInput(dt);
    itsGameModel->getPlayer()->update(dt);
    levelCtrl->update(dt, *itsGameModel->getPlayer(), *itsGameModel->getLevel());
    checkStateTransitions();

    itsGameView->updateAllViews(dt, itsGameModel, levelCtrl);
    itsGameView->followPlayer(itsGameModel);
}

void GameController::handleInput(float dt) {
    const QSet<int>& keys  = itsGameView->getPressedKeys();
    PlayerModel*     player = itsGameModel->getPlayer();
    if (!player) return;

    // Aggregate direction vector from held keys; diagonal movement emerges
    // naturally when two perpendicular keys are pressed simultaneously.
    Vector2f dir(0.0f, 0.0f);
    if (keys.contains(Qt::Key_Z) || keys.contains(Qt::Key_Up))    dir.y -= 1.0f;
    if (keys.contains(Qt::Key_S) || keys.contains(Qt::Key_Down))  dir.y += 1.0f;
    if (keys.contains(Qt::Key_Q) || keys.contains(Qt::Key_Left))  dir.x -= 1.0f;
    if (keys.contains(Qt::Key_D) || keys.contains(Qt::Key_Right)) dir.x += 1.0f;

    if (dir.length() > 0.0f)
        player->move(dir, dt);
    else
        player->stopMoving();

    // Debug/iteration-1 speed toggle (GDD Iteration 1: activate/deactivate with U key).
    if (keys.contains(Qt::Key_U)) {
        if (!player->getBoostState())
            player->setSpeed(Cfg::PLAYER_SPEED_BOOSTED);
        else
            player->setSpeed(Cfg::PLAYER_SPEED);
    }
    if (keys.contains(Qt::Key_H))
        player->setSpeed(Cfg::PLAYER_SPEED);

    if (keys.contains(Qt::Key_R))
        player->reloadGun();
}

void GameController::checkStateTransitions() {
    if (itsGameModel->getPlayer()->isDead()) {
        transitionTo(AppState::GAME_OVER);
    } else if (levelCtrl->levelJustCompleted) {
        int nextIndex = currentLevelIndex + 1;
        if (nextIndex < LevelRegistry::instance().count()) {
            // Between levels: stop the loop, open the market (blocking modal dialog),
            // then load the next level and resume.  The market runs synchronously via
            // QDialog::exec() so no state machine complexity is needed here.
            gameTimer.stop();
            itsMarketController->open(itsGameModel->getPlayer());
            loadLevel(nextIndex);
            itsGameView->initWithModel(itsGameModel);
            elapsedTimer.restart();
            gameTimer.start(Cfg::TIMER_MS);
        } else {
            transitionTo(AppState::VICTORY);
        }
    }
}

void GameController::transitionTo(AppState state) {
    currentState = state;
    gameTimer.stop(); // Prevent any further ticks from modifying a terminal state.

    QString msg;
    switch (state) {
        case AppState::GAME_OVER: msg = "GAME OVER"; break;
        case AppState::VICTORY:   msg = "VICTOIRE";  break;
        default: break;
    }
    itsGameView->showOverlay(msg);
}
