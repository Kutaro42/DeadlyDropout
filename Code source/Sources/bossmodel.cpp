/**
 * @file bossmodel.cpp
 * @brief Implementation of the boss gate model.
 */

#include "Headers/bossmodel.h"
#include "Headers/playermodel.h"
#include <QRandomGenerator>
#include <QSet>

bossModel::bossModel(Vector2f pos, int levelIndex)
    : Enemy(pos,
            (levelIndex <= 1) ? Cfg::BOSSP1_MAX_HP :
                (levelIndex == 2) ? Cfg::BOSSP2_MAX_HP : Cfg::BOSSP3_MAX_HP,
            Cfg::BOSS_SPEED,
            Cfg::BOSS_DMG,
            Cfg::BOSS_DETECT_R,
            static_cast<float>(Cfg::ENTITY_SIZE * 3),
            static_cast<float>(Cfg::ENTITY_SIZE * 3),
            Cfg::BOSS_GOLD_MIN)
    , hasPattern2       (levelIndex >= 2)
    , zoneChargeTimer   ((levelIndex > 2) ? Cfg::BOSS_ZONE_CHARGE_CD2 : Cfg::BOSS_ZONE_CHARGE_CD3)
    , zoneExplosionTimer(0.0f)
    , zoneCharging      (false)
    , zoneExplodedFlag  (false)
    , deathSound        (nullptr)
{
    // A boolean indicating that this enemy is the boss in the enemy array.
    isBoss = true;

    deathSound = new QSoundEffect();
    deathSound->setSource(QUrl("qrc:/images/Sound/zombiedeath.wav"));
    deathSound->setVolume(0.6f);
}

bossModel::~bossModel()
{
    if (deathSound)
        deathSound->deleteLater();
}

void bossModel::onDeath()
{
    if (deathSound)
        deathSound->play();
}

// ─── Providing wall colliders (from LevelController) ────
void bossModel::setWallColliders(const QVector<QRectF>& walls)
{
    wallColliders = walls;
}

void bossModel::attack(PlayerModel* player)
{
    // Check that the cooldown has elapsed before attacking.
    if (attackTimer > 0.0f) return;

    // deals damage to the player and triggers the cooldown.
    player->takeDamage(Cfg::BOSS_DMG);
    attackTimer = Cfg::BOSS_ATK_CD;
}

void bossModel::update(float dt, Vector2f playerPos)
{
    //Check to make sure the boss is alive (or dying) so you don't update an enemy that's already been disabled.
    if (!isActive() && aiState != EnemyAIState::DEAD) return;

    // The explosion flag lasts only one tick
    zoneExplodedFlag = false;
    float initialTimer = zoneExplosionTimer;

    // Transient state management (HURT, DEAD) — bypasses the IA if active.
    if (aiState == EnemyAIState::DEAD) {
        zoneCharging      = false;
        zoneExplodedFlag  = false;
        zoneTiles.clear();
    }
    if (tickTransientStates(dt)) return;

    // Attack cooldown timer.
    if (attackTimer > 0.0f)
        attackTimer -= dt;

    // ── Pattern 2 ────────────────────────────────────────────────
    if (hasPattern2) {

        if (zoneCharging) {
            // Boss remains stationary during the 5-second charge
            velocity = Vector2f(0.0f, 0.0f);
            aiState  = EnemyAIState::ATTACKING;

            zoneExplosionTimer -= dt;
            if (zoneExplosionTimer <= 0.0f) {
                // Explosion: LevelController will read this flag this tick
                zoneExplodedFlag = true;
                zoneCharging     = false;
                zoneChargeTimer  = initialTimer;
            }
            return;
        }


        // The timer counts down ONLY when the player is within the detection range
        if (detectsPlayer(playerPos)) {
            zoneChargeTimer -= dt;
            if (zoneChargeTimer <= 0.0f) {
                buildZoneTiles();
                zoneExplosionTimer = Cfg::BOSS_ZONE_TIMER;
                zoneCharging       = true;
                velocity           = Vector2f(0.0f, 0.0f);
                aiState            = EnemyAIState::ATTACKING;
                return;
            }
        }
    }

    // Check if a player is nearby.
    updateAIState(playerPos);

    // The player is moving so close.
    if (aiState == EnemyAIState::CHASING)
        chase(playerPos, dt);
    else
        velocity = Vector2f(0.0f, 0.0f);
}

