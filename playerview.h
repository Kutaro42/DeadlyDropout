#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "vector2f.h"
#include "config.h"

class PlayerView : public QGraphicsPixmapItem {
private:
    QPixmap idleSprites[4];
    QPixmap walkSprites[4];

    DirectionSprite currentDirection;
    bool isMoving;

    // Gestion du timing de l'animation de marche
    int currentFrame;
    float animationTimer;
    float frameDuration;
    int totalWalkFrames;

public:
    PlayerView(QGraphicsItem* parent = nullptr);

    void updateView(float dt, const Vector2f& velocity);
};

#endif // PLAYERVIEW_H