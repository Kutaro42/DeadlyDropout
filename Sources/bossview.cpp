/**
 * @file bossview.cpp
 * @brief Vue du boss : sprite animé + rendu zone d'attaque (pattern 2).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "Headers/bossview.h"
#include <QPainter>
#include <QGraphicsScene>

// ─── Direction → index ────────────────────────────────────────────────────────
int BossView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

// ─── Velocity → FacingDirection (axe dominant) ───────────────────────────────
FacingDirection BossView::velocityToDir(Vector2f vel) {
    if (std::abs(vel.x) >= std::abs(vel.y))
        return (vel.x >= 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
    else
        return (vel.y >= 0) ? FacingDirection::DOWN  : FacingDirection::UP;
}

BossView::BossView(bossModel* model, QGraphicsItem* parent) :
    QGraphicsPixmapItem(parent)
    , model(model)
    , currentFrame(0)
    , frameTimer(0.0f)
    , animSpeed(0.18f)
    , currentDir(0)     // DOWN par défaut
    , deadAnimFinished(false)
    , animSpeedDeath(0.10f)

{
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };
    for (int d = 0; d < DIR_COUNT; ++d) {
        loadSheet(sprites[IDLE][d],
                  QString(":/images/Textures/Enemy/Boss/Idle/%1.png")
                      .arg(dirNames[d]));
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Enemy/Boss/Move/%1.png")
                      .arg(dirNames[d]));
        loadSheet(sprites[HURT][d],
                  QString(":/images/Textures/Enemy/Boss/Hurt/%1.png")
                      .arg(dirNames[d]));
        loadSheet(sprites[DEAD][d],
                  QString(":/images/Textures/Enemy/Boss/Dead/%1.png")
                      .arg(dirNames[d]));
    }

    setZValue(Cfg::Z_ENTITY);
    setPos(model->getPosition().toQPointF());
    setPixmap(!sprites[IDLE][0].isEmpty() ? sprites[IDLE][0][0] : makeFallback());

}


BossView::~BossView()
{
    // 2. Désallocation sécurisée de chaque tuile graphique
    for (QGraphicsRectItem* item : zoneItems) {
        if (item) {
            scene()->removeItem(item);
            delete item;

        }
    }
}

void BossView::loadSheet(QVector<QPixmap>& frames, const QString& path)
{
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[BossView] Introuvable : %s", qPrintable(path));
        return;
    }
    int count = sheet.width() / FRAME_W;
    for (int i = 0; i < count; ++i) {
        QPixmap raw = sheet.copy(i * FRAME_W, 0, FRAME_W, FRAME_H);
        frames.append(raw.scaled(Cfg::ENTITY_SIZE * 3, Cfg::ENTITY_SIZE * 3,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}


// ─── Fallback ────────────────────────────────────────────────────────────────
QPixmap BossView::makeFallback()
{
    QPixmap pm(Cfg::ENTITY_SIZE*3, Cfg::ENTITY_SIZE*3);
    pm.fill(QColor(60, 120, 60));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.drawText(pm.rect(), Qt::AlignCenter, "B");
    return pm;
}

// ─── Affichage de la zone (pattern 2) ────────────────────────────
void BossView::showZone()
{
    QGraphicsScene* sc = scene();
    if (!sc) return;
    hideZone(); // purger d'éventuels restes
    int first = 0;
    for (const ZoneTile& t : model->getZoneTiles()) {
        auto* item = new QGraphicsRectItem(t.rect);
        item->setZValue(Cfg::Z_BOSS_ZONE);

        if (t.isSafe) {
            // Refuge : complètement transparent
            item->setBrush(Qt::NoBrush);
            item->setPen(Qt::NoPen);
        } else {
            // Danger : rouge semi-transparent
            item->setBrush(QBrush(QColor(220, 30, 30, 130)));
            item->setPen(QPen(QColor(255, 60, 60, 200), 1));
        }
        sc->addItem(item);
        zoneItems.append(item);

    }
    zoneVisible = true;
}

// ─── Suppression de la zone ───────────────────────────────────────
// item->scene() est testé AVANT removeItem pour éviter le double-free
// si scene()->clear() a déjà détruit les items (segfault précédent).
void BossView::hideZone()
{
    // 1. On vérifie si la vue est bien rattachée à une scène Qt
    if (!scene()) {
        zoneItems.clear();
        zoneVisible = false;
        return;
    }

    // 2. Désallocation sécurisée de chaque tuile graphique
    for (QGraphicsRectItem* item : zoneItems) {
        if (item) {
            // On le retire proprement de la scène de rendu
            scene()->removeItem(item);
            // On détruit la mémoire de l'item graphique
            qDebug() << item;
            //delete item;

        }
    }

    // 3. On vide le conteneur et on reset le flag pour couper le rendu
    zoneItems.clear();
    zoneVisible = false;
}


// ─── Avance une frame ────────────────────────────────────────────────────────
void BossView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
void BossView::updateView(float dt) {
    if (!model) return;

    EnemyAIState state = model->getAIState();

    // ── État DEAD : joue l'animation une seule fois, puis cache le sprite ──────
    if (state == EnemyAIState::DEAD || deadAnimFinished) {
        if (zoneVisible) hideZone();
        if (!deadAnimFinished) {
            setPos(model->getPosition().toQPointF());
            const QVector<QPixmap>& dead = sprites[DEAD][currentDir];
            if (!dead.isEmpty()) {
                frameTimer += dt;
                if (frameTimer >= animSpeedDeath) {
                    frameTimer = 0.0f;
                    if (currentFrame < dead.size() - 1) {
                        ++currentFrame;
                    } else {
                        deadAnimFinished = true;
                    }
                }
                setPixmap(dead[qMin(currentFrame, dead.size() - 1)]);
            } else {
                deadAnimFinished = true;
            }
        }
        if (deadAnimFinished)
            hide();
        return;
    }

    // ── Ennemi inactif sans état DEAD ─────────────────────────────────────────
    if (!model->isActive()) {
        hide();
        return;
    }

    setPos(model->getPosition().toQPointF());

    // ── Zone d'attaque ────────────────────────────────────────────
    if (model->isZoneCharging()) {
        if (!zoneVisible) showZone();   // créer une seule fois au déclenchement
    } else {
        if (zoneVisible)
        {
            hideZone();            // retirer après l'explosion (tick suivant)
        }
    }

    // ── État HURT : animation en boucle pendant la durée du HURT ─────────────
    if (state == EnemyAIState::HURT) {
        const QVector<QPixmap>& hurt = sprites[HURT][currentDir];
        if (!hurt.isEmpty()) {
            frameTimer += dt;
            if (frameTimer >= animSpeed) {
                frameTimer   = 0.0f;
                currentFrame = (currentFrame + 1) % hurt.size();
            }
            setPixmap(hurt[currentFrame]);
        } else {
            setPixmap(makeFallback());
        }
        return;
    }

    Vector2f vel = model->getVelocity();
    bool chasing = (state == EnemyAIState::CHASING);

    if (chasing && vel.length() > 0.01f) {
        // ── Walk : direction déduite du vecteur vitesse ────────
        int newDir = dirIdx(velocityToDir(vel));
        if (newDir != currentDir) {
            currentDir   = newDir;
            currentFrame = 0;
            frameTimer   = 0.0f;
        }
        const QVector<QPixmap>& walk = sprites[WALK][currentDir];
        if (!walk.isEmpty())
            advanceFrames(dt, walk);
        else
            setPixmap(makeFallback());

    } else {
        // ── Idle : frame fixe, direction conservée ────────────
        currentFrame = 0;
        frameTimer   = 0.0f;
        const QVector<QPixmap>& idle = sprites[IDLE][currentDir];
        setPixmap(!idle.isEmpty() ? idle[0] : makeFallback());
    }
}
