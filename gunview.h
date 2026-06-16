/**
 * @file gunview.h
 * @brief View of the pistol: attached to the player, oriented toward the mouse cursor.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#ifndef GUNVIEW_H
#define GUNVIEW_H

#include "bulletview.h"
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "gunmodel.h"
#include "playermodel.h"
#include "bulletmodel.h"
#include "config.h"

/**
 * @class GunView
 * @brief Graphics item for the pistol: anchored on the player and rotated toward the cursor.
 *
 * Positioning strategy:
 *  - The rotation pivot is set to the LEFT edge of the sprite (anchor point on the player).
 *  - setRotation(deg) rotates the sprite around that pivot → no positional drift.
 *  - When the cursor is to the left of the player, the sprite is pre-flipped vertically
 *    via a mirrored QPixmap (NOT via setTransform) to avoid conflicting with setRotation.
 *
 * Sprite loading order (first non-null wins):
 *  1. :/images/Textures/Player/Gun/gun.png
 *  2. :/images/Textures/Player/gun.png
 *  3. Procedural fallback (dark-grey body + barrel, GUN_W × GUN_H).
 *
 * Expected PNG convention: barrel pointing RIGHT (angle 0°), transparent background,
 * any source dimensions (scaled to GUN_W × GUN_H at runtime).
 *
 * Bullet management:
 *  A BulletView is created for each new BulletModel and removed from the scene
 *  as soon as the model becomes inactive, keeping scene items in sync with the model.
 */
class GunView : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Constructs GunView and sets the rotation anchor point.
     * @param gunModel    Logical gun model owning the BulletModel list.
     * @param playerModel Player model providing the anchor position.
     * @param parent      Qt parent graphics item (nullptr = directly in scene).
     */
    explicit GunView(GunModel*      gunModel,
                     PlayerModel*   playerModel,
                     QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates gun position/rotation and synchronises bullet views.
     *
     * Called once per game-loop tick by GameView::updateAllViews().
     * The cursor position must be in scene coordinates (not widget coordinates)
     * so that the aim direction is correct regardless of camera scroll.
     *
     * @param dt         Delta-time in seconds (unused currently, kept for API consistency).
     * @param mouseScene Current cursor position in scene coordinates.
     * @param scene      Qt scene used to add/remove BulletView items.
     */
    void updateView(float dt, QPointF mouseScene, QGraphicsScene* scene);

private:
    GunModel*    gun;     ///< Non-owning pointer to the gun model.
    PlayerModel* player;  ///< Non-owning pointer to the player model (for position).
    bool         flipped; ///< True when the sprite is currently displayed mirrored vertically.

    QVector<BulletView*>  bulletViews;  ///< Scene items for bullets in flight.
    QVector<BulletModel*> bulletModels; ///< Parallel array: same indices as bulletViews.

    /**
     * @brief Keeps bulletViews in sync with the gun model's active bullet list.
     *
     * Creates new BulletView entries for newly spawned bullets and removes entries
     * for bullets that have become inactive (hit a wall or enemy).
     * Runs in O(n) per tick where n = number of active bullets.
     *
     * @param scene Qt scene for item insertion/removal.
     */
    void syncBulletViews(QGraphicsScene* scene);

    /**
     * @brief Loads the gun sprite from resources or generates a procedural fallback.
     *
     * Result is cached in a static local variable so the file is only read once
     * per process lifetime regardless of how many GunView instances are created.
     *
     * @return Scaled QPixmap ready for setPixmap().
     */
    static QPixmap loadGunSprite();

    static constexpr int GUN_W = 32; ///< Display width of the gun sprite (px).
    static constexpr int GUN_H = 32; ///< Display height of the gun sprite (px).
};

#endif // GUNVIEW_H