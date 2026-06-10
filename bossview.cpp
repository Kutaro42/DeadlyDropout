#include "bossview.h"

#include <QPainter>
#include <QFont>
#include <cmath>

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

{
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };
    for (int d = 0; d < DIR_COUNT; ++d) {
        loadSheet(sprites[IDLE][d],
                  QString(":/images/Textures/Enemy/Boss/Idle/%1.png")
                      .arg(dirNames[d]));
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Enemy/Boss/Move/%1.png")
                      .arg(dirNames[d]));
    }

    setZValue(Cfg::Z_ENTITY);
    setPos(model->getPosition().toQPointF());
    setPixmap(!sprites[IDLE][0].isEmpty() ? sprites[IDLE][0][0] : makeFallback());


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
        frames.append(raw.scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
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






// ─── Avance une frame ────────────────────────────────────────────────────────
void BossView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    // Sécurité si erreur d'importation des sprites.
    if (frames.isEmpty()) return;

    // Mise a jour du timer pour ensuite afficher le bon sprite dans l'animation
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
void BossView::updateView(float dt) {

    // Sécurité pour ne plus afficher le boss si il est mort
    if (!model || !model->isActive()) {
        hide();
        return;
    }

    // lis le model pour syncroniser l'entité et le sprite
    setPos(model->getPosition().toQPointF());
    Vector2f vel = model->getVelocity();
    bool chasing = (model->getAIState() == EnemyAIState::CHASING);

    // Déplace le boss si en chasse puis rafraichi le sprite selon l'état et le temps
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


