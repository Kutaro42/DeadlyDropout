#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "playermodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerView — représentation visuelle du joueur.
//
//  Spritesheets (prefix /images, ressources.qrc) :
//    Textures/Player/Idle/{Down,Up,Left,Right}.png   → 2 frames 32×32
//    Textures/Player/Move/{Down,Up,Left,Right}.png   → 2 frames 32×32
//    Textures/Player/Attack/{Down,Up,Left,Right}.png → 3 frames 32×32
//
//  Priorité d'animation : ATTACK > WALK > IDLE
// ═══════════════════════════════════════════════════════════════════
class PlayerView : public QGraphicsPixmapItem {
public:
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);
    void updateView(float dt);

private:
    PlayerModel* model;

    // États d'animation
    static constexpr int IDLE   = 0;
    static constexpr int WALK   = 1;
    static constexpr int ATTACK = 2;

    static constexpr int STATE_COUNT        = 3;
    static constexpr int DIR_COUNT          = 4;
    static constexpr int FRAME_W            = 32;
    static constexpr int FRAME_H            = 32;
    static constexpr int ATTACK_FRAME_COUNT = 3;    // sheet 96×32

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];

    int   currentFrame;
    float frameTimer;
    float animSpeed;        // durée par frame walk/idle (s)
    float attackAnimSpeed;  // durée par frame attack (s) = KATANA_ANIM_DUR / 3

    static int  dirIdx(FacingDirection d);
    void        loadSheet(QVector<QPixmap>& frames, const QString& path,
                          int fw, int fh);
    QPixmap     makeFallback(FacingDirection dir);
    void        advanceFrames(float dt, const QVector<QPixmap>& frames,
                              float speed);
};

#endif // PLAYERVIEW_H
