#ifndef LEVELTEMPLATE_H
#define LEVELTEMPLATE_H

#include "config.h"
#include <QString>

// ═══════════════════════════════════════════════════════════════════
//  SYSTÈME DE TEMPLATE DE NIVEAU
//
//  LevelMatrix<ROWS, COLS>
//    Template C++ représentant une grille de level sous forme de
//    tableau 2D d'entiers.
//
//  LevelDescriptor
//    Type "type-erasé" dérivé d'un LevelMatrix.
//    Utilisé par LevelBuilder sans connaitre ROWS/COLS à la compilation.
//
//  makeDescriptor<ROWS, COLS>(id, name, matrix)
//    Fonction template helper pour créer un LevelDescriptor.
//
//  LevelData::LEVEL_1, LEVEL_2, …
//    Données des niveaux. Ajouter un niveau = ajouter une
//    LevelMatrix + son LevelDescriptor dans ce namespace.
//
//  CLÉS DE LA MATRICE :
//    0 = FLOOR        (sol libre)
//    1 = WALL         (mur)
//    2 = ZOMBIE_SPAWN (point de spawn zombie)
//    3 = EXIT_DOOR    (zone de sortie)
//    4 = PLAYER_SPAWN (point de spawn joueur)
// ═══════════════════════════════════════════════════════════════════

// ─── Type générique d'une grille ─────────────────────────────────
template<int ROWS, int COLS>
struct LevelMatrix {
    static constexpr int rows = ROWS;
    static constexpr int cols = COLS;
    int data[ROWS][COLS];

    int at(int r, int c) const { return data[r][c]; }
};

// ─── Descripteur "type-erased" pour LevelBuilder ─────────────────
struct LevelDescriptor {
    int         id;
    QString     name;
    int         rows;
    int         cols;
    const int*  flatData;     // pointeur vers data[0][0]

    int at(int r, int c) const { return flatData[r * cols + c]; }
};

// ─── Helper : crée un LevelDescriptor depuis n'importe quel LevelMatrix
template<int ROWS, int COLS>
inline LevelDescriptor makeDescriptor(int id, const QString& name,
                                       const LevelMatrix<ROWS, COLS>& m)
{
    return { id, name, ROWS, COLS, &m.data[0][0] };
}

// ═══════════════════════════════════════════════════════════════════
//  DONNÉES DE NIVEAUX
// ═══════════════════════════════════════════════════════════════════
namespace LevelData {

    // ─── Niveau 1 — Pattern 1 (15 cols × 12 rows = 480 × 384 px) ─
    inline constexpr LevelMatrix<12, 15> LEVEL_1 = {{
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1 },  // spawn joueur
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1 },  // sortie
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    }};

    // ─── Niveau 2 — Pattern 2 (22 cols × 18 rows = 704 × 576 px) ─
    inline constexpr LevelMatrix<18, 22> LEVEL_2 = {{
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1 },  // spawn joueur
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1 },  // sortie
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
    }};

    // ─── Descripteurs (accès par LevelBuilder) ────────────────────
    inline const LevelDescriptor LEVEL_1_DESC = makeDescriptor(1, "Niveau 1", LEVEL_1);
    inline const LevelDescriptor LEVEL_2_DESC = makeDescriptor(2, "Niveau 2", LEVEL_2);

    // ─── Tableau de tous les niveaux (accès par index) ────────────
    inline const LevelDescriptor* ALL_LEVELS[] = {
        &LEVEL_1_DESC,
        &LEVEL_2_DESC
    };
    inline constexpr int LEVEL_COUNT = 2;

} // namespace LevelData

#endif // LEVELTEMPLATE_H
