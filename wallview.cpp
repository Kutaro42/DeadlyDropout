#include "wallview.h"

WallView::WallView(WallModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
    QPixmap px(":/images/Textures/Environment/Wall/wall1.png");

    // Redimensionne au cas où la texture ne fait pas exactement TILE_SIZE
    if (px.width() != static_cast<int>(model->getWidth()) ||
        px.height() != static_cast<int>(model->getHeight()))
    {
        px = px.scaled(static_cast<int>(model->getWidth()),
                       static_cast<int>(model->getHeight()),
                       Qt::IgnoreAspectRatio,
                       Qt::SmoothTransformation);
    }

    setPixmap(px);
    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_WALL);
}
