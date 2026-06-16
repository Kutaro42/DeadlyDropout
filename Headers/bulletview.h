#ifndef BULLETVIEW_H
#define BULLETVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "bulletmodel.h"
#include "config.h"

class BulletView : public QGraphicsPixmapItem
{
public:
    explicit BulletView(BulletModel* model, QGraphicsItem* parent = nullptr);
    void updateView();

private:
    BulletModel* model;
    static QPixmap loadSprite();
};

#endif // BULLETVIEW_H
