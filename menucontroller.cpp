#include "menucontroller.h"
#include "menuview.h"


MenuController::MenuController(QObject* parent) : QObject(parent)
{
    itsMenuView = new MenuView;


    connect(itsMenuView, &MenuView::sigGameRq,this,&MenuController::sigGameRequest);
}


MenuController::~MenuController()
{
    delete itsMenuView;
}

MenuView * MenuController::getMenuView()
{
    return itsMenuView;
}
