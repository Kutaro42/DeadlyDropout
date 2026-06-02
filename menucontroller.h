#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H



#include "menuview.h"

class MenuController : public QObject
{
    Q_OBJECT
private:
    MenuView *itsMenuView;

public:
    MenuController(QObject *parent = nullptr);
    ~MenuController();
signals:
    void sigGameRequest();
};

#endif // MENUCONTROLLER_H
