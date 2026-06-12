#include "levelcontroller.h"
#include "bossmodel.h"

LevelController::LevelController()
    : levelJustCompleted(false)
{}

// ── Point d'entrée principal ──────────────────────────────────────
void LevelController::update(float dt, PlayerModel& player, LevelModel& level)
{
    levelJustCompleted = false;
    pendingPopups.clear();

    updateEnemies(dt, player, level);
    distributeGold(player, level);

    resolvePlayerWallCollisions(player, level);
    resolveEnemyWallCollisions(level);

    resolveEnemyEnemyCollisions(level);
    resolveEnemyWallCollisions(level);

    checkAndResolvePlayerEnemyCollisions(player, level);

    resolvePlayerWallCollisions(player, level);
    resolveEnemyWallCollisions(level);

    if (player.isKatanaActive()) {
        checkSlashZoneHits(player, level);
    } else {
        for (Enemy* e : level.getEnemies()) {
            if (e->getAIState() == EnemyAIState::DEAD) continue;
            e->setHitThisSlash(false);
        }
    }

    checkRubbleCollisions(player, level);
    checkBulletCollisions(player, level);
    checkBossZonePattern(player, level);

    checkStandardDoorInteractions(player, level);
    checkBossDoorInteractions(player, level);
    checkBossZonePattern(player, level);
    checkExitCondition(player, level);

    //purgeDeadEnemies(level); // ← absolument en dernier

}
// ── IA : mise à jour de chaque ennemi actif ───────────────────────
void LevelController::updateEnemies(float dt,
                                    PlayerModel& player,
                                    LevelModel&  level)
{
    for (Enemy* e : level.getEnemies())
        if (e->isActive() || e->getAIState() == EnemyAIState::DEAD)
            e->update(dt, player.getPosition());

}

void LevelController::purgeDeadEnemies(LevelModel& level)
{
    QVector<Enemy*>& enemies = level.getEnemies();
    for (int i = enemies.size() - 1; i >= 0; --i) {
        if (!enemies[i]->isActive() && enemies[i]->getAIState() != EnemyAIState::DEAD) {
            delete enemies[i];
            enemies.removeAt(i);
        }
    }
}

// ── Gold : distribué une seule fois à la mort de chaque ennemi ────
void LevelController::distributeGold(PlayerModel& player,
                                     LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() && !e->hasDroppedGold()) {
            // Gold de base (fixé par type d'ennemi) + bonus aléatoire 5-10 (GDD)
            int bonus = Cfg::ZOMBIE_GOLD_MIN + (rand() % (Cfg::ZOMBIE_GOLD_MAX - Cfg::ZOMBIE_GOLD_MIN + 1));
            int gold  = e->getGoldDrop() + bonus;
            player.addGold(gold);
            e->markGoldDropped();
            pendingPopups.append({gold, PopupRequest::Type::Gold, popupPos(e)});
        }
    }
}

// ── Résolution AABB avec position de référence ───────────────────
static void resolveAABBWithRef(Entity& mover,
                               const QRectF& obstacle,
                               const Vector2f& refPos)
{
    QRectF m = mover.getCollider();
    if (!m.intersects(obstacle)) return;

    QRectF inter = m.intersected(obstacle);
    double pw = inter.width();
    double ph = inter.height();

    double refCx = static_cast<double>(refPos.x) + m.width()  / 2.0;
    double refCy = static_cast<double>(refPos.y) + m.height() / 2.0;
    double obsCx = obstacle.left() + obstacle.width()  / 2.0;
    double obsCy = obstacle.top()  + obstacle.height() / 2.0;

    double signX = (refCx <= obsCx) ? -1.0 : 1.0;
    double signY = (refCy <= obsCy) ? -1.0 : 1.0;

    Vector2f pos = mover.getPosition();

    if (pw < ph) {
        mover.setPosition(Vector2f(pos.x + static_cast<float>(signX * pw), pos.y));
    } else if (ph < pw) {
        mover.setPosition(Vector2f(pos.x, pos.y + static_cast<float>(signY * ph)));
    } else {
        mover.setPosition(Vector2f(pos.x + static_cast<float>(signX * pw),
                                   pos.y + static_cast<float>(signY * ph)));
    }
}

