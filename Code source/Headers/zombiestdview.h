/**
 * @file zombiestdview.h
 * @brief Declaration of the ZombieStdView class, which manages the visual appearance of the standard zombie.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIESTDVIEW_H
#define ZOMBIESTDVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "zombiestdmodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdView — visual representation of the standard zombie.
//
//  Spritesheets (prefix /images, ressources.qrc):
//    Textures/Enemy/Standard/Idle/{Down,Up,Left,Right}.png → 2 frames 32×32
//    Textures/Enemy/Standard/Move/{Down,Up,Left,Right}.png → 2 frames 32×32
//
//  Direction inferred from the model's velocity vector.
//  Fallback: green rectangle annotated "Z" if sprite is missing.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieStdView
 * @brief Graphics component (View) of the standard zombie.
 * * Inherits from QGraphicsPixmapItem for Qt rendering. Manages walk and idle animations based on the AI state (Idle/Chasing).
 */
class ZombieStdView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Zombie view constructor.
     * @param model Pointer to the zombie's logical data (ZombieStdModel).
     * @param parent Parent graphics item.
     */
    explicit ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent = nullptr);
    
    /**
     * @brief Updates the sprite position and current animation.
     * @param dt Delta-time in seconds elapsed since the last cycle.
     */
    void updateView(float dt);

private:
    ZombieStdModel* model; ///< Reference to the zombie model managing state data (position, velocity).

    static constexpr int IDLE  = 0;         ///< Constant for the idle state.
    static constexpr int WALK  = 1;         ///< Constant for the moving state (Chasing).
    static constexpr int HURT  = 2;         ///< Constant for the hurt state.
    static constexpr int DEAD  = 3;         ///< Constant for the dead state.
    static constexpr int STATE_COUNT = 4;   ///< Total number of managed animation states.
    static constexpr int DIR_COUNT   = 4;   ///< Total number of directions (DOWN, UP, LEFT, RIGHT).
    static constexpr int FRAME_W     = 32;  ///< Width of one frame read from the spritesheet.
    static constexpr int FRAME_H     = 32;  ///< Height of one frame read from the spritesheet.

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< 2D sprite matrix (state × direction).

    int   currentFrame; ///< Index of the currently rendered frame.
    float frameTimer;   ///< Clock accumulating delta-time to trigger the next frame change.
    float animSpeed;    ///< Wait time (in s) before switching walk frames.
    float animSpeedDeath;
    int   currentDir;   ///< Index storing the last computed direction.
    bool  deadAnimFinished; ///< True once the DEAD animation has been played in full.

    /**
     * @brief Converts a FacingDirection enum to an integer table index.
     * @param d Cardinal direction to evaluate.
     * @return Corresponding index (0 = Down, 1 = Up, 2 = Left, 3 = Right).
     */
    static int  dirIdx(FacingDirection d);
    
    /**
     * @brief Estimates the dominant cardinal direction from a kinematic velocity vector.
     * @param vel The current velocity vector.
     * @return The most likely FacingDirection.
     */
    static FacingDirection velocityToDir(Vector2f vel);
    
    /**
     * @brief Extracts and scales the frames from a horizontal sprite sheet.
     * @param frames Image list (QVector) to store the sliced frames.
     * @param path Location of the PNG file in the Qt resources.
     */
    void        loadSheet(QVector<QPixmap>& frames, const QString& path);
    
    /**
     * @brief Builds a green square with the initial "Z" when a resource is missing.
     * @return A dynamically generated image in the correct format.
     */
    QPixmap     makeFallback();
    
    /**
     * @brief Advances the frame reader of the current animation.
     * @param dt Fraction of a second elapsed.
     * @param frames Stack of images composing the target animation.
     */
    void        advanceFrames(float dt, const QVector<QPixmap>& frames);
};

#endif // ZOMBIESTDVIEW_H
