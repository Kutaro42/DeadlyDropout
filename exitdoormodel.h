#ifndef EXITDOORMODEL_H
#define EXITDOORMODEL_H

#include "entity.h"
#include "config.h"

class PlayerModel; // forward declaration

// ═══════════════════════════════════════════════════════════════════
//  ExitDoorModel — zone de sortie du niveau.
//  Fermée par défaut ; s'ouvre quand tous les ennemis sont vaincus.
//  Le joueur doit se trouver sur cette zone ET la porte doit être
//  ouverte pour déclencher la victoire.
// ═══════════════════════════════════════════════════════════════════
class ExitDoorModel : public Entity {
private:
    bool open;

public:
    ExitDoorModel(Vector2f pos, float w, float h);
    void update(float dt) override;

    void openDoor()      { open = true;  }
    bool isOpen()  const { return open;  }

    // Retourne true si le joueur peut passer (porte ouverte)
    bool tryEnter(const PlayerModel& player) const;
};

#endif // EXITDOORMODEL_H
