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
 *
 * Gère les statistiques de l'arme (dégâts, portée), sa zone de collision
 * (SlashZone), ainsi que les temporisateurs pour le cooldown et l'animation.
 *
 * La SlashZone est maintenant calculée à partir d'un angle libre (radians)
 * fourni lors de tryAttack(), ce qui permet d'attaquer dans n'importe quelle
 * direction (pas seulement les 4 cardinaux).
 * L'AABB retournée par getSlashZone() est la bounding-box englobante du
 * rectangle orienté, garantissant la compatibilité avec les tests de collision
 * AABB existants dans LevelController.
 *
 * L'angle de la dernière attaque est exposé via getAttackAngle() pour que
 * PlayerView puisse orienter le sprite de l'effet visuel (slash).
 *
 * @version 2.0
 */
class Katana {
public:
    Katana();

    /**
     * @brief Met à jour les temporisateurs de l'arme (cooldown et animation).
     * @param dt Delta-time en secondes.
     */
    void update(float dt);

    /**
     * @brief Tente de déclencher une attaque dans la direction donnée par un angle.
     * @param playerPos  Centre du joueur (coin sup-gauche de la hitbox).
     * @param playerW    Largeur de la hitbox.
     * @param playerH    Hauteur de la hitbox.
     * @param angleRad   Angle en radians (atan2 standard : 0=droite, π/2=bas).
     * @return true si l'attaque a été lancée (cooldown prêt), false sinon.
     */
    bool tryAttack(Vector2f playerPos, float playerW, float playerH,
                   float angleRad);

    // ─── Getters ─────────────────────────────────────────────────────
    int    getDamage()      const { return itsDamage;   }
    int    getRange()       const { return itsRange;    }
    bool   isReady()        const { return itsReady;    }
    bool   isAnimActive()   const { return itsAnimTimer > 0.0f; }

    /**
     * @brief Retourne la bounding-box AABB englobant le rectangle de slash orienté.
     *        Utilisée par LevelController pour les tests de collision.
     */
    QRectF getSlashZone()   const { return itsSlashZone; }

    /**
     * @brief Retourne l'angle (radians) de la dernière attaque déclenchée.
     *        Utilisé par PlayerView pour orienter le sprite de l'effet de slash.
     */
    float  getAttackAngle() const { return itsAttackAngle; }

    // ─── Setters (upgrades) ───────────────────────────────────────────
    void setDamage(int d)     { itsDamage   = d; }
    void setRange(int r)      { itsRange    = r; }
    void setCooldown(float c) { itsCooldown = c; }

private:
    int    itsDamage;
    int    itsRange;
    float  itsCooldown;
    bool   itsReady;

    float  itsCooldownTimer;
    float  itsAnimTimer;

    /// AABB englobante du rectangle de slash orienté (compatibilité LevelController).
    QRectF itsSlashZone;

    /// Angle (radians) de la dernière attaque. Exposé pour le sprite de slash dans PlayerView.
    float  itsAttackAngle;

    /**
     * @brief Calcule la SlashZone AABB pour un angle libre.
     *
     * Le rectangle de frappe "logique" est un rectangle de dimensions
     * (KATANA_SLASH_W × KATANA_SLASH_D) centré sur le côté du joueur
     * faisant face à l'angle. Les 4 coins sont ensuite projetés via rotation
     * pour obtenir l'AABB englobante exacte.
     *
     * @param playerPos Centre de la hitbox joueur (coin sup-gauche).
     * @param playerW   Largeur hitbox.
     * @param playerH   Hauteur hitbox.
     * @param angleRad  Angle d'attaque en radians.
     */
    void computeSlashZone(Vector2f playerPos, float playerW, float playerH,
                          float angleRad);
};

#endif // KATANA_H
