/**
 * @file appcontroller.h
 * @brief Déclaration du contrôleur principal de l'application orchestrant les scènes (MVC).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef AppController_H
#define AppController_H

#include <QMainWindow>
#include <QStackedWidget>
#include "config.h"
#include "menucontroller.h"
#include "gamecontroller.h"
#include "marketcontroller.h"

/**
 * @class AppController
 * @brief Routeur global contrôlant l'empilement des interfaces et l'état général de la partie.
 */
class AppController : public QMainWindow
{
    Q_OBJECT

private:
    AppState *currentState;            ///< Pointeur vers l'état applicatif courant.
    QStackedWidget *mainContainer;     ///< Conteneur Qt permettant le basculement entre vues (Menu / Jeu).
    MenuController *itsMenuController; ///< Contrôleur dédié au menu principal.
    GameController *itsGameController; ///< Contrôleur dédié à la gestion in-game.
    MarketController* itsMarketController; ///< Controller dédié au market (logique métier et vue)

public:
    /**
     * @brief Constructeur principal.
     */
    AppController(QWidget *parent = nullptr);
    ~AppController();

    /**
     * @brief Demande le passage à un nouvel état de l'application.
     * @param next: Pointeur vers le futur état.
     */
    void switchState(AppState* next);
    
    /**
     * @brief Déclenche l'initialisation de la partie et passe la vue sur le moteur de jeu.
     */
    void startGame();
    
    /**
     * @brief Affiche la fenêtre globale et maintient la boucle applicative active.
     */
    void run();

    /**
     * @brief Initialise une nouvelle partit en supprimant puis recréant un gamecontroller.
     */
    void reset();

signals:
    /**
     * @brief Signal émis en interne pour signifier la volonté de démarrer une partie.
     */
    void sigGameRq();

private slots:
    /**
     * @brief Slots qui relance ou quite la partie en fin de partit (Game Over ou Victory).
     */
    void sltRestart();

    /**
     * @brief Slot pour faire revenir au menu (possibilité de relancer une partie ou de quitter l'application.
     */
    void sltBackMenu();

    /**
     * @brief Slots qui lance l'ouverture de l'overlay du market.
     */
    void sltOpenMarket();
};
#endif // AppController_H
