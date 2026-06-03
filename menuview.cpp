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
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "    stop:0 #4a90e2, stop:1 #1a5fa8);"
        "  color: white; font-size: 18px; font-weight: bold;"
        "  border-radius: 10px; border: 1px solid #2266aa;"
        "}"
        "QPushButton:hover { background: #5aaaf5; }"
        "QPushButton:pressed { background: #1a4e8a; }");

    connect(playButton, &QPushButton::clicked, this, &MenuView::sigGameRq);



    QPushButton *quitButton = new QPushButton("Quit", this);
    quitButton->setFixedSize(200, 42);
    quitButton->setStyleSheet(
        "QPushButton {"
        "  background: transparent;"
        "  color: #8899bb; font-size: 14px;"
        "  border: 1px solid #334466; border-radius: 8px;"
        "}"
        "QPushButton:hover { color: #ccddff; border-color: #6680aa; }");


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
