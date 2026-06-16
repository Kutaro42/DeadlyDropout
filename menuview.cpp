/**
 * @file menuview.cpp
 * @brief Implementation of MenuView, the main-menu screen.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
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
    setFixedSize(Cfg::WINDOW_W, Cfg::WINDOW_H);

    // --- Play button ---------------------------------------------------
    // Styled with a dark-red gradient to match the post-apocalyptic palette
    // defined in the GDD (warm, muted tones).
    QPushButton *playButton = new QPushButton("Play", this);
    playButton->setFixedSize(200, 52);
    playButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #8b0000, stop:1 #4a0000);"
        "  color: #ffffff;"
        "  font-size: 18px; font-weight: bold;"
        "  border-radius: 6px;"
        "  border: 2px solid #ffffff;"
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

    connect(playButton, &QPushButton::clicked, this, &MenuView::sigGameRq);

    // --- Quit button ---------------------------------------------------
    QPushButton *quitButton = new QPushButton("Quit", this);
    quitButton->setFixedSize(200, 42);
    quitButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #8b0000, stop:1 #4a0000);"
        "  color: #ffffff;"
        "  font-size: 14px; font-weight: bold;"
        "  border-radius: 6px;"
        "  border: 2px solid #ffffff;"
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

    painter.drawImage(200, 0, imgFond); // background image
    painter.drawImage(
        QRect(225, 0, 518, 200), // desired size
        logo);

    setWindowIcon(QIcon(":/images/Textures/Logo/favi.png")); // Show logo top-left
}