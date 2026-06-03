#include "zombiestdview.h"
#include <QPainter>

ZombieStdView::ZombieStdView(ZombieStdModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      model(m),
      currentFrame(0), frameTimer(0.0f), animSpeed(0.2f)
{
    // GDD : spritesheet 32×32 px, 2 frames (walk)
    loadSpriteSheet(":/assets/zombie_walk.png", 32, 32);
    setZValue(Cfg::Z_ENTITY);
}

void ZombieStdView::loadSpriteSheet(const QString& filePath,
                                     int frameW, int frameH)
{
    QPixmap sheet(filePath);
    if (sheet.isNull()) {
        setFallbackPixmap();
        return;
    }

    frames.clear();
    int cols = sheet.width()  / frameW;
    int rows = sheet.height() / frameH;

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            frames.append(sheet.copy(c * frameW, r * frameH, frameW, frameH));

    if (!frames.isEmpty())
        setPixmap(frames[0].scaled(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE,
                                    Qt::IgnoreAspectRatio,
                                    Qt::FastTransformation));
}

void ZombieStdView::setFallbackPixmap() {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(60, 120, 60));   // vert sombre = zombie
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.drawText(pm.rect(), Qt::AlignCenter, "Z");
    setPixmap(pm);
}

void ZombieStdView::updateView(float dt) {
    if (!model || !model->isActive()) {
        hide();
        return;
    }

    // Synchronise la position scène depuis le modèle
    setPos(model->getPosition().toQPointF());

    // Animation seulement si en mouvement (CHASING)
    if (model->getAIState() == EnemyAIState::CHASING && !frames.isEmpty()) {
        frameTimer += dt;
        if (frameTimer >= animSpeed) {
            frameTimer    = 0.0f;
            currentFrame  = (currentFrame + 1) % frames.size();
            setPixmap(frames[currentFrame].scaled(Cfg::ENTITY_SIZE,
                                                   Cfg::ENTITY_SIZE,
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::FastTransformation));
        }
    }
}
