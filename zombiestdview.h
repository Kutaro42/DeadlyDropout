#ifndef ZOMBIESTDVIEW_H
#define ZOMBIESTDVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QVector>
#include "zombiestdmodel.h"

class ZombieStdView : public QGraphicsPixmapItem {
private:
    ZombieStdModel* model;
    QVector<QPixmap> frames;
    int currentFrameIndex;
    float frameTimer;
    float animationSpeed; // Temps entre chaque frame

public:
    ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent = nullptr);
    
    // Code commun pour charger les spritesheets de n'importe quelle dimension
    void loadSpriteSheet(const QString& filePath, int frameWidth, int frameHeight);
    
    void updateView(float dt);
};

#endif // ZOMBIESTDVIEW_H