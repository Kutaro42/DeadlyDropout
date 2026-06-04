/**
 * @file gamemodel.h
 * @brief Déclaration du modèle central stockant les entités de la partie.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "playermodel.h"
#include "levelmodel.h"

// ═══════════════════════════════════════════════════════════════════
//  GameModel — état global de la partie (couche Model, MVC).
//  Possède le PlayerModel et le LevelModel courant.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class GameModel
 * @brief Modèle englobant les données persistantes d'une partie.
 */
class GameModel : public QObject{
    Q_OBJECT
private:
    PlayerModel* player;            ///< Modèle du joueur.
    LevelModel* level;             ///< Données structurelles et entités du niveau actuel.
    int          currentLevelIndex; ///< Numéro du niveau en cours d'exécution.

public:
    /**
     * @brief Constructeur du modèle de jeu.
     */
    GameModel();
    ~GameModel();

    /**
     * @brief Initialise ou réinitialise le joueur à une position donnée.
     * @param spawnPos Position vectorielle de départ.
     */
    void initPlayer(Vector2f spawnPos);
    void obtainKey();

    void setCurrentLevelIndex(int i) { currentLevelIndex = i; }

    PlayerModel* getPlayer() const { return player; }
    LevelModel* getLevel()  const { return level;  }
    int          getLevelIndex()   const { return currentLevelIndex; }
};

#endif // GAMEMODEL_H
