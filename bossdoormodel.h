/**
 * @file bossdoormodel.h
 * @brief Declaration of the boss's door model.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef BOSSDOORMODEL_H
#define BOSSDOORMODEL_H

#include "globaldoor.h"

/**
 * @class BossDoorModel
 * @brief single use door : teleport the player in the other side of the door after the first
 *        colision and become locked forever (Can no longer be crossed).
 */
class BossDoorModel : public GlobalDoor
{
public:
    /**
     * @brief Principal builder.
     */
    BossDoorModel(Vector2f pos, float w, float h);

    /**
     *  @return true if the player already crossed it, otherwise false.
     */
    bool isLocked() const { return locked; }

    /**
     * @brief Try to cross the door.
     *        If open and touch by the player, calculate the new position after teleportation,
     *        lock it and return true.
     *        Tente de traverser la porte.
     * @param playerCollider : Player hitbox.
     * @param teleportPos : Player's target position after teleportation.
     * @return true if a teleport is needed, otherwise return false.
     */
    bool tryTraverse(const QRectF& playerCollider, Vector2f& teleportPos);

private:
    bool locked; ///< false = crossable one time, true = permanent wall.
};

#endif // BOSSDOORMODEL_H