void bossModel::chase(Vector2f playerPos, float dt)
{
    if (zoneCharging)       return; // Pattern 2 (priority)
    // Calculating the displacement vector to move and follow the player.
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

void bossModel::updateAIState(Vector2f playerPos)
{
    // Switches between CHASING and IDLE depending on the player's proximity
    if (detectsPlayer(playerPos))
        aiState = EnemyAIState::CHASING;
    else
        aiState = EnemyAIState::IDLE;
}

void bossModel::buildZoneTiles()
{
    zoneTiles.clear();

    const int ts  = Cfg::BOSS_ZONE_TILE_SIZE; // 32 px par tuile
    const int rad = Cfg::BOSS_ZONE_RADIUS;    // rayon en px

    // Boss Center
    float cx = position.x + static_cast<float>(Cfg::ENTITY_SIZE * 3) / 2.0f;
    float cy = position.y + static_cast<float>(Cfg::ENTITY_SIZE * 3) / 2.0f;

    // Range of columns/rows to cover
    int startCol = static_cast<int>(std::floor((cx - rad) / ts));
    int endCol   = static_cast<int>(std::ceil ((cx + rad) / ts));
    int startRow = static_cast<int>(std::floor((cy - rad) / ts));
    int endRow   = static_cast<int>(std::ceil ((cy + rad) / ts));

    // ── Generate valid tiles (excluding walls) ────────────────────
    for (int row = startRow; row < endRow; ++row) {
        for (int col = startCol; col < endCol; ++col) {
            QRectF rect(col * ts, row * ts, ts, ts);

            bool blocked = false;
            for (const QRectF& wall : wallColliders) {
                if (rect.intersects(wall)) { blocked = true; break; }
            }
            if (blocked) continue;

            ZoneTile t;
            t.rect   = rect;
            t.isSafe = false;
            zoneTiles.append(t);
        }
    }

    if (zoneTiles.isEmpty()) return;

    // ── Place the shelters in 4×4-tile blocks ────────────────────
    // Identifying valid top-left corners for a complete 4×4 block:
    // A block is valid if all 16 of its tiles are present (not blocked).

    // Quick index: (row, col) → index in zoneTiles
    QHash<QPair<int,int>, int> tileIndex;
    for (int i = 0; i < zoneTiles.size(); ++i) {
        int c = static_cast<int>(zoneTiles[i].rect.left()  / ts);
        int r = static_cast<int>(zoneTiles[i].rect.top()   / ts);
        tileIndex[{r, c}] = i;
    }

    // Find all valid corners for a 4×4 block
    QVector<QPair<int,int>> validCorners;
    for (int row = startRow; row < endRow - 3; ++row) {
        for (int col = startCol; col < endCol - 3; ++col) {
            bool allPresent = true;
            for (int dr = 0; dr < 4 && allPresent; ++dr)
                for (int dc = 0; dc < 4 && allPresent; ++dc)
                    if (!tileIndex.contains({row+dr, col+dc}))
                        allPresent = false;
            if (allPresent)
                validCorners.append({row, col});
        }
    }

    if (validCorners.isEmpty()) return;

    // Mix and select BOSS_ZONE_SAFE_COUNT blocks (without overlap)
    QRandomGenerator* rng = QRandomGenerator::global();
    for (int i = validCorners.size() - 1; i > 0; --i) {
        int j = static_cast<int>(rng->bounded(static_cast<quint32>(i + 1)));
        std::swap(validCorners[i], validCorners[j]);
    }

    // Mark the selected blocks as safe (avoiding overlaps)
    QSet<QPair<int,int>> usedTiles;
    int placed = 0;
    for (auto& corner : validCorners) {
        if (placed >= Cfg::BOSS_ZONE_SAFE_COUNT) break;

        int r0 = corner.first, c0 = corner.second;

        // Check that no tiles from this block are already in a shelter
        bool overlap = false;
        for (int dr = 0; dr < 4 && !overlap; ++dr)
            for (int dc = 0; dc < 4 && !overlap; ++dc)
                if (usedTiles.contains({r0+dr, c0+dc}))
                    overlap = true;
        if (overlap) continue;

        // Mark the 16 tiles in the block
        for (int dr = 0; dr < 4; ++dr) {
            for (int dc = 0; dc < 4; ++dc) {
                auto key = QPair<int,int>{r0+dr, c0+dc};
                zoneTiles[tileIndex[key]].isSafe = true;
                usedTiles.insert(key);
            }
        }
        ++placed;
    }
}
