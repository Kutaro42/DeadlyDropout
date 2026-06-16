/**
 * @file gamecontroller.h
 * @brief Declaration of GameController, the main controller managing the game loop and logic.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "marketcontroller.h"
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include "gameview.h"
#include "gamemodel.h"
#include "levelcontroller.h"
#include "levelbuilder.h"
#include "leveltemplate.h"
#include "levelregistry.h"  
#include "config.h"
#include "musiccontroller.h"

/**
 * @class GameController
 * @brief Orchestrator of the in-game MVC loop.
 *
 * Responsibilities (single-responsibility breakdown):
 *  - Creates and links GameModel, GameView and LevelController.
 *  - Loads a level via LevelBuilder using descriptors from LevelRegistry.
 *  - Reads keyboard/mouse input from GameView on every tick.
 *  - Delegates physics and AI to LevelController::update().
 *  - Detects state transitions (PLAYING → GAME_OVER / VICTORY / MARKET).
 *  - Opens MarketController between levels without exposing it to the view.
 *
 * The QTimer drives the game loop at ~60 FPS (Cfg::TIMER_MS).
 * QElapsedTimer measures real elapsed milliseconds so that dt is accurate
 * even if a tick fires slightly late (avoids physics drift on slow machines).
 */
class GameController : public QObject {
    Q_OBJECT

private:
    GameView*         itsGameView;          ///< Owned pointer to the main game view.
    GameModel*        itsGameModel;         ///< Owned pointer to the global game state.
    LevelController*  levelCtrl;            ///< Handles per-level entity logic and collisions.
    MarketController* itsMarketController;  ///< Manages the between-level upgrade shop.

    AppState         currentState;         ///< Drives which branch of slt_tick() executes.
    QTimer           gameTimer;            ///< Fires slt_tick() at ~60 FPS.
    QElapsedTimer    elapsedTimer;         ///< Measures real elapsed time for dt calculation.
    int              currentLevelIndex;    ///< 0-based index of the currently loaded level.

public:
    /**
     * @brief Constructs GameController and allocates all owned sub-objects.
     * @param parent Qt parent object (usually AppController).
     */
    explicit GameController(QObject* parent);
    ~GameController();

    /**
     * @brief Returns the player model for external queries (e.g. AppController HUD).
     * @return Pointer to PlayerModel, or nullptr if the model is not yet initialised.
     */
    PlayerModel* getPlayer() const {
        return itsGameModel ? itsGameModel->getPlayer() : nullptr;
    }

    /**
     * @brief Wires all signals, loads the first level and starts the game loop.
     *
     * @param startMusic If true, picks a random in-game track.
     *                   Pass false on "Restart" to avoid cutting the current track mid-play.
     */
    void init(bool startMusic = true);

    /**
     * @brief Returns the game view so AppController can embed it in the main window.
     * @return Pointer to GameView.
     */
    GameView* getView() const { return itsGameView; }

private slots:
    /**
     * @brief Main game-loop tick — called by gameTimer at ~60 FPS.
     *
     * Computes dt, dispatches input, updates model, checks transitions
     * and requests a view repaint, in that order.
     */
    void slt_tick();

    /**
     * @brief Slot triggered when the player chooses "Back to menu" from the pause overlay.
     *
     * Hides the pause overlay and bubbles sigBackMenu up to AppController so
     * the state machine transition (PLAYING → MENU) stays in AppController.
     */
    void sltPauseMenu();

private:
    /**
     * @brief Loads and initialises a level by index using LevelRegistry.
     *
     * Clears the current scene, asks LevelBuilder to populate model + scene,
     * then repositions the player at the new spawn point.
     * LevelRegistry::init() MUST be called before the first loadLevel() call
     * to avoid a modulo-by-zero crash.
     *
     * @param index 0-based level index.
     */
    void loadLevel(int index);

    /**
     * @brief Reads the keyboard state from GameView and moves the player.
     *
     * Movement uses AZERTY (Z/Q/S/D) and arrow keys as specified in the GDD
     * (Step 5 — Movement rules).  Diagonal movement is supported naturally
     * because multiple keys can be held simultaneously.
     *
     * @param dt Delta-time in seconds.
     */
    void handleInput(float dt);

    /**
     * @brief Checks for win/loss conditions and level-completion after each tick.
     *
     * Separated from slt_tick() to keep the tick function readable.
     * Triggers loadLevel(), MarketController::open() or transitionTo() as needed.
     */
    void checkStateTransitions();

    /**
     * @brief Moves the application to a terminal state (GAME_OVER or VICTORY).
     *
     * Stops the game timer so no further ticks execute, then asks the view
     * to display the appropriate overlay message.
     *
     * @param newState Target state (AppState::GAME_OVER or AppState::VICTORY).
     */
    void transitionTo(AppState newState);

    /**
     * @brief Toggles between PLAYING and PAUSED states.
     *
     * Also restarts elapsedTimer on resume to prevent a large dt spike
     * that would cause entities to jump after a long pause.
     */
    void togglePause();

signals:
    /// @brief Bubbled up to AppController to re-create a fresh GameController (full restart).
    void sigRestart();
    /// @brief Bubbled up to AppController to return to the main menu.
    void sigBackMenu();
};

#endif // GAMECONTROLLER_H