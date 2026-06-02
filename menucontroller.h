#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "menuview.h"

class MenuController
{
private:
    MenuView *itsMenuView;

public:
    MenuController();
    ~MenuController();
    void handleInput();
    void render();
};

#endif // MENUCONTROLLER_H
