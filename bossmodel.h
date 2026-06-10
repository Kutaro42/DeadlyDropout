/**
 * @file bossmodel.h
 * @brief Déclaration du modèle du boss gérant son attaque, ses déplacement et autre atribut
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "enemy.h"

/**
 * @class bossModel
 * @brief Modèle du boss s'occupant de la logique du boss (attaque,déplacement,ect)
 */
class bossModel : public Enemy
{
public:

    /**
     * @brief Constructeur du modèle
     * @param pos: Position initial du boss.
     */
    bossModel(Vector2f pos);

    /**
     * @brief Rafraichissement du boss (interval d'attaque, état, déplaement).
     * @param dt: Distance entre le joueur et le boss.
     * @param playerPos: position du joueur.
     */
    void update(float dt, Vector2f playerPos) ;

    /**
     * @brief Attaque le joueur si le cooldown est a zéro.
     * @param pos: modèle du joueur pour infliger des dégats.
     */
    void attack(PlayerModel* player);

private:
    /**
     * @brief Constructeur du modèle.
     * @param playerPos: position du joueur.
     * @param dt : distance entre le boss et le joueur.
     */
    void chase(Vector2f playerPos, float dt);

    /**
     * @brief Vérifie l'état dans lequel doit être le boss.
     * @param playerPos: Position du joueur.
     */
    void updateAIState(Vector2f playerPos);

};

#endif // BOSSMODEL_H
