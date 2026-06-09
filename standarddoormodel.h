/**
 * @file standarddoormodel.h
 * @brief Déclaration du modèle de la porte standard (ouvrable au contact du joueur).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef STANDARDDOORMODEL_H
#define STANDARDDOORMODEL_H

#include "globaldoor.h"
#include "config.h"

class PlayerModel; // forward declaration

// ═══════════════════════════════════════════════════════════════════
//  StandardDoorModel — porte de jonction entre salles/couloirs.
//  Fermée par défaut.
//  Elle s'ouvre dès que le joueur entre en contact avec elle et
//  reste ouverte jusqu'à la fin du niveau.
//  Une fois ouverte, elle n'exerce plus de collision (le joueur
//  peut passer librement).
// ═══════════════════════════════════════════════════════════════════

/**
 * @class StandardDoorModel
 * @brief Porte standard ouvrable par simple contact du joueur.
 */
class StandardDoorModel : public GlobalDoor {
private:
    bool open; ///< Indique si la porte est ouverte (et donc traversable).

public:
    /**
     * @brief Constructeur.
     * @param pos Position haut-gauche dans la scène.
     * @param w   Largeur en pixels.
     * @param h   Hauteur en pixels.
     */
    StandardDoorModel(Vector2f pos, float w, float h);

    void update(float dt) override;

    /**
     * @brief Ouvre la porte (irréversible durant le niveau).
     */
    void openDoor() { open = true; }

    /**
     * @brief Indique si la porte est actuellement ouverte.
     * @return true si ouverte (traversable), false sinon.
     */
    bool isOpen() const { return open; }

    /**
     * @brief Ouvre la porte si le joueur la touche.
     *        Doit être appelé par LevelController à chaque tick.
     * @param player Modèle du joueur.
     * @return true si la porte vient d'être ouverte, false sinon.
     */
    bool tryOpen(const PlayerModel& player);
};

#endif // STANDARDDOORMODEL_H
