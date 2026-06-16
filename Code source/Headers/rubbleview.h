/**
 * @file rubbleview.h
 * @brief Display of the graphical view of the rubble (projectile fired by the zombie launcher).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef RUBBLEVIEW_H
#define RUBBLEVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "rubblemodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  RubbleView — graphical representation of a rubble projectile.
//
//  Expected sprite (Qt resources):
//    :/images/Textures/Enemy/Launcher/rubble.png  (16×16 or 32×32)
//  If the file is missing, a grey-brown square is drawn as fallback.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class RubbleView
 * @brief Qt graphics item synchronized with a RubbleModel.
 */
class RubbleView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructor.
     * @param model  Pointer to the rubble model.
     * @param parent Qt parent item (nullptr by default).
     */
    explicit RubbleView(RubbleModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the graphical position and hides the item if inactive.
     */
    void updateView();

private:
    RubbleModel* model; ///< Source model.

    /// Loads the sprite from resources; builds a fallback if missing.
    static QPixmap loadSprite();
};

#endif // RUBBLEVIEW_H
