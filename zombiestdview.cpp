#include "zombiestdview.h"

ZombieStdView::ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), model(model), currentFrameIndex(0), frameTimer(0.0f), animationSpeed(0.2f) {
    
    // Le GDD indique des fichiers de 32x64px contenant 2 frames de 32x32px
    loadSpriteSheet(":/assets/zombie_walk.png", 32, 32); 
}

void ZombieStdView::loadSpriteSheet(const QString& filePath, int frameWidth, int frameHeight) {
    QPixmap spriteSheet(filePath);
    if (spriteSheet.isNull()) return;

    frames.clear();
    // Découpage automatique de la spritesheet basé sur les dimensions
    int cols = spriteSheet.width() / frameWidth;
    int rows = spriteSheet.height() / frameHeight;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            frames.append(spriteSheet.copy(x * frameWidth, y * frameHeight, frameWidth, frameHeight));
        }
    }
    
    if (!frames.isEmpty()) {
        // Met à l'échelle à 64x64px comme demandé dans le GDD
        setPixmap(frames[0].scaled(64, 64)); 
    }
}

void ZombieStdView::updateView(float dt) {
    if (!model || !model->isActive()) {
        hide(); // Cache le sprite si le zombie est mort
        return;
    }

    // Mise à jour de la position depuis le modèle vers la scène Qt
    setPos(model->getPosition().toQPointF());

    // Animation basique si le zombie bouge
    frameTimer += dt;
    if (frameTimer >= animationSpeed && !frames.isEmpty()) {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        setPixmap(frames[currentFrameIndex].scaled(64, 64));
        frameTimer = 0.0f;
    }
}