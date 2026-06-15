/**
 * @file matrixextractor.h
 * @brief Déclaration de MatrixExtractor : lecture des matrices de niveau depuis des fichiers texte.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef MATRIXEXTRACTOR_H
#define MATRIXEXTRACTOR_H

#include <QString>
#include <QVector>
#include "leveltemplate.h"

// ═══════════════════════════════════════════════════════════════════
//  MatrixExtractor
//
//  Charge les trois matrices d'un niveau depuis un dossier structuré :
//
//    levels/
//      level_1/
//        entities.txt
//        walls.txt
//        floor.txt
//      level_2/
//        ...
//
//  Format d'un fichier texte :
//    Ligne 1 : "<rows> <cols>"
//    Lignes suivantes : valeurs entières séparées par des espaces,
//                       une ligne par rangée de la matrice.
//
//  La classe produit un DynamicLevelDescriptor (voir ci-dessous) qui
//  possède ses propres buffers et peut donc être utilisé exactement
//  comme un LevelDescriptor classique via son accesseur asDescriptor().
//
//  Exemple d'utilisation :
//    auto dld = MatrixExtractor::load(1, ":/levels"); // ressources Qt
//    if (dld) LevelBuilder::build(dld->asDescriptor(), levelModel, scene);
// ═══════════════════════════════════════════════════════════════════

/**
 * @struct DynamicLevelDescriptor
 * @brief Wrapper dynamique d'un LevelDescriptor dont les buffers sont
 *        alloués sur le tas (QVector<int>) et non dans des tableaux
 *        statiques en mémoire constante.
 *
 * Utilisé à la place des LevelMatrix<R,C> constexpr lorsque les données
 * proviennent de fichiers externes plutôt que du binaire compilé.
 */
struct DynamicLevelDescriptor {
    QVector<int> entityBuf;  ///< Buffer plat de la matrice entités.
    QVector<int> wallBuf;    ///< Buffer plat de la matrice murs.
    QVector<int> floorBuf;   ///< Buffer plat de la matrice sol.

    int     id;              ///< Identifiant numérique du niveau.
    QString name;            ///< Nom lisible du niveau.
    int     rows;            ///< Nombre de lignes des matrices.
    int     cols;            ///< Nombre de colonnes des matrices.
    int     tilesetCols;     ///< Colonnes du tileset murs (pour WallView).
    int     floorTilesetCols;///< Colonnes du tileset sol  (pour FloorView).

    /**
     * @brief Construit et retourne un LevelDescriptor pointant vers
     *        les buffers internes de cette instance.
     *
     * @warning Le DynamicLevelDescriptor doit rester en vie aussi
     *          longtemps que le LevelDescriptor retourné est utilisé
     *          (pas de copie profonde des pointeurs).
     * @return Un LevelDescriptor prêt à être passé à LevelBuilder::build().
     */
    LevelDescriptor asDescriptor() const;
};

/**
 * @class MatrixExtractor
 * @brief Classe utilitaire (statique) chargeant les matrices d'un niveau
 *        depuis des fichiers texte organisés en dossiers.
 */
class MatrixExtractor {
public:
    /**
     * @brief Charge les trois matrices d'un niveau depuis un dossier.
     *
     * @param levelId         Identifiant du niveau (ex. 1 → dossier "level_1").
     * @param basePath        Chemin racine contenant les sous-dossiers de niveaux.
     *                        Peut être un chemin système ("levels") ou une ressource
     *                        Qt (":/levels").
     * @param tilesetCols     Colonnes du tileset murs  (défaut : 50).
     * @param floorTilesetCols Colonnes du tileset sol  (défaut : 50).
     * @return Un DynamicLevelDescriptor valide, ou un objet vide (rows==0)
     *         si le chargement a échoué.
     */
    static DynamicLevelDescriptor load(int            levelId,
                                       const QString& basePath        = ":/levels",
                                       int            tilesetCols      = 302,
                                       int            floorTilesetCols = 302);

private:
    /**
     * @brief Lit un fichier matrice texte et remplit un QVector<int>.
     *
     * @param filePath Chemin complet du fichier à lire.
     * @param rows     [out] Nombre de lignes lu dans l'en-tête.
     * @param cols     [out] Nombre de colonnes lu dans l'en-tête.
     * @return Le buffer plat des valeurs, vide en cas d'erreur.
     */
    static QVector<int> readMatrixFile(const QString& filePath,
                                       int&           rows,
                                       int&           cols);
};

#endif // MATRIXEXTRACTOR_H
