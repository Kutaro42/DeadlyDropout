/**
 * @file bossdoorview.cpp
 * @brief Implementation of the door's boss view.
 */

#include "Headers/bossdoorview.h"

BossDoorView::BossDoorView(BossDoorModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , itsModel(model)
    , wasLocked(false)
{
    // Create the visual now to a quicker access
    buildPixmaps();
    setPixmap(unlockedPixmap);
    setPos(model->getPosition().toQPointF());
    setZValue(Cfg::Z_WALL);
}

void BossDoorView::updateView()
{
    // Check if the door is lock or not and choose the right visual to show
    bool nowLocked = itsModel->isLocked();
    if (nowLocked != wasLocked) {
        setPixmap(nowLocked ? lockedPixmap : unlockedPixmap);
        wasLocked = nowLocked;
    }
}

void BossDoorView::buildPixmaps()
{
    // Get the door's size
    int w = static_cast<int>(itsModel->getWidth());
    int h = static_cast<int>(itsModel->getHeight());

    // Create the two visuals
    QPixmap src(":/images/Textures/Doors/boss_door_unlocked.png");
    unlockedPixmap = src.isNull() ? makeFallback(w, h, false)
                                  : src.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QPixmap src2(":/images/Textures/Doors/boss_door_locked.png");
    lockedPixmap = src2.isNull() ? makeFallback(w, h, true)
                                 : src2.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QPixmap BossDoorView::makeFallback(int w, int h, bool isLocked)
{
    //Draw the doors in case of an errors with the sprites files
    QPixmap pm(w, h);
    pm.fill(Qt::transparent);
    QPainter p(&pm);

    if (!isLocked) {
        // Unlocked door : golden border with "BOSS"
        p.fillRect(0, 0, w, h, QColor(80, 40, 0, 200));
        p.setPen(QPen(QColor(255, 180, 0), 3));
        p.drawRect(2, 2, w-4, h-4);
        p.setPen(QColor(255, 220, 80));
        p.setFont(QFont("Arial", 7, QFont::Bold));
        p.drawText(QRect(0,0,w,h), Qt::AlignCenter, "BOSS");
    } else {
        // Locked door : dark red with a white cross
        p.fillRect(0, 0, w, h, QColor(60, 0, 0, 220));
        p.setPen(QPen(QColor(180, 30, 30), 3));
        p.drawRect(2, 2, w-4, h-4);
        p.setPen(QPen(QColor(200, 40, 40), 2));
        p.drawLine(6, 6, w-6, h-6);
        p.drawLine(w-6, 6, 6, h-6);
    }
    return pm;
}