void LevelController::resolveAABB(Entity& mover, const QRectF& obstacle)
{
    resolveAABBWithRef(mover, obstacle, mover.getPosition());
}

// ── Collisions joueur / murs ──────────────────────────────────────
void LevelController::resolvePlayerWallCollisions(PlayerModel& player,
                                                  LevelModel&  level)
{
    for (WallModel* w : level.getWalls())
        resolveAABB(player, w->getCollider());
}

// ── Collisions ennemis / murs ─────────────────────────────────────
void LevelController::resolveEnemyWallCollisions(LevelModel& level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() || e->getAIState() == EnemyAIState::DEAD) continue;
        Vector2f refPos = e->getPosition();
        for (int pass = 0; pass < 4; ++pass) {
            for (WallModel* w : level.getWalls())
                resolveAABBWithRef(*e, w->getCollider(), refPos);
            // Les portes fermées sont opaques pour les ennemis
            for (StandardDoorModel* d : level.getStandardDoors())
                if (!d->isOpen())
                    resolveAABBWithRef(*e, d->getCollider(), refPos);
            for (BossDoorModel* d : level.getBossDoors())
                if (!d->isLocked())
                    resolveAABBWithRef(*e, d->getCollider(), refPos);
        }
    }
}

// ── Joueur bloqué par les ennemis ────────────────────────────────
void LevelController::resolvePlayerVsEnemyWalls(PlayerModel& player,
                                                LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() || e->getAIState() == EnemyAIState::DEAD) continue;
        resolveAABB(player, e->getCollider());
    }
}

// ── Contact joueur / ennemi → attaque puis séparation ────────────
// Au lieu de pousser l'ennemi (ce qui peut l'envoyer dans un mur),
// on pousse le JOUEUR en arrière. L'ennemi reste sur place.
// Si la séparation est impossible sans traverser un mur, l'ennemi
// ne bouge pas du tout — il est bloqué par le mur.
void LevelController::checkAndResolvePlayerEnemyCollisions(PlayerModel& player,
                                                           LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() || e->getAIState() == EnemyAIState::DEAD) continue;
        if (!player.getCollider().intersects(e->getCollider())) continue;

        e->attack(&player);

        // Sauvegarder la position de l'ennemi avant toute tentative de poussée
        Vector2f enemyRefPos = e->getPosition();

        // Tenter de séparer l'ennemi du joueur
        resolveAABBWithRef(*e, player.getCollider(), enemyRefPos);

        // Vérifier si la nouvelle position intersecte un mur
        bool inWall = false;
        for (WallModel* w : level.getWalls()) {
            if (e->getCollider().intersects(w->getCollider())) {
                inWall = true;
                break;
            }
        }

        if (inWall) {
            // L'ennemi ne peut pas reculer sans traverser un mur :
            // on annule son déplacement et on pousse le joueur à la place.
            e->setPosition(enemyRefPos);
            resolveAABB(player, e->getCollider());
        }
    }
}

// ── SlashZone katana / ennemis ────────────────────────────────────
void LevelController::checkSlashZoneHits(PlayerModel& player,
                                         LevelModel&  level)
{
    QRectF slash = player.getSlashZone();
    for (Enemy* e : level.getEnemies()) {
        if (!e->isActive() || e->getAIState() == EnemyAIState::DEAD) continue;
        if (e->getHitThisSlash()) continue;

        if (slash.intersects(e->getCollider())) {
            int dmg = player.getKatana().getDamage();
            e->takeDamage(dmg);
            e->setHitThisSlash(true);
            pendingPopups.append({dmg, PopupRequest::Type::Damage, popupPos(e)});
        }
    }
}

