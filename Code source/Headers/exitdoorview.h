/**
 * @file exitdoorview.h
 * @brief Graphical view declaration for the exit door.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef EXITDOORVIEW_H
#define EXITDOORVIEW_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include "exitdoormodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ExitDoorView — visual representation of the exit door.
//  Dark red = closed, bright green = open.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ExitDoorView
 * @brief A Qt rendering of the door that changes its color based on its lock status.
 */
class ExitDoorView : public QGraphicsRectItem {
private:
    ExitDoorModel* model;       ///< Reference to the logical model of the gate.
    bool           cachedOpen;  ///< State caching to reduce brush calculations.

public:
    /**
     * @brief Builder of the graphical view of the exit door.
     * @param model : Source physical model.
     * @param parent : Parent element in the Qt graphics scene.
     */
    explicit ExitDoorView(ExitDoorModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the graphical attributes (color, size) based on the model's state.
     */
    void updateView();
};

#endif // EXITDOORVIEW_H
