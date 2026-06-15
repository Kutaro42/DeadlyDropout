/**
 * @file floorview.cpp
 * @brief Implémentation de FloorView.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "floorview.h"

// ── Cache statique du tileset de sol ─────────────────────────────
const QPixmap& FloorView::tileset()
{
    // Chargé une seule fois pour toute l'application.
    // Adaptez le chemin selon l'organisation de vos ressources Qt.
    static QPixmap ts(":/images/Textures/Environment/Floor/floor_tileset.png");
    return ts;
}

// ── Découpe la tile N dans le tileset ────────────────────────────
QPixmap FloorView::extractTile(int tileIndex, int tilesetCols,
                               int tileW, int tileH)
{
    const int idx = tileIndex - 1;          // 0-based
    const int col = idx % tilesetCols;
    const int row = idx / tilesetCols;

    QPixmap tile = tileset().copy(col * tileW, row * tileH, tileW, tileH);

    // Redimensionne si la tile du tileset ne fait pas exactement TILE_SIZE
    if (tile.width() != tileW || tile.height() != tileH)
        tile = tile.scaled(tileW, tileH,
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
    return tile;
}

// ── Constructeur ─────────────────────────────────────────────────
FloorView::FloorView(FloorModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
    const int tileW = static_cast<int>(model->getWidth());
    const int tileH = static_cast<int>(model->getHeight());

    setPixmap(extractTile(model->getTileIndex(),
                          model->getTilesetCols(),
                          tileW, tileH));

    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_FLOOR);   // En dessous des murs (Z_WALL = 1.0) et des entités
}
