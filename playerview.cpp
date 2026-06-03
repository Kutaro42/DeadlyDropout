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
{
    // Préfixe : :/images  (correspond au qresource prefix="/images" du QRC)
    // Noms : correspondent exactement aux fichiers Textures/Player/{état}/{Dir}.png
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };

    for (int d = 0; d < DIR_COUNT; ++d) {
        loadSheet(sprites[0][d],
                  QString(":/images/Textures/Player/Idle/%1.png").arg(dirNames[d]));
        loadSheet(sprites[1][d],
                  QString(":/images/Textures/Player/Move/%1.png").arg(dirNames[d]));
        //loadSheet(sprites[2][d],
          //        QString(":/images/Textures/Player/Idle/%1.png").arg(dirNames[d]));
    }

    setZValue(Cfg::Z_PLAYER);
    setPos(model->getPosition().toQPointF());

    // Affiche la première frame idle par défaut
    int d = dirIdx(model->getFacing());
    if (!sprites[0][d].isEmpty())
        setPixmap(sprites[0][d][0]);
    else
        showFallback(model->getFacing());
}

// ─── Chargement d'un spritesheet ─────────────────────────────────────────────
// Sheets : 64×32 px → 2 frames de 32×32, scalées à ENTITY_SIZE×ENTITY_SIZE
void PlayerView::loadSheet(QVector<QPixmap>& frames, const QString& path) {
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[PlayerView] Introuvable : %s", qPrintable(path));
        return;
    }
    int frameCount = sheet.width() / FRAME_W;
    for (int i = 0; i < frameCount; ++i) {
        QPixmap raw = sheet.copy(i * FRAME_W, 0, FRAME_W, FRAME_H);
        frames.append(raw.scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}

// ─── Fallback visuel ─────────────────────────────────────────────────────────
void PlayerView::showFallback(FacingDirection dir) {
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
    setPixmap(pm);
}

// ─── Avance d'une frame d'animation ──────────────────────────────────────────
void PlayerView::advanceFrames(float dt, const QVector<QPixmap>& frames) {
    if (frames.isEmpty()) return;
    frameTimer += dt;
    if (frameTimer >= animSpeed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());

    int d = dirIdx(model->getFacing());

    if (model->isMoving()) {
        if (!sprites[1][d].isEmpty())
            advanceFrames(dt, sprites[1][d]);
        else
            showFallback(model->getFacing());
    } else {
        currentFrame = 0;
        frameTimer   = 0.0f;
        if (!sprites[0][d].isEmpty())
            setPixmap(sprites[0][d][0]);
        else
            showFallback(model->getFacing());
    }
}
