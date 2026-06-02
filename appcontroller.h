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

    void run();
    void switchState(AppState* next);
    void handleEvents();

private slots:
    //void sltOnStartGame();
    //void sltOnQuitRequested();
};
#endif // APPCONTROLLER_H
