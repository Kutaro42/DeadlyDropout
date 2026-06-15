/**
 * @file bossdoormodel.h
 * @brief Déclaration du modèle de la porte de boss.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef BOSSDOORMODEL_H
#define BOSSDOORMODEL_H

#include "globaldoor.h"

/**
 * @class BossDoorModel
 * @brief Porte à usage unique : téléporte le joueur de l'autre côté au premier contact,
 *        puis se verrouille définitivement (obstacle infranchissable dans les deux sens).
 */
class BossDoorModel : public GlobalDoor
{
public:
    BossDoorModel(Vector2f pos, float w, float h);

    /** @return true si le joueur a déjà traversé (porte verrouillée). */
    bool isLocked() const { return locked; }

    /**
     * @brief Tente de traverser la porte.
     *        Si non verrouillée et que le joueur la touche : calcule la position
     *        de téléportation, verrouille la porte, retourne true.
     * @param playerCollider Hitbox du joueur.
     * @param[out] teleportPos Position cible du joueur après téléportation.
     * @return true si la téléportation doit avoir lieu.
     */
    bool tryTraverse(const QRectF& playerCollider, Vector2f& teleportPos);

private:
    bool locked; ///< false = franchissable une fois, true = mur permanent.
};

#endif // BOSSDOORMODEL_H
