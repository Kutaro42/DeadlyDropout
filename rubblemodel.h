/**
 * @file rubblemodel.h
 * @brief Déclaration du modèle de projectile "gravat" lancé par le ZombieLauncher.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef RUBBLEMODEL_H
#define RUBBLEMODEL_H

#include "entity.h"
#include "config.h"

class PlayerModel; // forward declaration

// ═══════════════════════════════════════════════════════════════════
//  RubbleModel — projectile tiré par le ZombieLauncherModel.
//  Se déplace en ligne droite à vitesse constante.
//  Se désactive dès qu'il touche un mur ou le joueur.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class RubbleModel
 * @brief Modèle d'un gravat (projectile) tiré par le zombie lanceur.
 */
class RubbleModel : public Entity {
public:
    /**
     * @brief Construit un gravat.
     * @param pos      Position de départ (centre du lanceur).
     * @param dir      Direction normalisée vers la cible au moment du tir.
     * @param damage   Dégâts infligés au joueur en cas de contact.
     * @param speed    Vitesse en pixels/s (défaut : Cfg::RUBBLE_SPEED).
     */
    RubbleModel(Vector2f pos,
                Vector2f dir,
                int      damage = Cfg::RUBBLE_DAMAGE,
                float    speed  = Cfg::RUBBLE_SPEED);

    /**
     * @brief Avance le gravat dans sa direction sur le delta-time fourni.
     * @param dt Delta-time en secondes.
     */
    void update(float dt) override;

    int      getDamage()    const { return damage;    }
    Vector2f getDirection() const { return direction; }

private:
    Vector2f direction; ///< Vecteur unitaire de déplacement.
    float    speed;     ///< Vitesse en pixels/s.
    int      damage;    ///< Dégâts infligés au contact du joueur.
};

#endif // RUBBLEMODEL_H
