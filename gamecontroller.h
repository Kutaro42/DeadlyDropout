#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "gameview.h"
#include "gamemodel.h"

class GameController
{

private:
    GameView *itsGameView;
    GameModel *itsGameModel;

public:
    GameController();
    ~GameController();
    void init();
};

#endif // GAMECONTROLLER_H
