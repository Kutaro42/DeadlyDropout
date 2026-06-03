#ifndef WALLVIEW_H
#define WALLVIEW_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include "wallmodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  WallView — représentation visuelle d'un WallModel.
//  Hérite QGraphicsRectItem (item statique dans la scène Qt).
// ═══════════════════════════════════════════════════════════════════
class WallView : public QGraphicsRectItem {
public:
    explicit WallView(WallModel* model, QGraphicsItem* parent = nullptr);
};

#endif // WALLVIEW_H
