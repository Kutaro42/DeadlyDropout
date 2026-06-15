/**
 * @file leveltemplate.h
 * @brief Structures de description de niveaux (LevelMatrix, LevelDescriptor, makeDescriptor).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 *
 * @note Ce fichier ne contient PLUS les données de niveaux (matrices inline).
 *       Les matrices sont désormais stockées dans des fichiers texte :
 *
 *         levels/
 *           level_1/  entities.txt  walls.txt  floor.txt
 *           level_2/  entities.txt  walls.txt  floor.txt
 *           ...
 *
 *       Pour charger un niveau, utilisez MatrixExtractor::load() (voir matrixextractor.h),
 *       puis passez le DynamicLevelDescriptor::asDescriptor() à LevelBuilder::build().
 *
 *       Pour enregistrer la liste complète des niveaux, voir levelregistry.h.
 */

#ifndef LEVELTEMPLATE_H
#define LEVELTEMPLATE_H

#include "config.h"
#include <QString>

// ═══════════════════════════════════════════════════════════════════
//  SYSTÈME DE TEMPLATE DE NIVEAU — VERSION 3 (matrices externalisées)
//
//  Chaque niveau est défini par TROIS matrices de même dimension :
//
//  entityMatrix  (spawns et objets logiques) :
//    0 = FLOOR        (sol libre)
//    2 = ZOMBIE_SPAWN
//    3 = EXIT_DOOR
//    4 = PLAYER_SPAWN
//    5 = ZOMBIE_LAUNCHER_SPAWN
//    6 = STANDARD_DOOR
//    7 = BOSS_SPAWN
//    8 = BOSS_DOOR
//
//  wallMatrix :
//    0 = pas de mur
//    N = index de tile dans le tileset de murs (1-based)
//
//  floorMatrix :
//    0 = sol uniforme (couleur de fond de scène, rien à dessiner)
//    N = index de tile dans le tileset de sol (1-based)
// ═══════════════════════════════════════════════════════════════════

// ─── LevelMatrix (rétrocompatibilité — non utilisé en runtime) ────
// Conservé pour que le template makeDescriptor reste compilable si
// quelqu'un souhaite encore définir un niveau directement en C++.
template<int ROWS, int COLS>
struct LevelMatrix {
    static constexpr int rows = ROWS;
    static constexpr int cols = COLS;
    int data[ROWS][COLS];
    int at(int r, int c) const { return data[r][c]; }
};

// ─── Descripteur "type-erased" ────────────────────────────────────
/**
 * @struct LevelDescriptor
 * @brief Vue non-propriétaire sur les trois matrices d'un niveau.
 *
 * Les pointeurs entityData, wallData et floorData NE POSSÈDENT PAS
 * les données : leur durée de vie est gérée par l'appelant
 * (buffer statique constexpr ou DynamicLevelDescriptor).
 */
struct LevelDescriptor {
    int         id;
    QString     name;
    int         rows;
    int         cols;

    const int*  entityData;       ///< Matrice entités (spawns, sortie…)
    const int*  wallData;         ///< Matrice murs  (0 = vide, N = index tileset murs)
    int         tilesetCols;      ///< Nombre de colonnes du tileset de murs PNG

    const int*  floorData;        ///< Matrice sol   (0 = fond uni, N = index tileset sol)
    int         floorTilesetCols; ///< Nombre de colonnes du tileset de sol PNG

    int entityAt(int r, int c) const { return entityData[r * cols + c]; }
    int wallAt  (int r, int c) const { return wallData  [r * cols + c]; }
    int floorAt (int r, int c) const { return floorData [r * cols + c]; }
};

// ─── Helpers makeDescriptor (rétrocompatibilité) ──────────────────

/// Surcharge complète : entités + murs + sol (matrices statiques C++).
template<int ROWS, int COLS>
inline LevelDescriptor makeDescriptor(int id, const QString& name,
                                      const LevelMatrix<ROWS, COLS>& entities,
                                      const LevelMatrix<ROWS, COLS>& walls,
                                      const LevelMatrix<ROWS, COLS>& floor,
                                      int tilesetCols      = 302,
                                      int floorTilesetCols = 302)
{
    return { id, name, ROWS, COLS,
             &entities.data[0][0],
             &walls.data[0][0],
             tilesetCols,
             &floor.data[0][0],
             floorTilesetCols };
}

/// Surcharge sans matrice sol (rétrocompatibilité).
template<int ROWS, int COLS>
inline LevelDescriptor makeDescriptor(int id, const QString& name,
                                      const LevelMatrix<ROWS, COLS>& entities,
                                      const LevelMatrix<ROWS, COLS>& walls,
                                      int tilesetCols = 302)
{
    return { id, name, ROWS, COLS,
             &entities.data[0][0],
             &walls.data[0][0],
             tilesetCols,
             nullptr,
             0 };
}

#endif // LEVELTEMPLATE_H
