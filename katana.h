/**
 * @file katana.h
 * @brief Déclaration de la classe Katana, l'arme corps-à-corps du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef KATANA_H
#define KATANA_H

#include <QRectF>
#include "config.h"
#include "vector2f.h"

/**
 * @class Katana
 * @brief Modèle de l'arme corps-à-corps du joueur.
 * * Gère les statistiques de l'arme (dégâts, portée), sa zone de collision 
 * (SlashZone), ainsi que les temporisateurs pour le cooldown et l'animation.
 * * @version 1.0
 */
class Katana {
public:
    /**
     * @brief Constructeur par défaut du Katana.
     */
    Katana();

    /**
     * @brief Met à jour les temporisateurs de l'arme (cooldown et animation).
     * @param dt Delta-time en secondes écoulé depuis le dernier tick.
     */
    void update(float dt);

    /**
     * @brief Tente de déclencher une attaque avec le katana.
     * * Si le cooldown est écoulé, calcule la zone d'impact (SlashZone) en fonction 
     * de la position et de la direction du joueur.
     * * @param playerPos Coordonnées actuelles du joueur.
     * @param playerW Largeur de la hitbox du joueur.
     * @param playerH Hauteur de la hitbox du joueur.
     * @param dir Direction dans laquelle le joueur regarde.
     * @return true si l'attaque a été lancée, false si l'arme est en cooldown.
     */
    bool tryAttack(Vector2f playerPos, float playerW, float playerH, FacingDirection dir);

    // ─── Getters ─────────────────────────────────────────────────────
    int    getDamage()      const { return itsDamage;   } ///< @return Les dégâts infligés par l'arme.
    int    getRange()       const { return itsRange;    } ///< @return La portée de l'arme en pixels.
    bool   isReady()        const { return itsReady;    } ///< @return true si l'arme peut attaquer.
    bool   isAnimActive()   const { return itsAnimTimer > 0.0f; } ///< @return true si l'animation est en cours.
    QRectF getSlashZone()   const { return itsSlashZone; } ///< @return Le rectangle de collision de l'attaque.

    // ─── Setters (upgrades itérations futures) ────────────────────────
    void setDamage(int d)       { itsDamage    = d; } ///< @param d Nouveaux dégâts de l'arme.
    void setRange(int r)        { itsRange     = r; } ///< @param r Nouvelle portée de l'arme.
    void setCooldown(float c)   { itsCooldown  = c; } ///< @param c Nouveau délai de récupération (en secondes).

private:
    int    itsDamage;       ///< Dégâts par coup (réf. Cfg::KATANA_DAMAGE).
    int    itsRange;        ///< Profondeur de la SlashZone en pixels (réf. Cfg::KATANA_SLASH_D).
    float  itsCooldown;     ///< Durée totale du cooldown en secondes (réf. Cfg::KATANA_COOLDOWN).
    bool   itsReady;        ///< Indicateur booléen stipulant si le cooldown est écoulé.

    float  itsCooldownTimer; ///< Temps restant avant la prochaine attaque possible.
    float  itsAnimTimer;     ///< Durée restante de l'animation d'attaque en cours.
    QRectF itsSlashZone;     ///< Rectangle de collision actif représentant la zone de dégâts.

    /**
     * @brief Calcule la géométrie de la SlashZone selon l'orientation.
     * @param playerPos Position du joueur.
     * @param playerW Largeur du joueur.
     * @param playerH Hauteur du joueur.
     * @param dir Direction de l'attaque.
     */
    void computeSlashZone(Vector2f playerPos, float playerW, float playerH, FacingDirection dir);
};

#endif // KATANA_H