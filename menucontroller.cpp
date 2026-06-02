#include "menucontroller.h"
#include "menuview.h"

MenuController::MenuController()
{
    MenuView *itsMenuView = new MenuView;
    int i =5;
}


MenuController::~MenuController()
{
    delete itsMenuView;
}


void MenuController::render()
{

    itsMenuView->show();
}
