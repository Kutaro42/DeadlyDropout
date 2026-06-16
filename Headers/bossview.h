/**
 * @file bossview.h
 * @brief Declaration of the boss's view.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "bossmodel.h"
#include "config.h"

/**
 * @class BossView
 * @brief Boss's view.
 *        Display the different boss's sprites and the area damage.
 */
class BossView : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Principal builder.
     */
    BossView(bossModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Class's destructive.
     */
    ~BossView();

    /**
     * @brief Updates the boss's view and its patern.
     */
    void updateView(float dt);

private:
    bossModel* model;    ///< Boss's model to read it.

    static constexpr int IDLE  = 0;         ///< Idle state constant.
    static constexpr int WALK  = 1;         ///< Walk state constant (Chasing).
    static constexpr int HURT  = 2;         ///< Injured state constant.
    static constexpr int DEAD  = 3;         ///< Dead state constant.
    static constexpr int STATE_COUNT = 4;   ///< number of different animation state managed.
    static constexpr int DIR_COUNT   = 4;   ///< Total number of direction (DOWN, UP, LEFT, RIGHT).
    static constexpr int FRAME_W     = 48;  ///< Width of a single frame read from the sprite sheet
    static constexpr int FRAME_H     = 48;  ///< Height of a single frame read from the sprite sheet.

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];  ///< Vector with all the sprite for every state

    int   currentFrame; ///< Index of the image currently being displayed.
    float frameTimer;   ///< Clock that accumulates the delta time to trigger a frame change.
    float animSpeed;    ///< Delay (in seconds) before changing the walking image.
    float animSpeedDeath;
    int   currentDir;   ///< An index storing the last calculated heading.
    bool  deadAnimFinished; ///< True once the DEAD animation has played in full.

    // ─── Attack zone (pattern 2) ───────────────────────────────
    QVector<QGraphicsRectItem*> zoneItems;  ///< Qt items for area tiles.
    bool zoneVisible;                       ///< true if the items are in the scene.

    void showZone();   ///< Creates and adds zone items to the scene.
    void hideZone();   ///< Removes and destroys area items.


    /**
     * @brief Converts the FacingDirection enumeration to an integer array index.
     * @param d : Cardinal direction assessed.
     * @return Corresponding index (0 = Down, 1 = Up, 2 = Left, 3 = Right).
     */
    static int  dirIdx(FacingDirection d);

    /**
     * @brief Estimate the dominant cardinal direction from a kinematic displacement vector.
     * @param vel : The current velocity vector.
     * @return The most likely FacingDirection.
     */
    static FacingDirection velocityToDir(Vector2f vel);

    /**
     * @brief Extracts and resizes the various frames from a horizontal sprite sheet.
     * @param frames : a list of images (QVector) in which to store the cropped sections.
     * @param path : Location of the PNG file in the Qt resources.
     */
    void        loadSheet(QVector<QPixmap>& frames, const QString& path);

    /**
     * @brief Displays a green square with the letter ‘B’ if a resource is missing.
     * @return An image generated dynamically in the correct format.
     */
    QPixmap     makeFallback();

    /**
     * @brief Advances the frame counter of the current animation.
     * @param dt : A fraction of a second has passed.
     * @param frames : Stack of images making up the targeted animation.
     */
    void        advanceFrames(float dt, const QVector<QPixmap>& frames);

};

#endif // BOSSVIEW_H
