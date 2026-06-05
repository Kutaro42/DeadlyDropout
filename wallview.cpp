#include "wallview.h"

// ── Cache statique du tileset ─────────────────────────────────────
const QPixmap& WallView::tileset()
{
    // Chargé une seule fois pour toute l'application.
    // Chemin à adapter selon l'organisation de tes ressources Qt.
    static QPixmap ts(":/images/Textures/Environment/Wall/wall_tileset.png");
    return ts;
}

// ── Découpe la tile N dans le tileset ─────────────────────────────
QPixmap WallView::extractTile(int tileIndex, int tilesetCols,
                              int tileW, int tileH)
{
    const int idx  = tileIndex - 1;          // 0-based
    const int col  = idx % tilesetCols;
    const int row  = idx / tilesetCols;

    QPixmap tile = tileset().copy(col * tileW, row * tileH, tileW, tileH);

    // Redimensionne si la tile du tileset ne fait pas exactement TILE_SIZE
    if (tile.width() != tileW || tile.height() != tileH)
        tile = tile.scaled(tileW, tileH,
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
    return tile;
}

// ── Constructeur ─────────────────────────────────────────────────
WallView::WallView(WallModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
    const int tileW = static_cast<int>(model->getWidth());
    const int tileH = static_cast<int>(model->getHeight());

    setPixmap(extractTile(model->getTileIndex(),
                          model->getTilesetCols(),
                          tileW, tileH));

    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_WALL);
}
