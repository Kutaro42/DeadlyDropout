#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "entity.h"
#include "config.h"
#include <QRectF>

// ═══════════════════════════════════════════════════════════════════
//  PlayerModel — état et logique du joueur (couche Model, MVC).
//  Gère : déplacement, FacingDirection, attaque katana (SlashZone).
// ═══════════════════════════════════════════════════════════════════
class PlayerModel : public Entity {
private:
    int             hp;
    int             maxHp;
    float           speed;
    int             gold;
    bool            hasKey;
    bool            moving;         // flag pour l'animation de marche

    // ─── Katana ───────────────────────────────────────────────
    int             katanaDamage;
    float           katanaSlashW;       // largeur  SlashZone (px)
    float           katanaSlashD;       // profondeur SlashZone (px)
    float           katanaCooldownTimer;
    float           katanaAnimTimer;
    bool            katanaActive;       // animation en cours ?
    QRectF          slashZone;          // rectangle de collision actif

    FacingDirection facingDir;

public:
    explicit PlayerModel(Vector2f startPos);

    // ─── Mise à jour (timers katana) ─────────────────────────
    void update(float dt) override;

    // ─── Déplacement ─────────────────────────────────────────
    // inputDir : vecteur brut des touches pressées (sera normalisé)
    void move(Vector2f inputDir, float dt);
    void stopMoving() { moving = false; }

    // ─── Katana ───────────────────────────────────────────────
    // Retourne false si le cooldown n'est pas écoulé
    bool   launchKatana();
    bool   isKatanaActive()  const { return katanaActive; }
    QRectF getSlashZone()    const { return slashZone;    }

    // ─── Dégâts ───────────────────────────────────────────────
    void takeDamage(int amount);
    bool isDead() const { return hp <= 0; }

    // ─── Collecte ─────────────────────────────────────────────
    void addGold(int amount) { gold += amount; }
    void collectKey()        { hasKey = true;  }

    // ─── Getters ──────────────────────────────────────────────
    int             getHp()         const { return hp;          }
    int             getMaxHp()      const { return maxHp;       }
    int             getGold()       const { return gold;        }
    bool            getHasKey()     const { return hasKey;      }
    bool            isMoving()      const { return moving;      }
    int             getKatanaDmg()  const { return katanaDamage;}
    FacingDirection getFacing()     const { return facingDir;   }

    // ─── Setters d'upgrade (itérations futures) ───────────────
    void setMaxHp(int v)          { maxHp        = v; }
    void setSpeed(float v)        { speed        = v; }
    void setKatanaDamage(int v)   { katanaDamage = v; }
    void setKatanaSlashW(float v) { katanaSlashW = v; }
    void setKatanaSlashD(float v) { katanaSlashD = v; }

private:
    // Recalcule slashZone à partir de facingDir et position courante
    void updateSlashZone();
};

#endif // PLAYERMODEL_H
