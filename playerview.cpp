#include "playerview.h"
#include <QPainter>
#include <QFont>

// ─── Correspondance FacingDirection → index tableau ─────────────────────────
int PlayerView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

// ─── Constructeur ────────────────────────────────────────────────────────────
PlayerView::PlayerView(PlayerModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
    , currentFrame(0)
    , frameTimer(0.0f)
    , animSpeed(0.15f)
    , attackAnimSpeed(Cfg::KATANA_ANIM_DUR / ATTACK_FRAME_COUNT)
{
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };
    for (int d = 0; d < DIR_COUNT; ++d) {
        // état 0 = idle   (2 frames 32×32)
        loadSheet(sprites[IDLE][d],
                  QString(":/images/Textures/Player/Idle/%1.png").arg(dirNames[d]),
                  FRAME_W, FRAME_H);
        // état 1 = walk   (2 frames 32×32)
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Player/Move/%1.png").arg(dirNames[d]),
                  FRAME_W, FRAME_H);
        // état 2 = attack (3 frames 32×32 — sheet 96×32)
        loadSheet(sprites[ATTACK][d],
                  QString(":/images/Textures/Player/Attack/%1.png").arg(dirNames[d]),
                  FRAME_W, FRAME_H);
    }

    setZValue(Cfg::Z_PLAYER);
    setPos(model->getPosition().toQPointF());

    int d = dirIdx(model->getFacing());
    setPixmap(!sprites[IDLE][d].isEmpty() ? sprites[IDLE][d][0]
                                          : makeFallback(model->getFacing()));
}

// ─── Chargement d'un spritesheet (frames horizontales) ───────────────────────
void PlayerView::loadSheet(QVector<QPixmap>& frames, const QString& path,
                           int fw, int fh)
{
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[PlayerView] Introuvable : %s", qPrintable(path));
        return;
    }
    int count = sheet.width() / fw;
    for (int i = 0; i < count; ++i) {
        QPixmap raw = sheet.copy(i * fw, 0, fw, fh);
        frames.append(raw.scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}

// ─── Fallback visuel ─────────────────────────────────────────────────────────
QPixmap PlayerView::makeFallback(FacingDirection dir) {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(40, 80, 180));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 9, QFont::Bold));
    const char* arrow = "▼";
    switch (dir) {
    case FacingDirection::UP:    arrow = "▲"; break;
    case FacingDirection::LEFT:  arrow = "◀"; break;
    case FacingDirection::RIGHT: arrow = "▶"; break;
    default: break;
    }
    p.drawText(pm.rect(), Qt::AlignCenter, QString("P\n") + arrow);
    return pm;
}

// ─── Avance une frame et l'affiche ───────────────────────────────────────────
void PlayerView::advanceFrames(float dt, const QVector<QPixmap>& frames, float speed)
{
    if (frames.isEmpty()) return;

    // Sécurité : currentFrame peut venir d'un état avec plus de frames
    if (currentFrame >= frames.size()) {
        currentFrame = 0;
        frameTimer   = 0.0f;
    }

    frameTimer += dt;
    if (frameTimer >= speed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
// Priorité : ATTACK > WALK > IDLE
void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());
    int d = dirIdx(model->getFacing());

    if (model->isKatanaActive()) {
        // ── Animation attaque (priorité maximale) ─────────────
        const QVector<QPixmap>& atk = sprites[ATTACK][d];
        if (!atk.isEmpty())
            advanceFrames(dt, atk, attackAnimSpeed);
        else
            setPixmap(makeFallback(model->getFacing()));

    } else if (model->isMoving()) {
        // ── Animation marche ──────────────────────────────────
        // Repart à la frame 0 si on sort de l'état attaque
        if (currentFrame >= sprites[WALK][d].size()) {
            currentFrame = 0;
            frameTimer   = 0.0f;
        }
        const QVector<QPixmap>& walk = sprites[WALK][d];
        if (!walk.isEmpty())
            advanceFrames(dt, walk, animSpeed);
        else
            setPixmap(makeFallback(model->getFacing()));

    } else {
        // Idle : animation cyclique (comme WALK)
        const QVector<QPixmap>& idle = sprites[IDLE][d];
        if (!idle.isEmpty())
            advanceFrames(dt, idle, animSpeed);
        else
            setPixmap(makeFallback(model->getFacing()));
    }
}
