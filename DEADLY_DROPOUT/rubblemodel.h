/**
 * @file rubblemodel.h
 * @brief Déclaration du modèle de gravat, projectile tiré par le ZombieLauncher.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef RUBBLEMODEL_H
#define RUBBLEMODEL_H

#include "projectile.h"

// ═══════════════════════════════════════════════════════════════════
//  RubbleModel — projectile concret lancé par ZombieLauncherModel.
//  Hérite de Projectile (→ Entity).
//  Déplacement en ligne droite délégué à Projectile::update().
//  Se désactive dès qu'il touche un mur ou le joueur
//  (via setActive(false) dans LevelController).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class RubbleModel
 * @brief Gravat (projectile) tiré par le zombie lanceur.
 */
class RubbleModel : public Projectile {
public:
    /**
     * @brief Construit un gravat.
     * @param pos    Position de départ (centre du lanceur, ajusté).
     * @param dir    Direction normalisée vers la cible au moment du tir.
     * @param damage Dégâts infligés au joueur à l'impact.
     * @param speed  Vitesse en pixels/s (défaut : Cfg::RUBBLE_SPEED).
     */
    RubbleModel(Vector2f pos,
                Vector2f dir,
                int      damage = Cfg::RUBBLE_DAMAGE,
                float    speed  = Cfg::RUBBLE_SPEED);
};

#endif // RUBBLEMODEL_H
