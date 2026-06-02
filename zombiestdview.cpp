#include "zombiestdview.h"

ZombieStdView::ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), model(model), currentFrameIndex(0), frameTimer(0.0f), animationSpeed(0.15f) {
    
    // Exemple d'appel : charge un sprite et découpe automatiquement les frames de 32x32px
    loadSpriteSheet(":/assets/zombie_walk.png", 32, 32);
}

void ZombieStdView::loadSpriteSheet(const QString& filePath, int frameWidth, int frameHeight) {
    QPixmap spriteSheet(filePath);
    
    // Si l'image PNG n'est pas trouvée, on crée une texture de remplacement visible (Carré Vert)
    if (spriteSheet.isNull()) {
        QPixmap fallback(frameWidth, frameHeight);
        fallback.fill(Qt::green);
        frames.append(fallback);
    } else {
        int cols = spriteSheet.width() / frameWidth;
        int rows = spriteSheet.height() / frameHeight;
        
        // Découpage dynamique de n'importe quelle dimension (ex: 32x64 ou 32x96)
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                frames.append(spriteSheet.copy(x * frameWidth, y * frameHeight, frameWidth, frameHeight));
            }
        }
    }

    if (!frames.isEmpty()) {
        setPixmap(frames[0].scaled(64, 64)); // Mise à l'échelle finale
    }
}

void ZombieStdView::updateView(float dt) {
    if (!model || !model->isActive()) {
        hide();
        return;
    }

    // Synchronisation de la position de la vue avec le modèle
    setPos(model->getPosition().toQPointF());

    // Cycle d'animation
    frameTimer += dt;
    if (frameTimer >= animationSpeed && !frames.isEmpty()) {
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
        setPixmap(frames[currentFrameIndex].scaled(64, 64));
        frameTimer = 0.0f;
    }
}