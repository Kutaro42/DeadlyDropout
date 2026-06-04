#ifndef ZOMBIESTDVIEW_H
#define ZOMBIESTDVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "zombiestdmodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdView — représentation visuelle du zombie standard.
//
//  Spritesheets (prefix /images, ressources.qrc) :
//    Textures/Enemy/Standard/Idle/{Down,Up,Left,Right}.png → 2 frames 32×32
//    Textures/Enemy/Standard/Move/{Down,Up,Left,Right}.png → 2 frames 32×32
//
//  Direction déduite du vecteur velocity du modèle.
//  Fallback : rectangle vert annoté "Z" si sprite absent.
// ═══════════════════════════════════════════════════════════════════
class ZombieStdView : public QGraphicsPixmapItem {
public:
    explicit ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent = nullptr);
    void updateView(float dt);

private:
    ZombieStdModel* model;

    static constexpr int IDLE  = 0;
    static constexpr int WALK  = 1;
    static constexpr int STATE_COUNT = 2;
    static constexpr int DIR_COUNT   = 4;   // DOWN UP LEFT RIGHT
    static constexpr int FRAME_W     = 32;
    static constexpr int FRAME_H     = 32;

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];

    int   currentFrame;
    float frameTimer;
    float animSpeed;        // secondes par frame walk
    int   currentDir;       // index direction courante

    static int  dirIdx(FacingDirection d);
    static FacingDirection velocityToDir(Vector2f vel);
    void        loadSheet(QVector<QPixmap>& frames, const QString& path);
    QPixmap     makeFallback();
    void        advanceFrames(float dt, const QVector<QPixmap>& frames);
};

#endif // ZOMBIESTDVIEW_H
