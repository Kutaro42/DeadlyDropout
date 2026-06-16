/**
 * @file main.cpp
 * @brief Entry point of the Deadly Dropout application.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#include <QApplication>
#include "appcontroller.h"

/**
 * @brief Main function of the program.
 *
 * Creates the Qt application, instantiates the top-level controller (AppController)
 * and starts the Qt event loop.
 *
 * AppController is used rather than directly instantiating GameController or MenuController
 * so that the application-level state machine (MENU → PLAYING → GAME_OVER …) lives in
 * one place and neither the menu nor the game need to know about each other.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return The Qt application exit code.
 */
int main(int argc, char* argv[]) {
    qputenv("QT_LOGGING_RULES", "qt.multimedia.*=false");
    QApplication app(argc, argv);
    app.setApplicationName("Deadly Dropout");

    AppController appcontroller;
    appcontroller.run();
    return app.exec();
}