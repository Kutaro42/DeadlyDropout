#include "playerview.h"
#include <QPainter>
#include <QFont>

int PlayerView::dirIdx(FacingDirection d) {
    switch (d) {
        case FacingDirection::DOWN:  return 0;
        case FacingDirection::UP:    return 1;
        case FacingDirection::LEFT:  return 2;
        case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

PlayerView::PlayerView(PlayerModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      model(m),
      currentFrame(0), frameTimer(0.0f),
      animSpeed(0.15f), prevAttacking(false)
{
    const QString dirNames[4] = { "down", "up", "left", "right" };
    for (int d = 0; d < 4; ++d) {
        loadSheet(sprites[0][d],
                  QString(":/assets/player_idle_%1.png").arg(dirNames[d]), 64, 64);
        loadSheet(sprites[1][d],
                  QString(":/assets/player_walk_%1.png").arg(dirNames[d]), 64, 64);
        loadSheet(sprites[2][d],
                  QString(":/assets/katana_slash_%1.png").arg(dirNames[d]), 64, 64);
    }
    setZValue(Cfg::Z_PLAYER);
    // Position initiale
    setPos(model->getPosition().toQPointF());
    showFallback(model->getFacing());
}

void PlayerView::loadSheet(QVector<QPixmap>& frames,
                             const QString& path, int fw, int fh)
{
    QPixmap sheet(path);
    if (sheet.isNull()) return;
    int cols = sheet.width()  / fw;
    int rows = sheet.height() / fh;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            frames.append(sheet.copy(c * fw, r * fh, fw, fh));
}

void PlayerView::showFallback(FacingDirection dir) {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(40, 80, 180));   // bleu = joueur
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 9, QFont::Bold));
    // Petite flèche indiquant la direction
    const char* arrow = "▼";
    switch (dir) {
        case FacingDirection::UP:    arrow = "▲"; break;
        case FacingDirection::LEFT:  arrow = "◀"; break;
        case FacingDirection::RIGHT: arrow = "▶"; break;
        default: break;
    }
    p.drawText(pm.rect(), Qt::AlignCenter, QString("P\n") + arrow);
    setPixmap(pm);
}

void PlayerView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer    = 0.0f;
        currentFrame  = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame].scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                           Qt::IgnoreAspectRatio,
                                           Qt::FastTransformation));
}

void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());

    int  d         = dirIdx(model->getFacing());
    bool attacking = model->isKatanaActive();

    // Réinitialise la frame au début d'une nouvelle animation de tranche
    if (attacking && !prevAttacking) {
        currentFrame = 0;
        frameTimer   = 0.0f;
    }
    prevAttacking = attacking;

    // ─── Sélection de l'état d'animation ──────────────────────
    if (attacking && !sprites[2][d].isEmpty()) {
        advanceFrames(dt, sprites[2][d]);   // tranche (slash)
    } else if (model->isMoving() && !sprites[1][d].isEmpty()) {
        advanceFrames(dt, sprites[1][d]);   // marche (walk)
    } else if (!sprites[0][d].isEmpty()) {
        // Idle : frame fixe (pas de cycling)
        setPixmap(sprites[0][d][0].scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                           Qt::IgnoreAspectRatio,
                                           Qt::FastTransformation));
    } else {
        showFallback(model->getFacing());
    }
}
