/**
 * @file bulletmodel.h
 * @brief Modèle d'une balle tirée par le pistolet du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#ifndef BULLETMODEL_H
#define BULLETMODEL_H

#include "projectile.h"
#include "config.h"

/**
 * @class BulletModel
 * @brief Projectile tiré par le joueur (pistolet semi-auto).
 *
 * Hérite de Projectile : déplacement rectiligne à vitesse constante.
 * Se désactive (setActive(false)) dès collision avec un mur ou un ennemi,
 * pris en charge par LevelController::checkBulletCollisions().
 *
 * Dégâts : 12 pts de base + bonus adaptatif 2-5 + éventuel critique ×2 (GDD).
 * Vitesse : 500 px/s (GDD).
 */
class BulletModel : public Projectile
{
public:
    /**
     * @brief Construit une balle.
     * @param pos    Position de départ (bouche du pistolet).
     * @param dir    Direction normalisée vers le curseur souris.
     * @param damage Dégâts déjà calculés (base + bonus + critique) par GunModel.
     * @param speed  Vitesse en px/s (Cfg::BULLET_SPEED par défaut).
     */
    BulletModel(Vector2f pos,
                Vector2f dir,
                int      damage = Cfg::BULLET_DAMAGE,
                float    speed  = Cfg::BULLET_SPEED);
};

#endif // BULLETMODEL_H
