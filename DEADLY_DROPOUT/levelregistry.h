/**
 * @file levelregistry.h
 * @brief Registre centralisé des niveaux du jeu.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 *
 * LevelRegistry remplace l'ancien namespace LevelData de leveltemplate.h.
 * Il charge tous les niveaux depuis leurs fichiers texte via MatrixExtractor,
 * et expose la liste nécessaire au GameController.
 *
 * Pour ajouter un niveau :
 *   1. Créer le dossier  levels/level_N/
 *   2. Y placer entities.txt, walls.txt, floor.txt
 *   3. Incrémenter LEVEL_COUNT et appeler loadLevel(N) dans LevelRegistry::init().
 *   4. Déclarer le fichier dans ressources.qrc (section /levels).
 */

#ifndef LEVELREGISTRY_H
#define LEVELREGISTRY_H

#include "matrixextractor.h"
#include <QVector>

/**
 * @class LevelRegistry
 * @brief Charge et stocke tous les DynamicLevelDescriptor du jeu.
 *
 * Usage typique (dans GameController::init ou AppController) :
 * @code
 *   LevelRegistry& reg = LevelRegistry::instance();
 *   reg.init();                          // charge depuis :/levels
 *   int count = reg.count();
 *   const LevelDescriptor* desc = reg.descriptorAt(0);
 * @endcode
 */
class LevelRegistry {
public:
    // ── Nombre total de niveaux déclarés ──────────────────────────
    static constexpr int LEVEL_COUNT = 3;

    /**
     * @brief Singleton : retourne l'instance unique du registre.
     */
    static LevelRegistry& instance();

    /**
     * @brief Charge tous les niveaux.
     *
     * @param basePath Chemin racine des dossiers de niveaux.
     *                 Par défaut ":/levels" (ressources Qt embarquées).
     *                 Passer un chemin système pour les tests hors-ressources.
     */
    void init(const QString& basePath = ":/levels");

    /**
     * @brief Nombre de niveaux chargés avec succès.
     */
    int count() const;

    /**
     * @brief Retourne un pointeur vers le LevelDescriptor du niveau à l'index donné.
     *
     * @param index Index dans la liste (0-based).
     * @return Pointeur valide tant que ce LevelRegistry est en vie, nullptr si hors bornes.
     */
    const LevelDescriptor* descriptorAt(int index) const;

private:
    LevelRegistry() = default;

    /// Buffers propriétaires (DynamicLevelDescriptor gère la mémoire).
    QVector<DynamicLevelDescriptor> m_dynamics;

    /// Vues légères pointant vers les buffers ci-dessus.
    /// Reconstruites après chaque init() car les pointeurs raw peuvent changer
    /// si QVector réalloue lors d'un append.
    QVector<LevelDescriptor> m_descriptors;
};

#endif // LEVELREGISTRY_H
