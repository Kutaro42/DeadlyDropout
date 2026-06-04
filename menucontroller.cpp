/**
 * @file menucontroller.cpp
 * @brief Implémentation du contrôleur gérant le menu d'accueil.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "menucontroller.h"
#include "menuview.h"

/**
 * @brief Constructeur du MenuController.
 * * Initialise la vue du menu et connecte les signaux de l'interface graphique 
 * à la logique du contrôleur.
 * * @param parent Objet parent Qt optionnel.
 */
MenuController::MenuController(QObject* parent) : QObject(parent)
{
    itsMenuView = new MenuView;
    connect(itsMenuView, &MenuView::sigGameRq, this, &MenuController::sigGameRequest);
}

/**
 * @brief Destructeur du MenuController.
 * Libère la mémoire allouée pour la vue du menu.
 */
MenuController::~MenuController()
{
    delete itsMenuView;
}

/**
 * @brief Accesseur de la vue du menu.
 * @return Un pointeur vers l'instance de MenuView.
 */
MenuView* MenuController::getMenuView()
{
    return itsMenuView;
}