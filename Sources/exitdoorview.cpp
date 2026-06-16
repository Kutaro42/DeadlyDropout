/**
 * @file exitdoorview.cpp
 * @brief Implémentation de la vue graphique pour la porte de sortie.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "Headers/exitdoorview.h"

ExitDoorView::ExitDoorView(ExitDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), model(m), cachedOpen(false)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());
    
    // Configuration visuelle initiale
    setBrush(QBrush(QColor(140, 20, 20)));  // fermé : rouge sombre
    setPen(QPen(Qt::yellow, 2));
    setZValue(Cfg::Z_ENTITY);
}

void ExitDoorView::updateView() {
    if (!model) return;
    bool nowOpen = model->isOpen();
    
    // Optimisation pour éviter un redraw inutile si l'état n'a pas basculé.
    if (nowOpen == cachedOpen) return;   // pas de changement

    cachedOpen = nowOpen;
    if (nowOpen)
        setBrush(QBrush(QColor(30, 180, 60))); // ouvert : vert vif
    else
        setBrush(QBrush(QColor(140, 20, 20))); // fermé  : rouge
}
