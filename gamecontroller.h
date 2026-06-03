#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include "gameview.h"
#include "gamemodel.h"
#include "levelcontroller.h"
#include "levelbuilder.h"
#include "leveltemplate.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  GameController — contrôleur principal du jeu (couche Controller).
//  Hérite QObject pour utiliser QTimer (boucle de jeu) et connect().
//
//  Rôle :
//    - Créer et relier GameModel, GameView, LevelController
//    - Charger un niveau via LevelBuilder
//    - Lire les entrées clavier depuis GameView à chaque tick
//    - Appeler LevelController::update() pour la logique
//    - Déclencher les transitions d'état (PLAYING→GAME_OVER/VICTORY)
// ═══════════════════════════════════════════════════════════════════
class GameController : public QObject {
    Q_OBJECT

private:
    GameView*        itsGameView;
    GameModel*       itsGameModel;
    LevelController* levelCtrl;

    AppState         currentState;
    QTimer           gameTimer;
    QElapsedTimer    elapsedTimer;
    int              currentLevelIndex;

public:
    explicit GameController(QObject* parent);
    ~GameController();

    // Initialise tout et affiche la fenêtre de jeu
    void init();

    GameView* getView() const { return itsGameView; }

private slots:
    // Appelé à chaque tick du QTimer (~60 FPS)
    void slt_tick();

private:
    void loadLevel(int index);
    void handleInput(float dt);
    void checkStateTransitions();
    void transitionTo(AppState newState);
};

#endif // GAMECONTROLLER_H
