#include "exitdoorview.h"

ExitDoorView::ExitDoorView(ExitDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), model(m), cachedOpen(false)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());
    setBrush(QBrush(QColor(140, 20, 20)));  // fermé : rouge sombre
    setPen(QPen(Qt::yellow, 2));
    setZValue(Cfg::Z_ENTITY);
}

void ExitDoorView::updateView() {
    if (!model) return;
    bool nowOpen = model->isOpen();
    if (nowOpen == cachedOpen) return;   // pas de changement

    cachedOpen = nowOpen;
    if (nowOpen)
        setBrush(QBrush(QColor(30, 180, 60))); // ouvert : vert vif
    else
        setBrush(QBrush(QColor(140, 20, 20))); // fermé  : rouge
}
