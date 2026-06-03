#include "gamecontroller.h"
#include <Qt>
#include <algorithm>
#include <qDebug>

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
    // ── Création de la vue (fenêtre principale) ────────────────
    //itsGameView = new GameView();

    // ── Création du modèle global ─────────────────────────────
    //itsGameModel = new GameModel();

    // ── Liaison du clic droit  ────────────────────────KATANA
    // Dès que la vue émet le signal, on exécute une fonction qui attaque
    connect(itsGameView, &GameView::sig_rightClicked, this, [this]() {
        // Sécurité : on ne lance l'attaque que si on est en train de jouer
        if (currentState == AppState::PLAYING && itsGameModel && itsGameModel->getPlayer()) {
            itsGameModel->getPlayer()->launchKatana();
        }
    });



    // ── Liaison du clic gauche  ────────────────────────GUN
    // Dès que la vue émet le signal, on exécute une fonction qui attaque
    connect(itsGameView, &GameView::sig_leftClicked, this, [this]() {
        // Sécurité : on ne lance l'attaque que si on est en train de jouer
        if (currentState == AppState::PLAYING && itsGameModel && itsGameModel->getPlayer()) {
            //itsGameModel->gun->launchfire(); //Pour le pistolet
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

// ── Chargement d'un niveau ─────────────────────────────────────────
void GameController::loadLevel(int index) {
    // Sécurité : boucle sur les niveaux disponibles
    index = index % LevelData::LEVEL_COUNT;
    currentLevelIndex = index;

    const LevelDescriptor* desc = LevelData::ALL_LEVELS[index];

    // Réinitialise la scène Qt (supprime les QGraphicsItems précédents)
    if (itsGameView) itsGameView->clearScene();

    // Construit le niveau : remplit LevelModel + ajoute items dans la scène
    LevelBuilder::build(*desc,
                         *itsGameModel->getLevel(),
                         itsGameView->getScene());

    // Spawne le joueur à la position définie dans la matrice
    Vector2f spawn = itsGameModel->getLevel()->getSpawnPos();
    itsGameModel->initPlayer(spawn);
    itsGameModel->setCurrentLevelIndex(index);
}

// ── Tick principal (~60 FPS) ───────────────────────────────────────
void GameController::slt_tick() {
    if (currentState != AppState::PLAYING) return;

    // Delta-time (cap à 50 ms pour éviter les sauts lors de lag)
    float dt = static_cast<float>(elapsedTimer.restart()) / 1000.0f;
    dt = std::min(dt, 0.05f);

    // 1. Lecture des inputs clavier → déplace le joueur
    handleInput(dt);

    // 2. Mise à jour des timers internes du joueur (cooldowns katana)
    itsGameModel->getPlayer()->update(dt);

    // 3. Logique du niveau : IA, collisions, gold, sortie
    levelCtrl->update(dt, *itsGameModel->getPlayer(),
                           *itsGameModel->getLevel());

    // 4. Vérification des transitions d'état
    checkStateTransitions();

    // 5. Mise à jour des vues + caméra
    itsGameView->updateAllViews(dt, itsGameModel);
    itsGameView->followPlayer(itsGameModel);
}

// ── Lecture des entrées clavier ────────────────────────────────────
void GameController::handleInput(float dt) {
    const QSet<int>& keys  = itsGameView->getPressedKeys();
    PlayerModel*     player = itsGameModel->getPlayer();
    if (!player) return;

    // ─── Déplacement (ZQSD / WASD) ───────────────────────────
    Vector2f dir(0.0f, 0.0f);
    if (keys.contains(Qt::Key_Z) || keys.contains(Qt::Key_Up))    dir.y -= 1.0f;
    if (keys.contains(Qt::Key_S) || keys.contains(Qt::Key_Down))  dir.y += 1.0f;
    if (keys.contains(Qt::Key_Q) || keys.contains(Qt::Key_Left))  dir.x -= 1.0f;
    if (keys.contains(Qt::Key_D) || keys.contains(Qt::Key_Right)) dir.x += 1.0f;

    if (dir.length() > 0.0f)
        player->move(dir, dt);
    else
        player->stopMoving();

    // ─── Attaque katana (Espace ou J) ─────────────────────────
    if (keys.contains(Qt::Key_Space) || keys.contains(Qt::Key_J))
        player->launchKatana();
}

// ── Transitions d'état ─────────────────────────────────────────────
void GameController::checkStateTransitions() {
    if (itsGameModel->getPlayer()->isDead())
        transitionTo(AppState::GAME_OVER);
    else if (levelCtrl->levelJustCompleted)
        transitionTo(AppState::VICTORY);
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
