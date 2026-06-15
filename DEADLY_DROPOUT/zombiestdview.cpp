/**
 * @file zombiestdview.cpp
 * @brief Implémentation du système d'animation et de rendu graphique pour les zombies standards.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "zombiestdview.h"
#include <QPainter>

// ─── Direction → index ────────────────────────────────────────────────────────
int ZombieStdView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

// ─── Velocity → FacingDirection (axe dominant) ───────────────────────────────
FacingDirection ZombieStdView::velocityToDir(Vector2f vel) {
    if (std::abs(vel.x) >= std::abs(vel.y))
        return (vel.x >= 0) ? FacingDirection::RIGHT : FacingDirection::LEFT;
    else
        return (vel.y >= 0) ? FacingDirection::DOWN  : FacingDirection::UP;
}

// ─── Constructeur ─────────────────────────────────────────────────────────────
ZombieStdView::ZombieStdView(ZombieStdModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
    , currentFrame(0)
    , frameTimer(0.0f)
    , animSpeed(0.18f)
    , currentDir(0)     // DOWN par défaut
    , deadAnimFinished(false)
    , animSpeedDeath(0.01f)
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

// ─── Chargement spritesheet (2 frames 32×32 côte à côte) ─────────────────────
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

// ─── Fallback ────────────────────────────────────────────────────────────────
QPixmap ZombieStdView::makeFallback() {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(60, 120, 60));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.drawText(pm.rect(), Qt::AlignCenter, "Z");
    return pm;
}

// ─── Avance une frame ────────────────────────────────────────────────────────
void ZombieStdView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
void ZombieStdView::updateView(float dt) {
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
                    if (currentFrame < dead.size() - 1) {
                        ++currentFrame;
                    } else {
                        // Dernière frame atteinte : on reste dessus et on marque la fin
                        deadAnimFinished = true;
                    }
                }
                setPixmap(dead[qMin(currentFrame, dead.size() - 1)]);
            } else {
                // Pas de spritesheet DEAD : disparaître immédiatement
                deadAnimFinished = true;
            }
        }
        if (deadAnimFinished)
            hide();
        return;
    }

    // ── Ennemi inactif sans état DEAD (ne devrait pas arriver) ────────────────
    if (!model->isActive()) {
        hide();
        return;
    }

    setPos(model->getPosition().toQPointF());

    // ── État HURT : joue l'animation en boucle pendant la durée du HURT ───────
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

    // ── Réinitialisation de currentFrame si on sort d'un état transitoire ─────
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
