/**
 * @file zombielauncherview.h
 * @brief Declaration of the zombie launcher's graphical view.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIELAUNCHERVIEW_H
#define ZOMBIELAUNCHERVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QVector>
#include <QPixmap>
#include "zombielaunchermodel.h"
#include "rubbleview.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieLauncherView — visual representation of the launcher zombie.
//
//  Expected spritesheets (Qt resources):
//    :/images/Textures/Enemy/Launcher/Idle/{Down,Up,Left,Right}.png
//    :/images/Textures/Enemy/Launcher/Move/{Down,Up,Left,Right}.png
//  → 2 frames of 32×32 side by side, like ZombieStdView.
//  If missing, a purple rectangle annotated "L" is used.
//
//  RubbleView management:
//    On every tick, updateView() synchronizes the RubbleView list
//    with the RubbleModel instances created by the model, then calls
//    pruneInactiveRubbles() to free memory.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieLauncherView
 * @brief Qt view of the launcher zombie, also managing the views of the rubble it fires.
 */
class ZombieLauncherView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructor.
     * @param model  Source model.
     * @param scene  Qt scene in which to add the RubbleView instances.
     * @param parent Qt parent item.
     */
    explicit ZombieLauncherView(ZombieLauncherModel* model,
                                QGraphicsScene*      scene,
                                QGraphicsItem*       parent = nullptr);

    /**
     * @brief Updates the launcher's animation and the views of its rubble projectiles.
     * @param dt Delta-time in seconds.
     */
    void updateView(float dt);

private:
    ZombieLauncherModel* model;          ///< Source model.
    QGraphicsScene*      scene;          ///< Scene into which new RubbleView instances are injected.
    QVector<RubbleView*> rubbleViews;    ///< Views of rubble currently in flight.

    // ─── Animation ────────────────────────────────────────────
    static constexpr int IDLE        = 0;
    static constexpr int WALK        = 1;
    static constexpr int HURT        = 2;  ///< Constant for the hurt state.
    static constexpr int DEAD        = 3;  ///< Constant for the dead state.
    static constexpr int STATE_COUNT = 4;
    static constexpr int DIR_COUNT   = 4;
    static constexpr int FRAME_W     = 32; ///< Width of one source frame (px).
    static constexpr int FRAME_H     = 32; ///< Height of one source frame (px).

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< Frames per state × direction.

    int   currentFrame; ///< Current frame index.
    float frameTimer;   ///< Time elapsed on the current frame.
    float animSpeed;    ///< Display duration of one frame in seconds.
    float animSpeedDeath;
    int   currentDir;   ///< Current direction (index 0-3).
    bool  deadAnimFinished; ///< True once the DEAD animation has been played in full.

    // ─── Helpers ──────────────────────────────────────────────
    static int             dirIdx(FacingDirection d);
    static FacingDirection velocityToDir(Vector2f vel);
    void                   loadSheet(QVector<QPixmap>& frames, const QString& path);
    QPixmap                makeFallback();
    void                   advanceFrames(float dt, const QVector<QPixmap>& frames);

    /**
     * @brief Synchronizes rubbleViews with the model's RubbleModel instances.
     * Creates a RubbleView for each new rubble, then removes
     * views of rubble that have become inactive.
     */
    void syncRubbleViews();
};

#endif // ZOMBIELAUNCHERVIEW_H
