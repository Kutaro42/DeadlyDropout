/**
 * @file bossmodel.h
 * @brief Déclaration du modèle du boss gérant son attaque, ses déplacement et autre atribut
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef BOSSMODEL_H
#define BOSSMODEL_H

#include "enemy.h"

/**
 * @class bossModel
 * @brief A boss model that handles the boss's behavior (attacks, movement, etc.)
 */
class bossModel : public Enemy
{
public:

    /**
     * @brief Model builder
     * @param pos: The boss's starting position.
     */
    explicit bossModel(Vector2f pos, int levelIndex = 1);
    ~bossModel() override;

    /**
     * @brief Death hook trigger: plays zombiedeath.wav.
     */
    void onDeath() override;

    /**
     * @brief Boss update (attack interval, status, movement).
     * @param dt: Distance between the player and the boss.
     * @param playerPos: player's position.
     */
    void update(float dt, Vector2f playerPos) ;

    /**
     * @brief Attack the player if the cooldown is at zero.
     * @param pos: player model for dealing damage.
     */
    void attack(PlayerModel* player);

    // ─── Interface Pattern 2 ─────────────────────────────────────
    bool isZoneCharging()    const { return zoneCharging;     }
    bool zoneJustExploded()  const { return zoneExplodedFlag; }
    void clearZoneExploded()       { zoneExplodedFlag = false;}
    const QVector<ZoneTile>& getZoneTiles() const { return zoneTiles; }

    /**
     * @brief Provides the level wall colliders to filter the area tiles.
     *
     * Called by LevelController on every tick before pattern 2 is triggered.
     * Allows buildZoneTiles() to remove tiles that overlap a physical wall.
     *
     * @param walls : List of collision boxes for the walls in the level.
     */
    void setWallColliders(const QVector<QRectF>& walls);

private:
    QSoundEffect*     deathSound;                ///< Sound played when the boss is defeated.
    bool              hasPattern2;               ///< Show if the boss can use the area attack
    float             zoneChargeTimer;           ///< Timer during the attack loading
    float             zoneExplosionTimer;        ///< Timer between the pattern show and the explosion
    bool              zoneCharging;              ///< Show if the attack area is load
    bool              zoneExplodedFlag;          ///< Show if the attack is done
    QVector<ZoneTile> zoneTiles;                 ///< QVector that stores the attack area
    QVector<QRectF>   wallColliders;             ///< Copy wall colliders for filtering.


    /**
     * @brief Model builder.
     * @param playerPos: Player position
     * @param dt : distance the player and the boss.
     */
    void chase(Vector2f playerPos, float dt);

    /**
     * @brief Check the condition the boss should be in.
     * @param playerPos: Player Position.
     */
    void updateAIState(Vector2f playerPos);

    /**
     * @brief Generates the tiles for the attack area.
     * Creates a 32×32 grid of tiles within a radius of BOSS_ZONE_RADIUS px around
     * the center of the boss. Removes tiles that overlap a wall (wallColliders).
     * Randomly selects BOSS_ZONE_SAFE_COUNT safe zones from the remaining tiles.
     */
    void buildZoneTiles();

};

#endif // BOSSMODEL_H
