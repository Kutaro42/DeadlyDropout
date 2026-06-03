#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "playermodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerView — représentation visuelle du joueur.
//  3 états × 4 directions = 12 jeux de frames.
//  Fallback : carré bleu annoté "P" si sprites absents.
// ═══════════════════════════════════════════════════════════════════
class PlayerView : public QGraphicsPixmapItem {
private:
    PlayerModel*     model;

    // sprites[état][direction] — état : 0=idle 1=walk 2=slash
    //                         — dir  : 0=down 1=up 2=left 3=right
    QVector<QPixmap> sprites[3][4];

    int   currentFrame;
    float frameTimer;
    float animSpeed;
    bool  prevAttacking; // pour détecter le début d'une nouvelle attaque

    static int dirIdx(FacingDirection d);

    void loadSheet(QVector<QPixmap>& frames,
                   const QString& path, int fw, int fh);
    void advanceFrames(float dt, const QVector<QPixmap>& frames);
    void showFallback(FacingDirection dir);

public:
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);

    // À appeler chaque tick depuis GameView
    void updateView(float dt);
};

#endif // PLAYERVIEW_H
