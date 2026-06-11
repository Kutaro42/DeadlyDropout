/**
 * @file gamecontroller.cpp
 * @brief Implémentation du contrôleur de la boucle de jeu.
 */

#include "gamecontroller.h"
#include <Qt>
#include <algorithm>
#include <cmath>
#include <QDebug>
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
    connect(itsGameView, &GameView::sig_rightClicked,
            this, [this](QPointF scenePos) {
        if (currentState != AppState::PLAYING) return;
        if (!itsGameModel || !itsGameModel->getPlayer()) return;

        PlayerModel* player = itsGameModel->getPlayer();

        Vector2f ppos = player->getPosition();
        double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0;
        double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0;

        double dx    = scenePos.x() - cx;
        double dy    = scenePos.y() - cy;
        float  angle = static_cast<float>(std::atan2(dy, dx));

        player->launchKatana(angle);
    });

    // ── Touche Échap → pause ──────────────────────────────────────
    connect(itsGameView, &GameView::sig_escapePressed, this, &GameController::togglePause);

    // ── Clic gauche → pistolet ───────────────────────────────────────────────
    connect(itsGameView, &GameView::sig_leftClicked, this, [this](QPointF scenePos) {
        if (currentState != AppState::PLAYING) return;
        if (!itsGameModel || !itsGameModel->getPlayer()) return;

        PlayerModel* player = itsGameModel->getPlayer();

        // Calcul de la position de la bouche du pistolet
        // (centre du joueur + décalage vers le curseur, identique à GunView)
        Vector2f ppos = player->getPosition();
        double cx = static_cast<double>(ppos.x) + Cfg::ENTITY_SIZE / 2.0;
        double cy = static_cast<double>(ppos.y) + Cfg::ENTITY_SIZE / 2.0;
        double dx = scenePos.x() - cx;
        double dy = scenePos.y() - cy;
        double angle = std::atan2(dy, dx);
        Vector2f muzzle(
            static_cast<float>(cx + std::cos(angle) * Cfg::GUN_OFFSET_X),
            static_cast<float>(cy + std::sin(angle) * Cfg::GUN_OFFSET_X)
            );
        Vector2f target(static_cast<float>(scenePos.x()),
                        static_cast<float>(scenePos.y()));
        player->shootGun(muzzle, target);
    });

    // ── Boutons fin de partie ─────────────────────────────────────
    connect(itsGameView, &GameView::sigRestartRq, this, &GameController::sigRestart);
    connect(itsGameView, &GameView::sigQuitRq,    this, &GameController::sigBackMenu);

    // ── Chargement du registre de niveaux ────────────────────────
    // DOIT être appelé avant tout loadLevel() — sans init(), count()
    // retourne 0 et le modulo dans loadLevel() provoque un SIGFPE.
    LevelRegistry::instance().init();

    // ── Chargement du niveau 1 ────────────────────────────────────
    loadLevel(0);

    // ── Liaison vue ↔ modèle ──────────────────────────────────────
    itsGameView->initWithModel(itsGameModel);

    // ── Focus clavier ─────────────────────────────────────────────
    itsGameView->setFocus();

    // ── Boucle de jeu (~60 FPS) ───────────────────────────────────
    connect(&gameTimer, &QTimer::timeout, this, &GameController::slt_tick);
    elapsedTimer.start();
    gameTimer.start(Cfg::TIMER_MS);

    currentState = AppState::PLAYING;
}

void GameController::togglePause() {
    if (currentState == AppState::PLAYING) {
        currentState = AppState::PAUSED;

        pauseTextOverlay = new QGraphicsTextItem("--- JEU EN PAUSE ---\nAppuyez sur ECHAP pour reprendre");
        QFont font("Arial", 22, QFont::Bold);
        pauseTextOverlay->setFont(font);
        pauseTextOverlay->setDefaultTextColor(Qt::white);
        pauseTextOverlay->setZValue(100);
        pauseTextOverlay->setPos(itsGameView->mapToScene(
            itsGameView->width() / 2 - 150,
            itsGameView->height() / 2 - 40));
        itsGameView->getScene()->addItem(pauseTextOverlay);
    }
    else if (currentState == AppState::PAUSED) {
        currentState = AppState::PLAYING;

        if (pauseTextOverlay) {
            itsGameView->getScene()->removeItem(pauseTextOverlay);
            delete pauseTextOverlay;
            pauseTextOverlay = nullptr;
        }
        elapsedTimer.restart();
    }
}

void GameController::loadLevel(int index) {
    // ── Récupérer le descripteur depuis le registre ───────────────
    LevelRegistry& reg = LevelRegistry::instance();
    index = index % reg.count();
    currentLevelIndex = index;

    const LevelDescriptor* desc = reg.descriptorAt(index);
    if (!desc || desc->rows == 0) {
        qCritical() << "[GameController::loadLevel] Descripteur invalide pour l'index" << index
                    << "— vérifiez que les fichiers texte du niveau sont bien présents"
                    << "dans levels/level_" << (index + 1) << "/ et déclarés dans le .qrc";
        return;
    }

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
    if (currentState == AppState::PAUSED) {
        elapsedTimer.restart();
        return;
    }

    if (currentState != AppState::PLAYING) return;

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

    if (keys.contains(Qt::Key_R))
        player->reloadGun();
}

void GameController::checkStateTransitions() {
    if (itsGameModel->getPlayer()->isDead()) {
        transitionTo(AppState::GAME_OVER);
    } else if (levelCtrl->levelJustCompleted) {
        int nextIndex = currentLevelIndex + 1;
        if (nextIndex < LevelRegistry::instance().count()) {
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
