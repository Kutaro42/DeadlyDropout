#include "bossmodel.h"
#include "playermodel.h"
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
{
    // Booléen indiquant que cette ennemie est le boss dans le vecteur d'ennemis.
    isBoss = true;
}

// ─── Fourniture des colliders de murs (depuis LevelController) ────
void bossModel::setWallColliders(const QVector<QRectF>& walls)
{
    wallColliders = walls;
}

void bossModel::attack(PlayerModel* player)
{
    // Vérifcation que cooldown est bien écoulé avant une attaque.
    if (attackTimer > 0.0f) return;

    // inflige les dégât au joueur et lance le cooldown.
    player->takeDamage(Cfg::BOSS_DMG);
    attackTimer = Cfg::BOSS_ATK_CD;
}

void bossModel::update(float dt, Vector2f playerPos)
{
    //Vérifie que le boss est en vie (ou en train de mourir) pour ne pas update un ennemi déjà désactivé.
    if (!isActive() && aiState != EnemyAIState::DEAD) return;

    // Le flag d'explosion ne dure qu'un seul tick
    zoneExplodedFlag = false;
    float initialTimer = zoneExplosionTimer;

    // Gestion des états transitoires (HURT, DEAD) — court-circuite l'IA si actif.
    if (aiState == EnemyAIState::DEAD) {
        zoneCharging      = false;
        zoneExplodedFlag  = false;
        zoneTiles.clear();
    }
    if (tickTransientStates(dt)) return;

    // Décompte du cooldown d'attaque.
    if (attackTimer > 0.0f)
        attackTimer -= dt;

    // ── Pattern 2 ────────────────────────────────────────────────
    if (hasPattern2) {

        if (zoneCharging) {
            // Boss immobile pendant les 5s de charge
            velocity = Vector2f(0.0f, 0.0f);
            aiState  = EnemyAIState::ATTACKING;

            zoneExplosionTimer -= dt;
            if (zoneExplosionTimer <= 0.0f) {
                // Explosion : LevelController lira ce flag ce tick
                zoneExplodedFlag = true;
                zoneCharging     = false;

                // On laisse intentionnellement les tuiles en mémoire ici.
                // BossView::hideZone() s'en servira pour nettoyer la scène Qt en toute sécurité.

                zoneChargeTimer  = initialTimer;
            }
            return; // pas de déplacement pendant la charge
        }


        // Timer ne décompte QUE quand le joueur est dans le rayon de détection
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

    // Appel de la vérification si un joueur est proche.
    updateAIState(playerPos);

    // Déplacement si proche du joueur.
    if (aiState == EnemyAIState::CHASING)
        chase(playerPos, dt);
    else
        velocity = Vector2f(0.0f, 0.0f);
}

void bossModel::chase(Vector2f playerPos, float dt)
{
    if (zoneCharging)       return; // pattern 2 prioritaire
    // Calcul du vecteur de déplacement pour déplacer suivre le joueur.
    Vector2f dir = (playerPos - position).normalized();
    velocity     = dir * speed;
    position     = position + (velocity * dt);
}

void bossModel::updateAIState(Vector2f playerPos)
{
    // Alterne entre CHASING et IDLE selon proximité du joueur
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

    // Centre du boss
    float cx = position.x + static_cast<float>(Cfg::ENTITY_SIZE * 3) / 2.0f;
    float cy = position.y + static_cast<float>(Cfg::ENTITY_SIZE * 3) / 2.0f;

    // Plage de colonnes/lignes à couvrir
    int startCol = static_cast<int>(std::floor((cx - rad) / ts));
    int endCol   = static_cast<int>(std::ceil ((cx + rad) / ts));
    int startRow = static_cast<int>(std::floor((cy - rad) / ts));
    int endRow   = static_cast<int>(std::ceil ((cy + rad) / ts));

    // ── Générer les tuiles valides (hors murs) ────────────────────
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

    // ── Placer les refuges en blocs 4×4 tuiles ────────────────────
    // On repère les coins supérieurs-gauches valides pour un bloc 4×4 complet :
    // un bloc est valide si ses 16 tuiles sont toutes présentes (non bloquées).

    // Index rapide : (row, col) → index dans zoneTiles
    QHash<QPair<int,int>, int> tileIndex;
    for (int i = 0; i < zoneTiles.size(); ++i) {
        int c = static_cast<int>(zoneTiles[i].rect.left()  / ts);
        int r = static_cast<int>(zoneTiles[i].rect.top()   / ts);
        tileIndex[{r, c}] = i;
    }

    // Trouver tous les coins valides pour un bloc 4×4
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

    // Mélanger et choisir BOSS_ZONE_SAFE_COUNT blocs (sans chevauchement)
    QRandomGenerator* rng = QRandomGenerator::global();
    for (int i = validCorners.size() - 1; i > 0; --i) {
        int j = static_cast<int>(rng->bounded(static_cast<quint32>(i + 1)));
        std::swap(validCorners[i], validCorners[j]);
    }

    // Marquer les blocs choisis comme sûrs (en évitant les recouvrements)
    QSet<QPair<int,int>> usedTiles;
    int placed = 0;
    for (auto& corner : validCorners) {
        if (placed >= Cfg::BOSS_ZONE_SAFE_COUNT) break;

        int r0 = corner.first, c0 = corner.second;

        // Vérifier qu'aucune tuile de ce bloc n'est déjà dans un refuge
        bool overlap = false;
        for (int dr = 0; dr < 4 && !overlap; ++dr)
            for (int dc = 0; dc < 4 && !overlap; ++dc)
                if (usedTiles.contains({r0+dr, c0+dc}))
                    overlap = true;
        if (overlap) continue;

        // Marquer les 16 tuiles du bloc
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
