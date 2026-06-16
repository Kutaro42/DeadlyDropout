/**
 * @file zombielauncherview.cpp
 * @brief Implementation of the zombie launcher view and debris view management.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombielauncherview.h"
#include <QPainter>

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

// ---- Spritesheet loading (2 frames 32×32 side by side) ---------------------
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

// ---- Purple fallback annotated "L" -----------------------------------------
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
    , animSpeedDeath(0.10f)
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

// ---- Advance one frame -----------------------------------------------------
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

// ---- RubbleView <-> RubbleModel synchronization ----------------------------
void ZombieLauncherView::syncRubbleViews()
{
    QVector<RubbleModel*>& rubbles = model->getRubbles();

    //    Create missing views for newly fired rubble.
    //    We identify models that do not yet have an associated view
    //    by comparing sizes: views are added in insertion order,
    //    so rubbleViews[i] corresponds to rubbles[i].
    while (rubbleViews.size() < rubbles.size()) {
        int idx = rubbleViews.size();
        auto* rv = new RubbleView(rubbles[idx]);
        scene->addItem(rv);
        rubbleViews.append(rv);
    }

    // Remove views whose model has become inactive
    for (int i = rubbleViews.size() - 1; i >= 0; --i) {
        if (!rubbles[i]->isActive()) {
            scene->removeItem(rubbleViews[i]);
            delete rubbleViews[i];
            rubbleViews.removeAt(i);
        }
    }

    // Update the position of remaining views
    for (RubbleView* rv : rubbleViews)
        rv->updateView();

    // Ask the model to release its inactive rubble (must be done after removing views
    // to avoid accessing an already-freed model.)
    model->pruneInactiveRubbles();
}

// ---- Main update -----------------------------------------------------------
void ZombieLauncherView::updateView(float dt)
{
    if (!model) return;

    EnemyAIState state = model->getAIState();

    // ---- DEAD state: play the animation once, then hide the sprite ----------
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
        syncRubbleViews(); // always called, even after deadAnimFinished
        return;
    }

    if (!model->isActive()) {
        hide();
        syncRubbleViews();
        return;
    }

    setPos(model->getPosition().toQPointF());

    // ---- HURT state: looping animation for the duration of HURT -------------
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

    // ---- Launcher animation -------------------------------------------------
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
        // IDLE or ATTACKING: fixed frame
        currentFrame = 0;
        frameTimer   = 0.0f;
        const QVector<QPixmap>& idle = sprites[IDLE][currentDir];
        setPixmap(!idle.isEmpty() ? idle[0] : makeFallback());
    }

    // ---- Rubble view synchronization ----------------------------------------
    syncRubbleViews();
}
