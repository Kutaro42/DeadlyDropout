/**
 * @file zombielauncherview.cpp
 * @brief Implémentation de la vue du zombie lanceur et de la gestion des vues de gravats.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombielauncherview.h"
#include <QPainter>

// ─── Helpers statiques ────────────────────────────────────────────
int ZombieLauncherView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

FacingDirection ZombieLauncherView::velocityToDir(Vector2f vel) {
    if (std::abs(vel.x) >= std::abs(vel.y))
        return (vel.x >= 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
    else
        return (vel.y >= 0) ? FacingDirection::DOWN  : FacingDirection::UP;
}

// ─── Chargement spritesheet (2 frames 32×32 côte à côte) ─────────
void ZombieLauncherView::loadSheet(QVector<QPixmap>& frames, const QString& path)
{
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[ZombieLauncherView] Introuvable : %s", qPrintable(path));
        return;
    }
    int count = sheet.width() / FRAME_W;
    for (int i = 0; i < count; ++i) {
        QPixmap raw = sheet.copy(i * FRAME_W, 0, FRAME_W, FRAME_H);
        frames.append(raw.scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}

// ─── Fallback violet annoté "L" ───────────────────────────────────
QPixmap ZombieLauncherView::makeFallback()
{
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(100, 40, 140));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 10, QFont::Bold));
    p.drawText(pm.rect(), Qt::AlignCenter, "L");
    return pm;
}

// ─── Constructeur ─────────────────────────────────────────────────
ZombieLauncherView::ZombieLauncherView(ZombieLauncherModel* m,
                                       QGraphicsScene*      sc,
                                       QGraphicsItem*       parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
    , scene(sc)
    , currentFrame(0)
    , frameTimer(0.0f)
    , animSpeed(0.18f)
    , currentDir(0)
    , deadAnimFinished(false)
    , animSpeedDeath(0.01f)
{
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };
    for (int d = 0; d < DIR_COUNT; ++d) {
        loadSheet(sprites[IDLE][d],
                  QString(":/images/Textures/Enemy/Launcher/Idle/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Enemy/Launcher/Move/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[HURT][d],
                  QString(":/images/Textures/Enemy/Launcher/Hurt/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[DEAD][d],
                  QString(":/images/Textures/Enemy/Launcher/Dead/%1.png")
                  .arg(dirNames[d]));
    }

    setZValue(Cfg::Z_ENTITY);
    setPos(model->getPosition().toQPointF());
    setPixmap(!sprites[IDLE][0].isEmpty() ? sprites[IDLE][0][0] : makeFallback());
}

// ─── Avance une frame ─────────────────────────────────────────────
void ZombieLauncherView::advanceFrames(float dt, const QVector<QPixmap>& frames)
{
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Synchronisation RubbleView ↔ RubbleModel ────────────────────
void ZombieLauncherView::syncRubbleViews()
{
    QVector<RubbleModel*>& rubbles = model->getRubbles();

    // 1) Créer les vues manquantes (nouveaux gravats sans vue)
    //    On identifie les modèles qui n'ont pas encore de vue associée
    //    en comparant les tailles : les vues sont ajoutées dans l'ordre
    //    d'insertion, donc rubbleViews[i] correspond à rubbles[i].
    while (rubbleViews.size() < rubbles.size()) {
        int idx = rubbleViews.size();
        auto* rv = new RubbleView(rubbles[idx]);
        scene->addItem(rv);
        rubbleViews.append(rv);
    }

    // 2) Supprimer les vues dont le modèle est inactif
    for (int i = rubbleViews.size() - 1; i >= 0; --i) {
        if (!rubbles[i]->isActive()) {
            scene->removeItem(rubbleViews[i]);
            delete rubbleViews[i];
            rubbleViews.removeAt(i);
        }
    }

    // 3) Mettre à jour la position des vues restantes
    for (RubbleView* rv : rubbleViews)
        rv->updateView();

    // 4) Demander au modèle de libérer ses gravats inactifs
    //    (doit être fait après suppression des vues)
    model->pruneInactiveRubbles();
}

// ─── Mise à jour principale ───────────────────────────────────────
void ZombieLauncherView::updateView(float dt)
{
    if (!model) return;

    EnemyAIState state = model->getAIState();

    // ── État DEAD : joue l'animation une seule fois, puis cache le sprite ──────
    if (state == EnemyAIState::DEAD || deadAnimFinished) {
        if (!deadAnimFinished) {
            setPos(model->getPosition().toQPointF());
            const QVector<QPixmap>& dead = sprites[DEAD][currentDir];
            if (!dead.isEmpty()) {
                frameTimer += dt;
                if (frameTimer >= animSpeedDeath) {
                    frameTimer = 0.0f;
                    if (currentFrame < dead.size() - 1)
                        ++currentFrame;
                    else
                        deadAnimFinished = true;
                }
                setPixmap(dead[qMin(currentFrame, dead.size() - 1)]);
            } else {
                deadAnimFinished = true;
            }
        }
        if (deadAnimFinished) hide();
        syncRubbleViews(); // ← toujours appelé, même après deadAnimFinished
        return;
    }

    if (!model->isActive()) {
        hide();
        syncRubbleViews();
        return;
    }

    setPos(model->getPosition().toQPointF());

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
        syncRubbleViews();
        return;
    }

    // ── Animation du lanceur ──────────────────────────────────
    Vector2f vel      = model->getVelocity();
    bool     chasing  = (state == EnemyAIState::CHASING);

    if (chasing && vel.length() > 0.01f) {
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
        // IDLE ou ATTACKING : frame fixe
        currentFrame = 0;
        frameTimer   = 0.0f;
        const QVector<QPixmap>& idle = sprites[IDLE][currentDir];
        setPixmap(!idle.isEmpty() ? idle[0] : makeFallback());
    }

    // ── Synchronisation des vues de gravats ───────────────────
    syncRubbleViews();
}
