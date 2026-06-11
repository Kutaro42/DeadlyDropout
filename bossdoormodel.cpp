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
    if (locked) return false;
    if (!playerCollider.intersects(getCollider())) return false;

    // Téléporte de l'autre côté selon l'axe le plus court de la porte.
    // Si la porte est plus large que haute → passage vertical (axe Y).
    // Sinon → passage horizontal (axe X).
    QRectF door = getCollider();
    QRectF player = playerCollider;

    if (door.width() >= door.height()) {
        // Porte horizontale : le joueur vient du haut ou du bas
        bool fromTop = (player.center().y() < door.center().y());
        float newY = fromTop
                         ? static_cast<float>(door.bottom()) + 2.0f
                         : static_cast<float>(door.top()) - static_cast<float>(player.height()) - 2.0f;
        teleportPos = Vector2f(static_cast<float>(player.left()), newY);
    } else {
        // Porte verticale : le joueur vient de la gauche ou de la droite
        bool fromLeft = (player.center().x() < door.center().x());
        float newX = fromLeft
                         ? static_cast<float>(door.right()) + 2.0f
                         : static_cast<float>(door.left()) - static_cast<float>(player.width()) - 2.0f;
        teleportPos = Vector2f(newX, static_cast<float>(player.top()));
    }

    locked = true;
    return true;
}
