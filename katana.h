#ifndef KATANA_H
#define KATANA_H

#include <QRectF>
#include "config.h"
#include "vector2f.h"

// ═══════════════════════════════════════════════════════════════════
//  Katana — modèle de l'arme corps-à-corps du joueur.
//  Gère : dégâts, portée (SlashZone), cooldown, timer d'animation.
//  PlayerModel possède un Katana et délègue launchKatana() ici.
// ═══════════════════════════════════════════════════════════════════
class Katana {
public:
    Katana();

    // ─── Mise à jour des timers (appelé depuis PlayerModel::update) ──
    void update(float dt);

    // ─── Tentative d'attaque ─────────────────────────────────────────
    // Retourne false si le cooldown n'est pas écoulé.
    // Recalcule la SlashZone à partir de la position et direction fournies.
    bool tryAttack(Vector2f playerPos, float playerW, float playerH,
                   FacingDirection dir);

    // ─── Getters ─────────────────────────────────────────────────────
    int    getDamage()      const { return itsDamage;   }
    int    getRange()       const { return itsRange;    }
    bool   isReady()        const { return itsReady;    }
    bool   isAnimActive()   const { return itsAnimTimer > 0.0f; }
    QRectF getSlashZone()   const { return itsSlashZone; }

    // ─── Setters (upgrades itérations futures) ────────────────────────
    void setDamage(int d)       { itsDamage    = d; }
    void setRange(int r)        { itsRange     = r; }
    void setCooldown(float c)   { itsCooldown  = c; }

private:
    int    itsDamage;       // dégâts par coup (Cfg::KATANA_DAMAGE)
    int    itsRange;        // profondeur SlashZone en px (Cfg::KATANA_SLASH_D)
    float  itsCooldown;     // durée cooldown (Cfg::KATANA_COOLDOWN)
    bool   itsReady;        // cooldown écoulé ?

    float  itsCooldownTimer;
    float  itsAnimTimer;    // durée restante de l'animation
    QRectF itsSlashZone;    // rectangle de collision actif

    void computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                          FacingDirection dir);
};

#endif // KATANA_H
