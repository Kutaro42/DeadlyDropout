/**
 * @file bulletview.h
 * @brief Declaration of the bullet's view.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef BULLETVIEW_H
#define BULLETVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "bulletmodel.h"
#include "config.h"

/**
 * @class BulletView
 * @brief Bullet's view.
 *        Display a quick bullet across the map.
 */
class BulletView : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Principal builder.
     *        explicit to avoid unwanted implicit conversions with parent.
     */
    explicit BulletView(BulletModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the bullet's view.
     */
    void updateView();

private:
    BulletModel* model;     ///< Pointer to the BulletModel to read it.

    /**
     * @brief Load the bullet sprite.
     * @return QPixmap of the sprite.
     */
    static QPixmap loadSprite();
};

#endif // BULLETVIEW_H
