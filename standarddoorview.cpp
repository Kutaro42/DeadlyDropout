/**
 * @file standarddoorview.cpp
 * @brief Implémentation de la vue graphique pour la porte standard.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "standarddoorview.h"

StandardDoorView::StandardDoorView(StandardDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), model(m), cachedOpen(false)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());

    // Fermée : marron sombre avec contour crème
    setBrush(QBrush(QColor(90, 55, 20)));
    setPen(QPen(QColor(210, 180, 100), 2));
    setZValue(Cfg::Z_WALL); // même plan que les murs tant que fermée
}

void StandardDoorView::updateView()
{
    if (!model) return;
    bool nowOpen = model->isOpen();
    if (nowOpen == cachedOpen) return; // pas de changement → aucun redraw

    cachedOpen = nowOpen;
    if (nowOpen) {
        // Ouverte : rendu quasi-transparent (sol visible), contour discret
        setBrush(QBrush(QColor(160, 120, 60, 60)));
        setPen(QPen(QColor(210, 180, 100, 100), 1));
        setZValue(Cfg::Z_FLOOR); // passe en dessous des entités
    } else {
        setBrush(QBrush(QColor(90, 55, 20)));
        setPen(QPen(QColor(210, 180, 100), 2));
        setZValue(Cfg::Z_WALL);
    }
}
