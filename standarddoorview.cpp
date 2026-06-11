#include "standarddoorview.h"

StandardDoorView::StandardDoorView(StandardDoorModel* m, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), itsModel(m), cachedOpen(false), wasLocked(false)
{
    setRect(0, 0,
            static_cast<double>(itsModel->getWidth()),
            static_cast<double>(itsModel->getHeight()));
    setPos(itsModel->getPosition().toQPointF());
    setZValue(Cfg::Z_WALL);
    buildPixmaps();
    updateView();
}

void StandardDoorView::buildPixmaps()
{
    int w = static_cast<int>(itsModel->getWidth());
    int h = static_cast<int>(itsModel->getHeight());

    QPixmap src(":/images/Textures/Doors/stddoor_locked.png");
    lockedPixmap = src.isNull() ? makeFallback(w, h, true)
                                : src.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QPixmap src2(":/images/Textures/Doors/stddoor_unlocked.png");
    unlockedPixmap = src2.isNull() ? makeFallback(w, h, false)
                                   : src2.scaled(w, h, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QPixmap StandardDoorView::makeFallback(int w, int h, bool isLocked)
{
    QPixmap pm(w, h);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    if (isLocked) {
        p.fillRect(0, 0, w, h, QColor(90, 55, 20));
        p.setPen(QPen(QColor(210, 180, 100), 2));
    } else {
        p.fillRect(0, 0, w, h, QColor(160, 120, 60, 60));
        p.setPen(QPen(QColor(210, 180, 100, 100), 1));
    }
    p.drawRect(1, 1, w - 2, h - 2);
    return pm;
}

void StandardDoorView::updateView()
{
    bool nowOpen = itsModel->isOpen();
    if (nowOpen == cachedOpen) return;
    cachedOpen = nowOpen;

    setBrush(nowOpen ? unlockedPixmap : lockedPixmap);
    setPen(Qt::NoPen);
    setZValue(nowOpen ? Cfg::Z_FLOOR : Cfg::Z_WALL);
}
