/**
 * @file standarddoorview.h
 * @brief Graphical view of the standard door.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef STANDARDDOORVIEW_H
#define STANDARDDOORVIEW_H

#include <QPainter>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include "standarddoormodel.h"

// ═══════════════════════════════════════════════════════════════════
//  StandardDoorView — visual representation of the standard door.
//  Dark brown color = closed, beige/gold = open.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class StandardDoorView
 * @brief Qt rendering of the standard door with open/closed color coding.
 */
class StandardDoorView : public QGraphicsRectItem {
private:
    StandardDoorModel* itsModel;   ///< Reference to the logical model.
    bool               cachedOpen; ///< Cached state to avoid unnecessary redraws.

    bool           wasLocked;
    QPixmap        unlockedPixmap;
    QPixmap        lockedPixmap;


public:
    /**
     * @brief Constructor.
     * @param model  Source model.
     * @param parent Qt parent item (optional).
     */
    explicit StandardDoorView(StandardDoorModel* model,
                              QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the color according to the model's open/closed state.
     */
    void updateView();

    /**
     * @brief Fallback if textures are not found.
     */
    QPixmap makeFallback(int w, int h, bool isLocked);

    /**
     * @brief Finds, builds and loads the textures.
     */
    void buildPixmaps();
};

#endif // STANDARDDOORVIEW_H
