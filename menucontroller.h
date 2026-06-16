/**
 * @file menucontroller.h
 * @brief Declaration of MenuController, which manages the main-menu logic.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "menuview.h"

/**
 * @class MenuController
 * @brief Controller responsible for handling interactions on the welcome screen.
 *
 * Sits between MenuView and AppController in the MVC chain.
 * Its only job is to relay the "Play" click upward as sigGameRequest so that
 * AppController — not the view — owns the decision to switch application state.
 * Keeping this relay in a dedicated controller means the view can be replaced
 * (e.g. a full-screen animated intro) without changing AppController.
 */
class MenuController : public QObject
{
    Q_OBJECT
private:
    MenuView *itsMenuView; ///< Pointer to the menu GUI; owned and deleted by this controller.

public:
    /**
     * @brief Constructs MenuController and connects MenuView signals.
     * @param parent Optional Qt parent object (nullptr by default).
     */
    MenuController(QObject *parent = nullptr);

    /**
     * @brief Destructor — frees the owned MenuView.
     */
    ~MenuController();

    /**
     * @brief Accessor for the managed MenuView instance.
     *
     * AppController calls this to embed the widget inside the main QStackedWidget
     * without having to create the view itself, preserving encapsulation.
     *
     * @return Pointer to the MenuView instance.
     */
    MenuView * getMenuView();

signals:
    /**
     * @brief Emitted when the player clicks "Play".
     *
     * AppController connects to this signal to trigger the MENU → PLAYING
     * state transition defined in the GDD state machine (Step 7).
     */
    void sigGameRequest();
};

#endif // MENUCONTROLLER_H