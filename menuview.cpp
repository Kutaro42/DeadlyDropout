/**
 * @file menuview.cpp
 * @brief Implémentation de la vue MenuView pour l'écran de démarrage.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "menuview.h"
#include "config.h"
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>

MenuView::MenuView(QWidget* parent) : QWidget(parent)
{
    // Windows size
    setFixedSize(Cfg::WINDOW_W,Cfg::WINDOW_H);

    // Buttons
    QPushButton *playButton = new QPushButton("Play", this);
    playButton->setFixedSize(200, 52);
    playButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #8b0000, stop:1 #4a0000);" // Dégradé rouge sombre
        "  color: #ffffff;"
        "  font-size: 18px; font-weight: bold;"
        "  border-radius: 6px;"
        "  border: 2px solid #ffffff;"         // Bordure blanche nette
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #a80000, stop:1 #660000);" // Rouge plus vif au survol
        "  border-color: #ffcccc;"            // Blanc incandécent au survol
        "}"
        "QPushButton:pressed {"
        "  background: #2b0000;"              // Sang très sombre au clic
        "  border-color: #aaaaaa;"            // Bordure blanche grisée au clic
        "}");

    connect(playButton, &QPushButton::clicked, this, &MenuView::sigGameRq);

    QPushButton *quitButton = new QPushButton("Quit", this);
    quitButton->setFixedSize(200, 42);
    quitButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #8b0000, stop:1 #4a0000);" // Même dégradé rouge
        "  color: #ffffff;"
        "  font-size: 14px; font-weight: bold;" // Police un peu plus petite pour le Quit
        "  border-radius: 6px;"
        "  border: 2px solid #ffffff;"         // Même bordure blanche
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #a80000, stop:1 #660000);"
        "  border-color: #ffcccc;"
        "}"
        "QPushButton:pressed {"
        "  background: #2b0000;"
        "  border-color: #aaaaaa;"
        "}");

    connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);

    QVBoxLayout *itsLayout = new QVBoxLayout(this);
    itsLayout->addWidget(playButton);
    itsLayout->addWidget(quitButton);

    itsLayout->setAlignment(Qt::AlignCenter);
}

void MenuView::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QImage logo(":/images/Textures/Logo/logopixel.png");
    QImage imgFond(":/images/Textures/bata.png");

    painter.drawImage(200, 0, imgFond); // image fond
    painter.drawImage(
        QRect(225, 0, 518, 200), // taille souhaitée
        logo);

    setWindowIcon(QIcon(":/images/Textures/Logo/favi.png")); // Avoir logo en haut a gauche
}
