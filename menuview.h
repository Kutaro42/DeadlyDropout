/**
 * @file menuview.h
 * @brief Déclaration de la classe MenuView représentant l'interface graphique du menu.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef MENUVIEW_H
#define MENUVIEW_H
#include <QWidget>

/**
 * @class MenuView
 * @brief Vue gérant l'affichage et les clics de l'écran principal.
 * * Affiche l'arrière-plan, le logo et les boutons Jouer/Quitter.
 */
class MenuView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de MenuView.
     * @param parent Pointeur vers le widget parent (nullptr par défaut).
     */
    MenuView(QWidget *parent = nullptr);
    
    /**
     * @brief Surcharge de l'événement de dessin de Qt (paintEvent).
     * * Gère le rendu de l'image de fond et du logo du jeu via QPainter.
     * @param event L'événement de peinture déclenché par l'interface.
     */
    void paintEvent(QPaintEvent* event);

signals:
    /**
     * @brief Signal émis lors d'un clic sur le bouton de lancement du jeu.
     */
    void sigGameRq();

};

#endif // MENUVIEW_H