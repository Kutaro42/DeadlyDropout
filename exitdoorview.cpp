/**
 * @file exitdoorview.cpp
 * @brief Implementation of the graphical view for the exit door.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#include "exitdoorview.h"

ExitDoorView::ExitDoorView(ExitDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), model(m), cachedOpen(false)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());
    
    // Initial visual configuration
    setBrush(QBrush(QColor(140, 20, 20)));
    setPen(QPen(Qt::yellow, 2));
    setZValue(Cfg::Z_ENTITY);
}

void ExitDoorView::updateView() {
    if (!model) return;
    bool nowOpen = model->isOpen();
    
    // Optimization to avoid unnecessary redrawing if the state has not changed.
    if (nowOpen == cachedOpen) return;

    cachedOpen = nowOpen;
    if (nowOpen)
        setBrush(QBrush(QColor(30, 180, 60))); // open light green
    else
        setBrush(QBrush(QColor(140, 20, 20))); // close : red
}