// ── Séparation ennemi / ennemi ────────────────────────────────────
void LevelController::resolveEnemyEnemyCollisions(LevelModel& level)
{
    QVector<Enemy*>& enemies = level.getEnemies();
    int n = enemies.size();
    for (int i = 0; i < n; ++i) {
        if (!enemies[i]->isActive() || enemies[i]->getAIState() == EnemyAIState::DEAD) continue;
        for (int j = i + 1; j < n; ++j) {
            if (!enemies[j]->isActive() || enemies[j]->getAIState() == EnemyAIState::DEAD) continue;

            QRectF a = enemies[i]->getCollider();
            QRectF b = enemies[j]->getCollider();
            if (!a.intersects(b)) continue;

            QRectF inter = a.intersected(b);
            float hx = 0.0f, hy = 0.0f;

            if (inter.width() < inter.height()) {
                float corr = static_cast<float>(inter.width()) * 0.5f;
                hx = (a.center().x() < b.center().x()) ? -corr : corr;
            } else {
                float corr = static_cast<float>(inter.height()) * 0.5f;
                hy = (a.center().y() < b.center().y()) ? -corr : corr;
            }

            Vector2f pa = enemies[i]->getPosition();
            Vector2f pb = enemies[j]->getPosition();
            enemies[i]->setPosition(Vector2f(pa.x + hx, pa.y + hy));
            enemies[j]->setPosition(Vector2f(pb.x - hx, pb.y - hy));
        }
    }
}

// ── Gravats (projectiles lanceur) / joueur + murs ─────────────────
// Itère sur tous les ZombieLauncherModel (via dynamic_cast) et vérifie
// les collisions de leurs RubbleModel avec le joueur et les murs.
void LevelController::checkRubbleCollisions(PlayerModel& player, LevelModel& level)
{
    for (Enemy* e : level.getEnemies()) {
        auto* launcher = dynamic_cast<ZombieLauncherModel*>(e);
        if (!launcher) continue;

        for (RubbleModel* r : launcher->getRubbles()) {
            if (!r->isActive()) continue;

            QRectF rb = r->getCollider();

            if (rb.intersects(player.getCollider())) {
                player.takeDamage(r->getDamage());
                r->setActive(false);
                continue;
            }

            for (WallModel* w : level.getWalls()) {
                if (rb.intersects(w->getCollider())) { r->setActive(false); break; }
            }
            if (!r->isActive()) continue;

            for (StandardDoorModel* d : level.getStandardDoors()) {
                if (!d->isOpen() && rb.intersects(d->getCollider())) { r->setActive(false); break; }
            }
            if (!r->isActive()) continue;

            for (BossDoorModel* d : level.getBossDoors()) {
                if (rb.intersects(d->getCollider())) { r->setActive(false); break; }
            }
        }
        // ← plus d'appel à pruneInactiveRubbles() ici
    }
}

// ── Balles joueur / ennemis + murs ───────────────────────────────
void LevelController::checkBulletCollisions(PlayerModel& player, LevelModel& level)
{
    GunModel& gun = player.getGun();

    for (BulletModel* b : gun.getBullets()) {
        if (!b->isActive()) continue;

        QRectF br = b->getCollider();
        bool hit = false;

        for (Enemy* e : level.getEnemies()) {
            if (!e->isActive() || e->getAIState() == EnemyAIState::DEAD) continue;
            if (br.intersects(e->getCollider())) {
                int dmg = b->getDamage();
                e->takeDamage(dmg);
                pendingPopups.append({dmg, PopupRequest::Type::Damage, popupPos(e)});
                b->setActive(false);
                hit = true;
                break;
            }
        }
        if (hit) continue;

        for (WallModel* w : level.getWalls()) {
            if (br.intersects(w->getCollider())) { b->setActive(false); break; }
        }
        if (!b->isActive()) continue;

        for (StandardDoorModel* d : level.getStandardDoors()) {
            if (!d->isOpen() && br.intersects(d->getCollider())) { b->setActive(false); break; }
        }
        if (!b->isActive()) continue;

        for (BossDoorModel* d : level.getBossDoors()) {
            if (br.intersects(d->getCollider())) { b->setActive(false); break; }
        }
    }

    // ← purge des balles inactives
    gun.pruneInactiveBullets();
}

