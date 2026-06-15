#ifndef WALLMODEL_H
#define WALLMODEL_H

#include "entity.h"

/**
 * @class WallModel
 * @brief Entité statique bloquant le passage, associée à un index de tile du tileset.
 */
class WallModel : public Entity {
public:
    /**
     * @brief Construit un mur.
     * @param pos       Position haut-gauche dans la scène.
     * @param w         Largeur du bloc (pixels).
     * @param h         Hauteur du bloc (pixels).
     * @param tileIndex Index 1-based de la tile dans le tileset (1 = 1ère tile).
     * @param tilesetCols Nombre de colonnes du tileset PNG.
     */
    WallModel(Vector2f pos, float w, float h,
              int tileIndex = 1, int tilesetCols = 302);

    void update(float dt) override;

    int getTileIndex()    const { return tileIndex;    }
    int gettilesetCols()  const { return tilesetCols;  }

private:
    int tileIndex;    ///< Index 1-based dans le tileset.
    int tilesetCols;  ///< Nombre de colonnes du tileset.
};

#endif // WALLMODEL_H
