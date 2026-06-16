/**
 * @file standarddoormodel.h
 * @brief Specifications for the standard door model (which opens when the player touches it).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef STANDARDDOORMODEL_H
#define STANDARDDOORMODEL_H

#include "globaldoor.h"
#include "config.h"

class PlayerModel; 

// ═══════════════════════════════════════════════════════════════════
//  StandardDoorModel — junction door between rooms/corridors.
//  Closed by default.
//  Opens as soon as the player makes contact with it and
//  stays open until the end of the level.
//  Once open, it no longer creates a collision (the player
//  can pass through freely).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class StandardDoorModel
 * @brief Standard door that opens on simple contact with the player.
 */
class StandardDoorModel : public GlobalDoor {
private:
    bool open; ///< Indicates whether the door is open (and therefore passable).

public:
    /**
     * @brief Constructor.
     * @param pos Top-left position in the scene.
     * @param w   Width in pixels.
     * @param h   Height in pixels.
     */
    StandardDoorModel(Vector2f pos, float w, float h);

    void update(float dt) override;

    /**
     * @brief Opens the door (irreversible during the level).
     */
    void openDoor() { open = true; }

    /**
     * @brief Indicates whether the door is currently open.
     * @return true if open (passable), false otherwise.
     */
    bool isOpen() const { return open; }

    /**
     * @brief Opens the door if the player touches it.
     *        Must be called by LevelController every tick.
     * @param player Player model.
     * @return true if the door has just been opened, false otherwise.
     */
    bool tryOpen(const PlayerModel& player);
};

#endif // STANDARDDOORMODEL_H
