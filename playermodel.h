#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entity.h"
#include "katana.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerModel — état et logique du joueur (couche Model, MVC).
//  Délègue tout ce qui concerne l'arme à l'objet Katana.
// ═══════════════════════════════════════════════════════════════════
class PlayerModel : public Entity {
private:
    int             hp;
    int             maxHp;
    float           speed;
    int             gold;
    bool            hasKey;
    bool            moving;

    FacingDirection facingDir;

    // ─── Arme ─────────────────────────────────────────────────
    Katana          katana;     // PlayerModel possède son katana

public:
    explicit PlayerModel(Vector2f startPos);

    // ─── Mise à jour (timers katana) ──────────────────────────
    void update(float dt) override;

    // ─── Déplacement ──────────────────────────────────────────
    void move(Vector2f inputDir, float dt);
    void stopMoving() { moving = false; }

    // ─── Attaque katana (reste dans PlayerModel, délègue à Katana) ──
    bool launchKatana();

    // ─── Dégâts ───────────────────────────────────────────────
    void takeDamage(int amount);
    bool isDead() const { return hp <= 0; }

    // ─── Collecte ─────────────────────────────────────────────
    void addGold(int amount) { gold += amount; }
    void collectKey()        { hasKey = true;  }

    // ─── Getters ──────────────────────────────────────────────
    int             getHp()           const { return hp;       }
    int             getMaxHp()        const { return maxHp;    }
    int             getGold()         const { return gold;     }
    bool            getHasKey()       const { return hasKey;   }
    bool            isMoving()        const { return moving;   }
    FacingDirection getFacing()       const { return facingDir;}

    // Accès katana (LevelController pour la SlashZone, PlayerView pour l'anim)
    const Katana&   getKatana()       const { return katana;   }
    bool            isKatanaActive()  const { return katana.isAnimActive(); }
    QRectF          getSlashZone()    const { return katana.getSlashZone(); }

    // ─── Setters upgrade ──────────────────────────────────────
    void setMaxHp(int v)        { maxHp = v; }
    void setSpeed(float v)      { speed = v; }
    // upgrades katana délégués
    void setKatanaDamage(int v)   { katana.setDamage(v);   }
    void setKatanaRange(int v)    { katana.setRange(v);    }
    void setKatanaCooldown(float v){ katana.setCooldown(v);}
};

#endif // PLAYERMODEL_H
