/**
 * @file exitdoormodel.h
 * @brief Déclaration du modèle de la porte de fin de niveau.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef EXITDOORMODEL_H
#define EXITDOORMODEL_H

#include "entity.h"
#include "config.h"
#include "globaldoor.h"

class PlayerModel; // forward declaration

// ═══════════════════════════════════════════════════════════════════
//  ExitDoorModel — zone de sortie du niveau.
//  Fermée par défaut.
//  Le joueur doit se trouver sur cette zone ET la porte doit être
//  ouverte pour déclencher la victoire.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ExitDoorModel
 * @brief Représente le portail permettant de passer au niveau suivant.
 */
class ExitDoorModel : public GlobalDoor {
private:
    bool open; ///< Indique si la porte est déverrouillée.

public:
    /**
     * @brief Constructeur de la zone de sortie.
     */
    ExitDoorModel(Vector2f pos, float w, float h);
    void update(float dt) override;

    void openDoor()      { open = true;  }
    bool isOpen()  const { return open;  }

};

#endif // EXITDOORMODEL_H
