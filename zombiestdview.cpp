/**
 * @file zombiestdview.cpp
 * @brief Implementation of the animation and graphics rendering system for standard zombies.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombiestdview.h"
#include <QPainter>

int ZombieStdView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

FacingDirection ZombieStdView::velocityToDir(Vector2f vel) {
    if (std::abs(vel.x) >= std::abs(vel.y))
        return (vel.x >= 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
    else
        return (vel.y >= 0) ? FacingDirection::DOWN  : FacingDirection::UP;
}

ZombieStdView::ZombieStdView(ZombieStdModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
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
                  QString(":/images/Textures/Enemy/Standard/Idle/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Enemy/Standard/Move/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[HURT][d],
                  QString(":/images/Textures/Enemy/Standard/Hurt/%1.png")
                  .arg(dirNames[d]));
        loadSheet(sprites[DEAD][d],
                  QString(":/images/Textures/Enemy/Standard/Dead/%1.png")
                  .arg(dirNames[d]));
    }

    setZValue(Cfg::Z_ENTITY);
    setPos(model->getPosition().toQPointF());
    setPixmap(!sprites[IDLE][0].isEmpty() ? sprites[IDLE][0][0] : makeFallback());
}

// ---- Spritesheet loading (2 frames 32×32 side by side) ----------------------
void ZombieStdView::loadSheet(QVector<QPixmap>& frames, const QString& path) {
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[ZombieStdView] Introuvable : %s", qPrintable(path));
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

QPixmap ZombieStdView::makeFallback() {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(60, 120, 60));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.drawText(pm.rect(), Qt::AlignCenter, "Z");
    return pm;
}

void ZombieStdView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

void ZombieStdView::updateView(float dt) {
    if (!model) return;

    EnemyAIState state = model->getAIState();

    // ── DEAD state: play the animation once, then hide the sprite ────────────
    if (state == EnemyAIState::DEAD || deadAnimFinished) {
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
                        // Last frame reached: hold on it and mark as finished
                        deadAnimFinished = true;
                    }
                }
                setPixmap(dead[qMin(currentFrame, dead.size() - 1)]);
            } else {
                // No DEAD spritesheet: disappear immediately
                deadAnimFinished = true;
            }
        }
        if (deadAnimFinished)
            hide();
        return;
    }

    // ── Inactive enemy without DEAD state (should not happen) ────────────────
    if (!model->isActive()) {
        hide();
        return;
    }

    setPos(model->getPosition().toQPointF());

    // ── HURT state: loop the animation for the duration of HURT ──────────────
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

    // ── Reset currentFrame when leaving a transient state ────────────────────
    Vector2f vel = model->getVelocity();
    bool chasing = (state == EnemyAIState::CHASING);

    if (chasing && vel.length() > 0.01f) {
        // ── Walk: direction inferred from the velocity vector ─────
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
        // ── Idle: fixed frame, direction preserved ────────────────
        currentFrame = 0;
        frameTimer   = 0.0f;
        const QVector<QPixmap>& idle = sprites[IDLE][currentDir];
        setPixmap(!idle.isEmpty() ? idle[0] : makeFallback());
    }
}
