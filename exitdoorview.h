#ifndef EXITDOORVIEW_H
#define EXITDOORVIEW_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include "exitdoormodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ExitDoorView — représentation visuelle de la porte de sortie.
//  Couleur rouge foncé = fermée, vert vif = ouverte.
//  Mettre à jour via updateView() à chaque tick.
// ═══════════════════════════════════════════════════════════════════
class ExitDoorView : public QGraphicsRectItem {
private:
    ExitDoorModel* model;
    bool           cachedOpen;

public:
    explicit ExitDoorView(ExitDoorModel* model, QGraphicsItem* parent = nullptr);

    // À appeler chaque tick pour synchroniser l'état visuel
    void updateView();
};

#endif // EXITDOORVIEW_H
