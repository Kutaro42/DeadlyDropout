#ifndef AppController_H
#define AppController_H

#include <QMainWindow>
#include <QStackedWidget>
#include "config.h"
#include "menucontroller.h"
#include "gamecontroller.h"


class AppController : public QMainWindow
{
    Q_OBJECT

private:
    AppState *currentState;
    QStackedWidget *mainContainer;
    MenuController *itsMenuController;
    GameController *itsGameController;

public:
    AppController(QWidget *parent = nullptr);
    ~AppController();

    void switchState(AppState* next);
    void startGame();
    void run();

    signals:
    void sigGameRq();
};
#endif // AppController_H
