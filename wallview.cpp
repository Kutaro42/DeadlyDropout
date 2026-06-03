#include "wallview.h"

// Pixmap chargé une seule fois pour toutes les instances (static)
static QPixmap& wallPixmap() {
    static QPixmap px(":Textures/Assets/wall_tile_up.png");
    return px;
}

WallView::WallView(WallModel* model, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    const double w = static_cast<double>(model->getWidth());
    const double h = static_cast<double>(model->getHeight());

    setRect(0, 0, w, h);
    setPos(model->getPosition().toQPointF());

    // Texture tuilée si la tile est plus grande que le pixmap,
    // ou simplement étirée si elle est exactement TILE_SIZE × TILE_SIZE
    const QPixmap& px = wallPixmap();
    if (!px.isNull()) {
        setBrush(QBrush(px));          // Qt tile automatiquement le pixmap
    } else {
        // Fallback couleur si le fichier est introuvable
        setBrush(QBrush(QColor(72, 54, 36)));
    }

    setPen(Qt::NoPen);                 // supprime le contour entre tuiles
    setZValue(Cfg::Z_WALL);
}
// Affichera la texture du mur quand assets.qrc sera complet
