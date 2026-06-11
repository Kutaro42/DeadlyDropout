/**
 * @file standarddoorview.cpp
 * @brief Implémentation de la vue graphique pour la porte standard.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "standarddoorview.h"

StandardDoorView::StandardDoorView(StandardDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), model(m), cachedOpen(false)
{
    setRect(0, 0,
            static_cast<double>(model->getWidth()),
            static_cast<double>(model->getHeight()));
    setPos(model->getPosition().toQPointF());

    // Fermée : marron sombre avec contour crème
    setBrush(QBrush(QColor(90, 55, 20)));
    setPen(QPen(QColor(210, 180, 100), 2));
    setZValue(Cfg::Z_WALL); // même plan que les murs tant que fermée
}

void StandardDoorView::updateView()
{
    if (!model) return;
    bool nowOpen = model->isOpen();
    if (nowOpen == cachedOpen) return; // pas de changement → aucun redraw

    cachedOpen = nowOpen;
    if (nowOpen) {
        // Ouverte : rendu quasi-transparent (sol visible), contour discret
        setBrush(QBrush(QColor(160, 120, 60, 60)));
        setPen(QPen(QColor(210, 180, 100, 100), 1));
        setZValue(Cfg::Z_FLOOR); // passe en dessous des entités
    } else {
        setBrush(QBrush(QColor(90, 55, 20)));
        setPen(QPen(QColor(210, 180, 100), 2));
        setZValue(Cfg::Z_WALL);
    }
}
/*
void StandardDoorView::buildPixmaps()
{
    int w = static_cast<int>(itsModel->getWidth());
    int h = static_cast<int>(itsModel->getHeight());

    QPixmap src(":/images/Textures/Doors/stddoor_unlocked.png");
    unlockedPixmap = src.isNull() ? makeFallback(w, h, false)
                                  : src.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QPixmap src2(":/images/Textures/Doors/stddoor_locked.png");
    lockedPixmap = src2.isNull() ? makeFallback(w, h, true)
                                 : src2.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QPixmap StandardDoorView::makeFallback(int w, int h, bool isLocked)
{
    QPixmap pm(w, h);
    pm.fill(Qt::transparent);
    QPainter p(&pm);

    if (!isLocked) {
        // Porte franchissable : encadré doré avec "BOSS"
        p.fillRect(0, 0, w, h, QColor(80, 40, 0, 200));
        p.setPen(QPen(QColor(255, 180, 0), 3));
        p.drawRect(2, 2, w-4, h-4);
        p.setPen(QColor(255, 220, 80));
        p.setFont(QFont("Arial", 7, QFont::Bold));
        p.drawText(QRect(0,0,w,h), Qt::AlignCenter, "BOSS");
    } else {
        // Porte verrouillée : rouge sombre avec croix
        p.fillRect(0, 0, w, h, QColor(60, 0, 0, 220));
        p.setPen(QPen(QColor(180, 30, 30), 3));
        p.drawRect(2, 2, w-4, h-4);
        p.setPen(QPen(QColor(200, 40, 40), 2));
        p.drawLine(6, 6, w-6, h-6);
        p.drawLine(w-6, 6, 6, h-6);
    }
    return pm;
}*/

