/**
 * @file rubbleview.cpp
 * @brief Implémentation de la vue graphique du gravat.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "rubbleview.h"
#include <QPainter>

// ─── Chargement du sprite (cache statique) ────────────────────────
QPixmap RubbleView::loadSprite()
{
    const int sz = static_cast<int>(Cfg::RUBBLE_SIZE);

    // Chemin à adapter selon l'organisation du fichier .qrc
    QPixmap pm(":/images/Textures/Enemy/Launcher/rubble.png");
    if (!pm.isNull())
        return pm.scaled(sz, sz, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // ── Fallback : carré gris-brun annoté ─────────────────────────
    pm = QPixmap(sz, sz);
    pm.fill(QColor(110, 90, 60));
    QPainter p(&pm);
    p.setPen(QColor(200, 170, 120));
    p.drawRect(1, 1, sz - 3, sz - 3);
    return pm;
}

// ─── Constructeur ─────────────────────────────────────────────────
RubbleView::RubbleView(RubbleModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
{
    static const QPixmap sprite = loadSprite(); // chargé une seule fois
    setPixmap(sprite);
    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_ENTITY);
}

// ─── Mise à jour ──────────────────────────────────────────────────
void RubbleView::updateView()
{
    if (!model || !model->isActive()) {
        hide();
        return;
    }
    setPos(model->getPosition().toQPointF());
}
