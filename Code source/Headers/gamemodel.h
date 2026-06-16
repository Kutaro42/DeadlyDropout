/**
 * @file gamemodel.h
 * @brief Declaration of GameModel, the central model storing all in-game entities.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "bossmodel.h"
#include "playermodel.h"
#include "levelmodel.h"

/**
 * @class GameModel
 * @brief Root model aggregating persistent game-session data (MVC Model layer).
 *
 * Owns exactly one PlayerModel and one LevelModel at any time.
 * It does NOT own game logic — that responsibility belongs to GameController
 * and LevelController.  Keeping data and logic separate means the model can
 * be serialised, reset or inspected by tests without running the game loop.
 *
 * The boss pointer is kept here (rather than only in LevelModel) so that
 * GameController can query boss state for music/UI purposes without having
 * to search the enemy list every frame.
 */
class GameModel : public QObject {
    Q_OBJECT
private:
    PlayerModel* player;            ///< Owned player model; nullptr until initPlayer() is called.
    bossModel*   boss;              ///< Convenience pointer to the current boss (may be nullptr).
    LevelModel*  level;             ///< Structural and entity data for the current level.
    int          currentLevelIndex; ///< 0-based index of the level currently loaded.

public:
    /**
     * @brief Constructs GameModel and allocates an empty LevelModel.
     *
     * PlayerModel is NOT created here because the spawn position is only known
     * after LevelBuilder::build() has parsed the level descriptor.
     */
    GameModel();
    ~GameModel();

    /**
     * @brief Creates (or resets) the player at the given spawn position.
     *
     * Called by GameController after LevelBuilder has populated the level so
     * that the spawn coordinate is valid.  Also wires the level's key-obtained
     * signal to GameModel::obtainKey so the player inventory stays in sync.
     *
     * @param spawnPos World-space position where the player should appear.
     */
    void initPlayer(Vector2f spawnPos);

    /**
     * @brief Slot: forwards the key-collected event to the player inventory.
     *
     * Decouples LevelModel (which detects key pickup) from PlayerModel
     * (which tracks inventory) — neither class needs a direct pointer to the other.
     */
    void obtainKey();

    void setCurrentLevelIndex(int i) { currentLevelIndex = i; }

    PlayerModel* getPlayer() const { return player; }

    /**
     * @brief Searches the level's enemy list and returns the first boss found.
     *
     * Linear search is acceptable because it is only called on state transitions
     * (not every frame) and boss counts per level are at most 1 (GDD Step 9).
     *
     * @return Pointer to the boss Enemy, or nullptr if none exists.
     */
    Enemy* getBoss();

    LevelModel* getLevel()      const { return level;  }
    int          getLevelIndex() const { return currentLevelIndex; }

    int a = 1; ///< @todo Remove — debug artifact, no longer needed.
};

#endif // GAMEMODEL_H