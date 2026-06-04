/**
 * @file menucontroller.h
 * @brief Déclaration de la classe MenuController gérant la logique du menu principal.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "menuview.h"

/**
 * @class MenuController
 * @brief Contrôleur responsable de la gestion des interactions sur l'écran d'accueil.
 * * Orchestre les signaux émis par la vue du menu (MenuView) pour déclencher le début de la partie 
 * ou la fermeture de l'application.
 */
class MenuController : public QObject
{
    Q_OBJECT
private:
    MenuView *itsMenuView; ///< Pointeur vers l'interface graphique du menu.

public:
    /**
     * @brief Constructeur de la classe MenuController.
     * @param parent Pointeur vers le QObject parent (nullptr par défaut).
     */
    MenuController(QObject *parent = nullptr);
    
    /**
     * @brief Destructeur de la classe MenuController.
     */
    ~MenuController();

    /**
     * @brief Permet d'accéder à la vue du menu associée à ce contrôleur.
     * @return Pointeur vers l'instance de MenuView.
     */
    MenuView * getMenuView();

signals:
    /**
     * @brief Signal émis lorsque le joueur clique sur le bouton "Jouer" pour lancer la partie.
     */
    void sigGameRequest();
};

#endif // MENUCONTROLLER_H