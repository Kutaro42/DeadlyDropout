#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QMainWindow>
#include "config.h"
#include "menucontroller.h"
#include "gamecontroller.h"


class appController : public QMainWindow
{
    Q_OBJECT

private:
    AppState *currentState;
    MenuController *itsMenuController;
    GameController *itsGameController;

public:
    appController(QWidget *parent = nullptr);
    ~appController();

    void switchState(AppState* next);
    void startGame();

    signals:
    void sigGameRq();
};
#endif // APPCONTROLLER_H
