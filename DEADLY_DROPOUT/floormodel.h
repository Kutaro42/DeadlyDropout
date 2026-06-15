/**
 * @file floormodel.h
 * @brief Entité statique décorative de sol, associée à un index de tile du tileset de sol.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef FLOORMODEL_H
#define FLOORMODEL_H

#include "entity.h"

/**
 * @class FloorModel
 * @brief Entité statique décorative représentant une tile de sol.
 *
 * Contrairement à WallModel, FloorModel n'a aucun rôle dans la résolution
 * des collisions : il est uniquement utilisé par FloorView pour l'affichage.
 * Son collider est présent (hérité d'Entity) mais n'est jamais consulté
 * par le LevelController.
 */
class FloorModel : public Entity {
public:
    /**
     * @brief Construit une tile de sol.
     * @param pos         Position haut-gauche dans la scène.
     * @param w           Largeur de la tile (pixels).
     * @param h           Hauteur de la tile (pixels).
     * @param tileIndex   Index 1-based de la tile dans le tileset (1 = 1ère tile).
     * @param tilesetCols Nombre de colonnes du tileset PNG.
     */
    FloorModel(Vector2f pos, float w, float h,
               int tileIndex = 1, int tilesetCols = 8);

    /** @brief Aucune logique de mise à jour nécessaire pour le sol. */
    void update(float dt) override;

    int getTileIndex()   const { return tileIndex;   } ///< @return Index 1-based dans le tileset.
    int getTilesetCols() const { return tilesetCols;  } ///< @return Nombre de colonnes du tileset.

private:
    int tileIndex;    ///< Index 1-based dans le tileset de sol.
    int tilesetCols;  ///< Nombre de colonnes du tileset de sol.
};

#endif // FLOORMODEL_H
