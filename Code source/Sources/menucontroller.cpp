/**
 * @file menucontroller.cpp
 * @brief Implementation of the main-menu controller.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#include "menucontroller.h"
#include "menuview.h"

/**
 * @brief Constructs MenuController.
 *
 * Allocates the view and wires its sigGameRq signal to the local sigGameRequest
 * signal so the caller (AppController) only has to know about one signal type,
 * decoupling it from MenuView's internal naming.
 *
 * @param parent Optional Qt parent object.
 */
MenuController::MenuController(QObject* parent) : QObject(parent)
{
    itsMenuView = new MenuView;
    connect(itsMenuView, &MenuView::sigGameRq, this, &MenuController::sigGameRequest);
}

/**
 * @brief Destructor — releases the owned MenuView.
 */
MenuController::~MenuController()
{
    delete itsMenuView;
}

/**
 * @brief Returns a pointer to the managed MenuView.
 * @return Pointer to the MenuView instance.
 */
MenuView* MenuController::getMenuView()
{
    return itsMenuView;
}