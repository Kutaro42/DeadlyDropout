/**
 * @file wallmodel.h
 * @brief Déclaration de la classe WallModel définissant les collisions environnementales (murs).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef WALLMODEL_H
#define WALLMODEL_H

#include "entity.h"

// ═══════════════════════════════════════════════════════════════════
//  WallModel — entité statique servant de mur de salle.
//  Son collider (QRectF) est utilisé par LevelController pour la
//  résolution des collisions AABB.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class WallModel
 * @brief Entité inerte (statique) bloquant le passage des unités mobiles.
 * * Les dimensions de ces murs délimitent l'arène de jeu.
 */
class WallModel : public Entity {
public:
    /**
     * @brief Construit un nouveau modèle de mur.
     * @param pos Emplacement (haut-gauche) du mur dans la salle.
     * @param w Largeur du bloc.
     * @param h Hauteur du bloc.
     */
    WallModel(Vector2f pos, float w, float h);
    
    /**
     * @brief Routine de mise à jour. Totalement vide car l'objet est immobile.
     * @param dt Delta-time (ignoré).
     */
    void update(float dt) override;
};

#endif // WALLMODEL_H