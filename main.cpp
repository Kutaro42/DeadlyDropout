/**
 * @file main.cpp
 * @brief Point d'entrée de l'application Deadly Dropout.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include <QApplication>
#include "appcontroller.h"


/**
 * @brief Fonction principale du programme.
 * * Crée l'application Qt, instancie le contrôleur principal (AppController) 
 * et lance la boucle d'exécution de Qt.
 * * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Tableau des arguments en ligne de commande.
 * @return Le code de sortie de l'application Qt.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Deadly Dropout");

    AppController appcontroller;
    appcontroller.run();
    return app.exec();
}



