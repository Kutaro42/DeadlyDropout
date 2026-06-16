/**
 * @file rubbleview.cpp
 * @brief Implementation of the graphical view of the rubble.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "rubbleview.h"
#include <QPainter>

QPixmap RubbleView::loadSprite()
{
    const int sz = static_cast<int>(Cfg::RUBBLE_SIZE);

    // Path to adjust according to the .qrc file organization
    QPixmap pm(":/images/Textures/Enemy/Launcher/rubble.png");
    if (!pm.isNull())
        return pm.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // ── Fallback: annotated grey-brown square ────────────────────────
    pm = QPixmap(sz, sz);
    pm.fill(QColor(110, 90, 60));
    QPainter p(&pm);
    p.setPen(QColor(200, 170, 120));
    p.drawRect(1, 1, sz - 3, sz - 3);
    return pm;
}

RubbleView::RubbleView(RubbleModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
{
    static const QPixmap sprite = loadSprite(); // loaded only once
    setPixmap(sprite);
    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_ENTITY);
}

void RubbleView::updateView()
{
    if (!model || !model->isActive()) {
        hide();
        return;
    }
    setPos(model->getPosition().toQPointF());
}
