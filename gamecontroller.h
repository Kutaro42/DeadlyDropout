/**
 * @file gamecontroller.h
 * @brief Déclaration du contrôleur principal gérant la boucle et la logique de jeu.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

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
#include "levelregistry.h"   // ← remplace l'ancien #include "leveltemplate.h" (LevelData)
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  GameController — contrôleur principal du jeu (couche Controller).
//  Hérite QObject pour utiliser QTimer (boucle de jeu) et connect().
// ═══════════════════════════════════════════════════════════════════

/**
 * @class GameController
 * @brief Chef d'orchestre du jeu in-game (MVC).
 * @details Responsabilités :
 * - Crée et relie GameModel, GameView, LevelController.
 * - Charge un niveau via LevelBuilder en utilisant LevelRegistry.
 * - Lit les entrées clavier depuis GameView à chaque tick.
 * - Appelle LevelController::update() pour la logique.
 * - Déclenche les transitions d'état (PLAYING → GAME_OVER/VICTORY).
 */
class GameController : public QObject {
    Q_OBJECT

private:
    GameView*        itsGameView;       ///< Pointeur vers la vue principale du jeu.
    GameModel*       itsGameModel;      ///< Pointeur vers l'état global du modèle de jeu.
    LevelController* levelCtrl;         ///< Contrôleur spécifique à la gestion du niveau.

    AppState         currentState;      ///< État courant de la boucle de jeu.
    QTimer           gameTimer;         ///< Moteur temporel déclenchant les ticks (~60 FPS).
    QElapsedTimer    elapsedTimer;      ///< Chronomètre pour le calcul du delta-time (dt).
    int              currentLevelIndex; ///< Index (0-based) du niveau actuellement chargé.

public:
    /**
     * @brief Constructeur du contrôleur de jeu.
     * @param parent Objet parent Qt.
     */
    explicit GameController(QObject* parent);
    ~GameController();

    /**
     * @brief Initialise les modèles, la vue et lance la boucle de jeu.
     */
    void init();

    /**
     * @brief Accesseur pour la vue de jeu.
     * @return Le pointeur vers GameView.
     */
    GameView* getView() const { return itsGameView; }

private slots:
    /**
     * @brief Slot appelé à chaque tick du QTimer (environ 60 FPS).
     */
    void slt_tick();

    /**
     * @brief Slot appelé pour afficher l'overlay de pause.
     */
    void sltPauseMenu();

private:
    /**
     * @brief Charge et initialise un niveau spécifique via LevelRegistry.
     * @param index L'index (0-based) du niveau à charger.
     */
    void loadLevel(int index);

    /**
     * @brief Traite les entrées clavier pour les déplacements.
     * @param dt Delta-time en secondes.
     */
    void handleInput(float dt);

    /**
     * @brief Vérifie les conditions de victoire ou de défaite.
     */
    void checkStateTransitions();

    /**
     * @brief Opère la transition vers un nouvel écran de fin de partie.
     * @param newState L'état ciblé (GAME_OVER ou VICTORY).
     */
    void transitionTo(AppState newState);

    void togglePause();
    QGraphicsTextItem* pauseTextOverlay = nullptr; ///< Overlay texte affiché pendant la pause.

signals:
    void sigRestart();
    void sigBackMenu();
};

#endif // GAMECONTROLLER_H
