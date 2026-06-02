#include "appcontroller.h"

appController::appController(QWidget *parent)
    : QMainWindow(parent)
{
    AppState *currentState = new AppState;
    MenuController *itsMenuController = new MenuController;
    GameController *itsGameController = new GameController;


    //connect(GameView,);
}

appController::~appController()
{
    delete itsMenuController;
    delete currentState;
    delete itsGameController;

}


void appController::run()
{
    itsMenuController->render();
}
