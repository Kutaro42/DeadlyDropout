/**
 * @file bossdoorview.h
 * @brief Declaration of the boss's door view.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef BOSSDOORVIEW_H
#define BOSSDOORVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include "bossdoormodel.h"
#include "config.h"

/**
 * @class BossDoorView
 * @brief Boss's door view.
 *        Display an open srite while it's none open,
 *        then switch to lock after it crossed.
 */
class BossDoorView : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Principal builder.
     */
    BossDoorView(BossDoorModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the door's view.
     */
    void updateView();

private:
    BossDoorModel* itsModel;        ///< Pointer to the bossDoorModel to read it.
    bool           wasLocked;       ///< Boolean to keep the information if the door is locked.
    QPixmap        unlockedPixmap;  ///< QPixmap to display the unlocked door.
    QPixmap        lockedPixmap;    ///< QPixmap to display the locked door.

    /**
     * @brief Create the visual for the door.
     */
    void        buildPixmaps();

    /**
     * @brief Create the visual for the door in case of an error with the sprites loading.
     */
    QPixmap makeFallback(int w, int h, bool isLocked);
};

#endif // BOSSDOORVIEW_H