// ── Portes standard : ouverture au contact + collision tant que fermées ───────
// Lorsqu'une porte est fermée, elle bloque le joueur comme un mur.
// Dès que le joueur la touche, elle s'ouvre et n'est plus un obstacle.
void LevelController::checkStandardDoorInteractions(PlayerModel& player,
                                                    LevelModel&  level)
{
    for (StandardDoorModel* door : level.getStandardDoors()) {
        if (door->isOpen()) continue;

        if (door->tryOpen(player)) {
            // La porte vient de s'ouvrir : pas de résolution de collision,
            // le joueur peut s'y avancer librement dès ce tick.
            continue;
        }

        // Porte encore fermée → bloque le joueur (même comportement qu'un mur)
        resolveAABB(player, door->getCollider());
    }
}


// ── Portes de boss : ouverture à la mort du boss + collision tant que fermées ─
// La porte s'ouvre automatiquement dès qu'aucun boss actif n'est présent dans
// le niveau. Tant qu'elle est fermée, elle bloque le joueur comme un mur.
// ── Portes de boss : téléportation unique puis verrouillage ───────────────────
void LevelController::checkBossDoorInteractions(PlayerModel& player,
                                                LevelModel&  level)
{
    for (BossDoorModel* door : level.getBossDoors()) {
        if (door->isLocked()) {
            // Verrouillée → bloque joueur, ennemis, balles, gravats (géré ailleurs)
            resolveAABB(player, door->getCollider());
            continue;
        }

        Vector2f teleportPos;
        if (door->tryTraverse(player.getCollider(), teleportPos))
            player.setPosition(teleportPos);
        // Pas de resolveAABB si on vient de traverser (le joueur est déjà de l'autre côté)
    }
}

// ── Condition de victoire ─────────────────────────────────────────
void LevelController::checkExitCondition(PlayerModel& player,
                                         LevelModel&  level)
{
    ExitDoorModel* exitDoor = level.getExitDoor();
    level.bossDefeat();
    if (!exitDoor) return;
    if (level.getEnemies().isEmpty()) return;

    if (player.getHasKey() && !exitDoor->isOpen())
        exitDoor->openDoor();

    if (exitDoor->isOpen() &&
        player.getCollider().intersects(exitDoor->getCollider()) && player.getHasKey())
    {
        player.setHasKey(false);
        levelJustCompleted = true;
    }
}

void LevelController::checkBossZonePattern(PlayerModel& player,
                                           LevelModel&  level)
{
    for (Enemy* e : level.getEnemies()) {
        bossModel* boss = dynamic_cast<bossModel*>(e);
        if (!boss || !boss->isActive()) continue;

        // ── 1. Fournir les colliders de murs (calcul unique, mis en cache) ─
        if (wallCollidersCache.isEmpty() && !level.getWalls().isEmpty()) {
            wallCollidersCache.reserve(level.getWalls().size());
            for (WallModel* w : level.getWalls())
                wallCollidersCache.append(w->getCollider());
        }
        if (!wallCollidersCache.isEmpty())
            boss->setWallColliders(wallCollidersCache);

        // ── 2. Explosion : appliquer les dégâts ───────────────────────────
        if (!boss->zoneJustExploded()) continue;
        boss->clearZoneExploded();

        QRectF playerRect = player.getCollider();
        bool   hit        = false;

        for (const ZoneTile& tile : boss->getZoneTiles()) {
            if (tile.isSafe) continue;
            if (playerRect.intersects(tile.rect)) { hit = true; break; }
        }

        if (hit) {
            int hp = player.getHp();
            if (hp <= 1)
                player.takeDamage(hp);                  // mort directe
            else
                player.takeDamage(Cfg::BOSS_ZONE_DMG);  // 70 HP fixes
        }
    }
}

// ── Helper popup ──────────────────────────────────────────────────
QPointF LevelController::popupPos(const Enemy* e)
{
    float cx = e->getPosition().x + e->getWidth()  / 2.0f;
    float ty = e->getPosition().y - 6.0f;
    return QPointF(static_cast<double>(cx), static_cast<double>(ty));
}
