#ifndef AppController_H
#define AppController_H

#include <QMainWindow>
#include "config.h"
#include "menucontroller.h"
#include "gamecontroller.h"


class AppController : public QMainWindow
{
    Q_OBJECT

private:
    AppState *currentState;
    MenuController *itsMenuController;
    GameController *itsGameController;

public:
    AppController(QWidget *parent = nullptr);
    ~AppController();

    void switchState(AppState* next);
    void startGame();

    signals:
    void sigGameRq();
};
#endif // AppController_H
