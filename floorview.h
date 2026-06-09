/**
 * @file floorview.h
 * @brief Affiche une tile de sol en découpant le bon sprite depuis le tileset de sol.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef FLOORVIEW_H
#define FLOORVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "floormodel.h"
#include "config.h"

/**
 * @class FloorView
 * @brief Item graphique représentant une tile de sol.
 *
 * Fonctionne exactement comme WallView : le tileset est chargé une seule fois
 * via un cache statique, puis la tile est découpée selon l'index stocké dans
 * le FloorModel associé.
 *
 * La tile est placée sur la couche Z_FLOOR, sous les murs et les entités.
 *
 * Formule de découpe (identique à WallView) :
 *   col = (tileIndex - 1) % tilesetCols
 *   row = (tileIndex - 1) / tilesetCols
 */
class FloorView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Construit la vue à partir d'un FloorModel.
     * @param model  Pointeur vers le modèle de la tile de sol.
     * @param parent Item graphique parent (optionnel).
     */
    explicit FloorView(FloorModel* model, QGraphicsItem* parent = nullptr);

private:
    /// Retourne le tileset de sol (chargé une seule fois).
    static const QPixmap& tileset();

    /// Découpe et retourne la tile correspondant à l'index.
    static QPixmap extractTile(int tileIndex, int tilesetCols,
                               int tileW, int tileH);
};

#endif // FLOORVIEW_H
