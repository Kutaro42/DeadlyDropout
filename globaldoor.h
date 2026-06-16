/**
 * @file globaldoor.h
 * @brief Declaration of GlobalDoor, the common base class for all interactive doors.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef GLOBALDOOR_H
#define GLOBALDOOR_H
#include "entity.h"

/**
 * @class GlobalDoor
 * @brief Base entity from which standard doors, boss doors and level-exit zones inherit.
 *
 * The GDD (Step 5 — Entity rules) defines three distinct door behaviours:
 *   - Standard door  : blocks passage, opened by player interaction.
 *   - Boss door      : teleports the player and locks behind them.
 *   - Level-exit zone: detects the player + key and triggers the next level.
 *
 * Rather than duplicating position/collision data in each subclass, GlobalDoor
 * inherits Entity and acts as a single structural anchor.  Behaviour differences
 * are implemented in the subclasses so that LevelController can treat all doors
 * polymorphically when iterating the entity list.
 */
class GlobalDoor : public Entity
{
public:
    /**
     * @brief Constructs a door entity at a given position with explicit dimensions.
     *
     * Dimensions are explicit rather than fixed constants because the GDD specifies
     * different visual sizes for standard doors, boss doors and exit zones.
     *
     * @param pos Top-left position of the door's collision box in world coordinates.
     * @param w   Width of the collision box in pixels.
     * @param h   Height of the collision box in pixels.
     */
    GlobalDoor(Vector2f pos, float w, float h);
};

#endif // GLOBALDOOR_H