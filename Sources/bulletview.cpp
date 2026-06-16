#include "Headers/bulletview.h"
#include <QPainter>
#include <QDebug>

QPixmap BulletView::loadSprite()
{
    QPixmap pm(":/images/Textures/Player/Bullet/bullet32-32.png");
    if (!pm.isNull())
        qDebug("Texture des balles chargée avec succès !");

        return pm.scaled(static_cast<int>(Cfg::BULLET_SIZE),
                         static_cast<int>(Cfg::BULLET_SIZE),
                         Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);

    // Fallback : cercle jaune
    int sz = static_cast<int>(Cfg::BULLET_SIZE);
    pm = QPixmap(sz, sz);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QColor(255, 220, 60));
    p.setPen(Qt::NoPen);
    p.drawEllipse(0, 0, sz, sz);
    return pm;
}

BulletView::BulletView(BulletModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), model(m)
{
    static const QPixmap sprite = loadSprite();
    setPixmap(sprite);
    setZValue(Cfg::Z_ENTITY + 0.5);
}

void BulletView::updateView()
{
    setPos(model->getPosition().toQPointF());
}
