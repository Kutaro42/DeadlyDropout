#include "appcontroller.h"
#include <iostream>

appController::appController(QWidget *parent)
    : QMainWindow(parent)
{
    currentState = new AppState;
    itsMenuController = new MenuController(this);
    itsGameController = new GameController;


    connect(itsMenuController,&MenuController::sigGameRequest,this,&appController::sigGameRq);
    connect(this,&appController::sigGameRq,this,&appController::startGame);
}

appController::~appController()
{
    delete itsMenuController;
    delete currentState;
    delete itsGameController;
}

void appController::startGame()
{
    std::cout << "test";
}
