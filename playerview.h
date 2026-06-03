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
//  Spritesheets attendus (via ressources.qrc, prefix /images) :
//    Textures/Player/Idle/{Down,Up,Left,Right}.png  → 2 frames 32×32 (64×32)
//    Textures/Player/Move/{Down,Up,Left,Right}.png  → 2 frames 32×32 (64×32)
//
//  Chaque frame est extraite et scalée à Cfg::ENTITY_SIZE (64×64).
//  Fallback : carré bleu annoté si sprite absent.
// ═══════════════════════════════════════════════════════════════════
class PlayerView : public QGraphicsPixmapItem {
private:
    PlayerModel* model;

    // sprites[état][direction] — état : 0=idle  1=walk 3=attack
    //                          — dir  : 0=DOWN 1=UP 2=LEFT 3=RIGHT
    static constexpr int STATE_COUNT = 3;
    static constexpr int DIR_COUNT   = 4;

    // Dimensions source des frames dans le spritesheet
    static constexpr int FRAME_W = 32;   // largeur d'une frame source (px)
    static constexpr int FRAME_H = 32;   // hauteur d'une frame source (px)

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];

    int   currentFrame;
    float frameTimer;
    float animSpeed;    // durée d'une frame walk (s)

    // Helpers
    static int     dirIdx(FacingDirection d);
    void           loadSheet(QVector<QPixmap>& frames, const QString& path);
    void           advanceFrames(float dt, const QVector<QPixmap>& frames);
    void           showFallback(FacingDirection dir);

public:
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);

    // Appelé chaque tick depuis GameView
    void updateView(float dt);
};

#endif // PLAYERVIEW_H
