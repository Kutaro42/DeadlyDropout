#ifndef ZOMBIESTDVIEW_H
#define ZOMBIESTDVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "zombiestdmodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdView — représentation visuelle du zombie standard.
//  Hérite QGraphicsPixmapItem.
//  Supporte le chargement de spritesheet (N frames × 1 ligne).
//  Fallback : rectangle coloré si le fichier PNG est absent.
// ═══════════════════════════════════════════════════════════════════
class ZombieStdView : public QGraphicsPixmapItem {
private:
    ZombieStdModel* model;
    QVector<QPixmap> frames;
    int              currentFrame;
    float            frameTimer;
    float            animSpeed;    // secondes par frame

public:
    explicit ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent = nullptr);

    // Charge une spritesheet et découpe les frames automatiquement
    void loadSpriteSheet(const QString& filePath, int frameW, int frameH);

    // À appeler chaque tick pour synchro position + animation
    void updateView(float dt);

private:
    void setFallbackPixmap();
};

#endif // ZOMBIESTDVIEW_H
