/**
 * @file menuview.h
 * @brief Declaration of MenuView, the graphical interface of the main menu screen.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef MENUVIEW_H
#define MENUVIEW_H
#include <QWidget>

/**
 * @class MenuView
 * @brief View responsible for rendering and handling clicks on the main screen.
 *
 * Kept intentionally thin: it owns no business logic and emits a signal when
 * the player clicks "Play", delegating the decision of what to do next entirely
 * to MenuController.  This separation means the visual layout can be redesigned
 * without touching any game-state code.
 *
 * Renders the background image, the game logo and the Play/Quit buttons.
 */
class MenuView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs MenuView and wires the Play/Quit buttons.
     * @param parent Pointer to the parent widget (nullptr by default).
     */
    MenuView(QWidget *parent = nullptr);

    /**
     * @brief Qt paint-event override used to draw the background and logo.
     *
     * Drawing is done here rather than with QLabel pixmaps so that the logo
     * and background can be precisely composited at the exact pixel positions
     * defined by the GDD wireframe.
     *
     * @param event The paint event triggered by the Qt framework.
     */
    void paintEvent(QPaintEvent* event);

signals:
    /**
     * @brief Emitted when the player clicks the Play button.
     *
     * MenuController listens to this signal and re-emits sigGameRequest upward
     * so that AppController can perform the MENU → PLAYING transition.
     */
    void sigGameRq();
};

#endif // MENUVIEW_H