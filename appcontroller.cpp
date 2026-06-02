#include "AppController.h"
#include <iostream>

AppController::AppController(QWidget *parent)
    : QMainWindow(parent)
{
    currentState = new AppState;
    itsMenuController = new MenuController(this);
    itsGameController = new GameController;


    connect(itsMenuController,&MenuController::sigGameRequest,this,&AppController::sigGameRq);
    connect(this,&AppController::sigGameRq,this,&AppController::startGame);
}

AppController::~AppController()
{
    delete itsMenuController;
    delete currentState;
    delete itsGameController;
}

void AppController::startGame()
{
    std::cout << "test";
}
