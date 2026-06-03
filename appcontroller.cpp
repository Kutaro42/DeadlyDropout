/*#include "AppController.h"
#include <iostream>

AppController::AppController(QWidget *parent)
    : QMainWindow(parent)
{
    currentState = new AppState;
    mainContainer = new QStackedWidget();

    //création du controller menu et du game
    itsMenuController = new MenuController(this);
    itsGameController = new GameController(this);

    //ajout des vue au container :
    mainContainer->addWidget(itsMenuController->getMenuView().parentWidget());
    mainContainer->addWidget(itsGameController->getView()->parentWidget());

    mainContainer->setCurrentIndex(0);
    mainContainer->setFixedSize(Cfg::WINDOW_H, Cfg::WINDOW_W);
    mainContainer->show();


    connect(itsMenuController,&MenuController::sigGameRequest,this,&AppController::sigGameRq);
    connect(this,&AppController::sigGameRq,this,&AppController::startGame);
}

AppController::~AppController()
{
    delete itsMenuController;
    delete currentState;
    delete itsGameController;
}

void AppController::startGame()
{
    //std::cout << "test";
    itsGameController->init();

}*/

#include "AppController.h"
#include <iostream>

AppController::AppController(QWidget *parent)
    : QMainWindow(parent)
{
    currentState = new AppState;

    // On passe 'this' en paramètre pour lier le container à la QMainWindow
    mainContainer = new QStackedWidget(this);

    // Création des contrôleurs
    itsMenuController = new MenuController();
    itsGameController = new GameController(this);

    //on construit le menu controller en prévision pour obtenir sa vue
    //itsGameController->init();


    //  On ajoute directement les VUES (et non leur parentWidget qui vaut nullptr)

    mainContainer->addWidget((itsMenuController->getMenuView()));
    mainContainer->addWidget(itsGameController->getView());

    // On intègre le QStackedWidget au cœur de la QMainWindow
    setCentralWidget(mainContainer);

    mainContainer->setCurrentIndex(0); // On commence sur le menu (index 0)

    // setFixedSize prend (Largeur, Hauteur). Attention à ne pas inverser W et H !
    setFixedSize(Cfg::WINDOW_W, Cfg::WINDOW_H);

    // Connexions des signaux
    connect(itsMenuController, &MenuController::sigGameRequest, this, &AppController::sigGameRq);
    connect(this, &AppController::sigGameRq, this, &AppController::startGame);
}

AppController::~AppController()
{
    // Pas besoin de delete mainContainer si vous l'avez lié à 'this', Qt s'en occupe.
    delete itsMenuController;
    delete currentState;
    delete itsGameController;
}

void AppController::startGame()
{
    itsGameController->init();

    // CORRECTION 4 : On bascule l'affichage sur le jeu (index 1) !
    mainContainer->setCurrentIndex(1);

    // On donne le focus au jeu pour que les touches du clavier (ZQSD) fonctionnent directement
    if(itsGameController->getView()) {
        itsGameController->getView()->setFocus();
    }
}
void AppController::run() {
    this->show(); // C'est cette ligne qui maintient la fenêtre ouverte !
}
