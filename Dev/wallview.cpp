#include "wallview.h"

WallView::WallView(WallModel* model, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());
    setBrush(QBrush(QColor(72, 54, 36)));   // brique sombre
    setPen(QPen(QColor(50, 38, 24), 1));
    setZValue(Cfg::Z_WALL);
}
