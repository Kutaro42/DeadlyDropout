/**
 * @file bossmodel.h
 * @brief Déclaration du modèle du boss gérant son attaque, ses déplacement et autre atribut
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "enemy.h"
#include <QVector>
#include <QRectF>
#include "config.h"

/**
 * @class bossModel
 * @brief Modèle du boss s'occupant de la logique du boss (attaque,déplacement,ect)
 */
class bossModel : public Enemy
{
public:

    /**
     * @brief Constructeur du modèle
     * @param pos: Position initial du boss.
     */
    explicit bossModel(Vector2f pos, int levelIndex = 1);

    /**
     * @brief Rafraichissement du boss (interval d'attaque, état, déplaement).
     * @param dt: Distance entre le joueur et le boss.
     * @param playerPos: position du joueur.
     */
    void update(float dt, Vector2f playerPos) ;

    /**
     * @brief Attaque le joueur si le cooldown est a zéro.
     * @param pos: modèle du joueur pour infliger des dégats.
     */
    void attack(PlayerModel* player);

    // ─── Interface pattern 2 ─────────────────────────────────────
    bool isZoneCharging()    const { return zoneCharging;     }
    bool zoneJustExploded()  const { return zoneExplodedFlag; }
    void clearZoneExploded()       { zoneExplodedFlag = false;}
    const QVector<ZoneTile>& getZoneTiles() const { return zoneTiles; }

    /**
     * @brief Fournit les colliders des murs du niveau pour filtrer les tuiles de zone.
     *
     * Appelé par LevelController à chaque tick avant le déclenchement du pattern 2.
     * Permet à buildZoneTiles() d'éliminer les tuiles chevauchant un mur physique.
     *
     * @param walls Liste des rectangles de collision des murs du niveau.
     */
    void setWallColliders(const QVector<QRectF>& walls);

private:
    bool              hasPattern2;
    float             zoneChargeTimer;
    float             zoneExplosionTimer;
    bool              zoneCharging;
    bool              zoneExplodedFlag;
    QVector<ZoneTile> zoneTiles;
    QVector<QRectF>   wallColliders; ///< Copie des colliders de murs pour filtrage.


    /**
     * @brief Constructeur du modèle.
     * @param playerPos: position du joueur.
     * @param dt : distance entre le boss et le joueur.
     */
    void chase(Vector2f playerPos, float dt);

    /**
     * @brief Vérifie l'état dans lequel doit être le boss.
     * @param playerPos: Position du joueur.
     */
    void updateAIState(Vector2f playerPos);

    /**
     * @brief Génère les tuiles de la zone d'attaque.
     *
     * Crée une grille de tuiles 32×32 dans un rayon de BOSS_ZONE_RADIUS px autour
     * du centre du boss. Supprime les tuiles chevauchant un mur (wallColliders).
     * Tire aléatoirement BOSS_ZONE_SAFE_COUNT refuges parmi les tuiles restantes.
     */
    void buildZoneTiles();

};

#endif // BOSSMODEL_H
