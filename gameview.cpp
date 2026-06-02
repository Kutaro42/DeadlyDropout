#include "gameview.h"

GameView::GameView(QWidget* parent) : QGraphicsView(parent), inputDirection(0, 0) {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);
    
    setFixedSize(800, 600);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameView::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Z: case Qt::Key_Up:    inputDirection.y = -1.0f; break;
        case Qt::Key_S: case Qt::Key_Down:  inputDirection.y = 1.0f;  break;
        case Qt::Key_Q: case Qt::Key_Left:  inputDirection.x = -1.0f; break;
        case Qt::Key_D: case Qt::Key_Right: inputDirection.x = 1.0f;  break;
        default: QGraphicsView::keyPressEvent(event); break;
    }
}

void GameView::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Z: case Qt::Key_Up:    if (inputDirection.y < 0) inputDirection.y = 0.0f; break;
        case Qt::Key_S: case Qt::Key_Down:  if (inputDirection.y > 0) inputDirection.y = 0.0f; break;
        case Qt::Key_Q: case Qt::Key_Left:  if (inputDirection.x < 0) inputDirection.x = 0.0f; break;
        case Qt::Key_D: case Qt::Key_Right: if (inputDirection.x > 0) inputDirection.x = 0.0f; break;
        default: QGraphicsView::keyReleaseEvent(event); break;
    }
}