/**
 * @file bossdoormodel.cpp
 * @brief Implémentation du modèle de la porte de boss.
 */

#include "bossdoormodel.h"

BossDoorModel::BossDoorModel(Vector2f pos, float w, float h)
    : GlobalDoor(pos, w, h)
    , locked(false)
{}

bool BossDoorModel::tryTraverse(const QRectF& playerCollider, Vector2f& teleportPos)
{
    // Check teleport condition
    if (locked) return false;
    if (!playerCollider.intersects(getCollider())) return false;

    // Teleport the player on the other side.
    // Horizontal passage (X axis)
    QRectF door = getCollider();
    QRectF player = playerCollider;

    if (door.width() >= door.height()) {
        bool fromTop = (player.center().y() < door.center().y());
        float newY = fromTop
                         ? static_cast<float>(door.bottom()) + 2.0f
                         : static_cast<float>(door.top()) - static_cast<float>(player.height()) - 2.0f;
        teleportPos = Vector2f(static_cast<float>(player.left()), newY);
    }

    //lock the door, no U-turn
    locked = true;
    return true;
}
