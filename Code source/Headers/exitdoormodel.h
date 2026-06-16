/**
 * @file exitdoormodel.h
 * @brief Declaration of the end-of-level door model.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef EXITDOORMODEL_H
#define EXITDOORMODEL_H

#include "entity.h"
#include "config.h"
#include "globaldoor.h"


// ═══════════════════════════════════════════════════════════════════
//  ExitDoorModel — level exit zone.
//  Closed by default.
//  The player must be in this area AND the door must be
//  open to trigger the victory.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ExitDoorModel
 * @brief Represents the portal that allows you to advance to the next level.
 */
class ExitDoorModel : public GlobalDoor {
private:
    bool open; ///< Indicates whether the door is unlocked.

public:
    /**
     * @brief Constructor of the exit zone.
     */
    ExitDoorModel(Vector2f pos, float w, float h);
    void update(float dt) override;

    void openDoor()      { open = true;  }
    bool isOpen()  const { return open;  }

};

#endif // EXITDOORMODEL_H
