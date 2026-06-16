#ifndef WALLVIEW_H
#define WALLVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "wallmodel.h"
#include "config.h"

/**
 * @class WallView
 * @brief Affiche un mur en découpant la bonne tile depuis le tileset.
 *
 * Le tileset doit être chargé une seule fois (cache statique).
 * La tile est sélectionnée via l'index stocké dans WallModel :
 *   col = (index - 1) % tilesetCols
 *   row = (index - 1) / tilesetCols
 */
class WallView : public QGraphicsPixmapItem {
public:
    explicit WallView(WallModel* model, QGraphicsItem* parent = nullptr);

private:
    /// Retourne le tileset (chargé une seule fois).
    static const QPixmap& tileset();

    /// Découpe et retourne la tile correspondant à l'index.
    static QPixmap extractTile(int tileIndex, int tilesetCols,
                               int tileW, int tileH);
};

#endif // WALLVIEW_H
