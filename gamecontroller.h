#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include "gameview.h"
#include "gamemodel.h"

class GameController : public QObject
{
    Q_OBJECT

private:
    GameView *itsGameView;
    GameModel *itsGameModel;
    QTimer *gameTimer;

private slots:
    // Boucle de mise à jour logique et graphique (~60 FPS)
    void gameLoop();

public:
    explicit GameController(QObject *parent = nullptr);
    ~GameController();

    // Initialisation de la vue, du modèle et démarrage du Timer
    void init();
};

#endif // GAMECONTROLLER_H
