#include "playerview.h"
#include <cmath>

PlayerView::PlayerView(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    currentDirection(DirectionSprite::DOWN),
    isMoving(false),
    currentFrame(0),
    animationTimer(0.0f),
    frameDuration(0.12f),     // 120ms par frame de marche
    totalWalkFrames(2)        // Ajustez selon le nombre de frames de vos sprites de marche
{
    // 1. Chargement de vos 4 fichiers IDLE individuels
    idleSprites[DirectionSprite::UP].load(":/resources/Textures/Player/Idle/Up.png");
    idleSprites[DirectionSprite::DOWN].load(":/ressources/Textures/Player/Idle/Down.png");
    idleSprites[DirectionSprite::LEFT].load(":/resources/Textures/Player/Idle/Left.png");
    idleSprites[DirectionSprite::RIGHT].load(":/resources/Textures/Player/Idle/Right.png");

    // 2. Chargement de vos sprites de MARCHE
    walkSprites[DirectionSprite::UP].load(":/resources/Textures/Player/Move/Up.png");
    walkSprites[DirectionSprite::DOWN].load(":/resources/Textures/Player/Move/Down.png");
    walkSprites[DirectionSprite::LEFT].load(":/resources/Textures/Player/Move/_Left.png");
    walkSprites[DirectionSprite::RIGHT].load(":/resources/Textures/walk_Right.png");

    // Texture initiale par défaut (Immobile vers le bas)
    setPixmap(idleSprites[currentDirection]);

    // Centrage de l'origine pour le positionnement (ex: si vos sprites font 64x64)
    setTransformOriginPoint(32, 32);
    setOffset(-32, -32); // Aligne le centre logique du joueur avec le centre du sprite
}

void PlayerView::updateView(float dt, const Vector2f& velocity) {
    // On considère que le joueur bouge si la longueur du vecteur vitesse est significative
    bool movingNow = (velocity.length() > 0.01f);

    // Détermination de la direction prédominante si le joueur avance
    if (movingNow) {
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            currentDirection = (velocity.x > 0) ? DirectionSprite::RIGHT : DirectionSprite::LEFT;
        } else {
            currentDirection = (velocity.y > 0) ? DirectionSprite::DOWN : DirectionSprite::UP;
        }
    }

    // Gestion des transitions d'état (remise à zéro de l'anim si changement)
    if (isMoving != movingNow) {
        isMoving = movingNow;
        currentFrame = 0;
        animationTimer = 0.0f;
    }

    // Choix du Pixmap ou découpage de la frame d'animation
    if (isMoving) {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer -= frameDuration;
            currentFrame = (currentFrame + 1) % totalWalkFrames;
        }

        QPixmap framePixmap = walkSprites[currentDirection].copy(currentFrame * 64, 0, 64, 64);
        setPixmap(framePixmap);
    } else {
        // État IDLE : applique directement l'un des 4 fichiers d'origine
        setPixmap(idleSprites[currentDirection]);
    }
}