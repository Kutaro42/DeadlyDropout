/**
 * @file gameview.cpp
 * @brief Implémentation du moteur de rendu 2D et de la captation des inputs.
 */

#include "gameview.h"
#include <QFont>
#include <QFontMetrics>

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      playerView(nullptr),
      currentModel(nullptr),
      showingOverlay(false)
{
    setScene(scene);
    setFixedSize(Cfg::WINDOW_W, Cfg::WINDOW_H);

    // Désactive les scrollbars natives, car la caméra (followPlayer) est gérée par code.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configuration volontaire d'un rendu "pixel-perfect" sans flou pour l'esthétique rétro.
    setRenderHint(QPainter::Antialiasing,       false);
    setRenderHint(QPainter::SmoothPixmapTransform, false);

    setBackgroundBrush(QBrush(QColor(15, 12, 8))); // fond noir-brun
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Deadly Dropout");
}

GameView::~GameView() {}

void GameView::initWithModel(GameModel* model) {
    currentModel = model;
    if (!model || !model->getPlayer()) return;

    playerView = new PlayerView(model->getPlayer());
    scene->addItem(playerView);
}

void GameView::clearScene() {
    scene->clear();
    playerView = nullptr; // scene->clear() désalloue les objets, on remet le pointeur à null par sécurité.
}

//-----------------------------------------------------------------------------------------MOUSEDETECTION
void GameView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        emit sig_rightClicked(); 
    }
    else if(event->button() == Qt::LeftButton){
        emit sig_leftClicked();
    }

    // Transmission de l'événement à la classe parente pour ne pas bloquer les interactions UI classiques.
    QGraphicsView::mousePressEvent(event);
}
//-------------------------------------------------------------------------------------------------------

void GameView::updateAllViews(float dt, GameModel* model) {
    if (!model) return;
    currentModel = model;

    if (playerView) playerView->updateView(dt);

    for (QGraphicsItem* item : scene->items()) {
        if (auto* zv = dynamic_cast<ZombieStdView*>(item))
            zv->updateView(dt);
        else if (auto* ev = dynamic_cast<ExitDoorView*>(item))
            ev->updateView();
    }

    // Force l'appel de paintEvent() (et donc de drawForeground) sans redessiner les géométries statiques.
    viewport()->update();   
}

void GameView::followPlayer(GameModel* model) {
    if (!model || !model->getPlayer()) return;
    Vector2f pos = model->getPlayer()->getPosition();
    centerOn(QPointF(pos.x + Cfg::ENTITY_SIZE / 2.0,
                     pos.y + Cfg::ENTITY_SIZE / 2.0));
}

void GameView::showOverlay(const QString& message) {
    overlayMessage  = message;
    showingOverlay  = true;
    viewport()->update();
}

void GameView::keyPressEvent(QKeyEvent* e) {
    pressedKeys.insert(e->key());
    QGraphicsView::keyPressEvent(e);
}

void GameView::keyReleaseEvent(QKeyEvent* e) {
    pressedKeys.remove(e->key());
    QGraphicsView::keyReleaseEvent(e);
}

void GameView::drawForeground(QPainter* painter, const QRectF& /*rect*/) {
    // Reset des transformations pour s'accrocher aux coordonnées absolues de l'écran (HUD fixe).
    painter->resetTransform();  

    if (currentModel) drawHUD(painter, currentModel);

    // ─── Overlay fin de partie ────────────────────────────────
    if (showingOverlay) {
        painter->fillRect(0, 0, Cfg::WINDOW_W, Cfg::WINDOW_H,
                          QColor(0, 0, 0, 160));
        QFont font("Arial", 28, QFont::Bold);
        painter->setFont(font);
        painter->setPen(Qt::yellow);
        painter->drawText(QRect(0, 0, Cfg::WINDOW_W, Cfg::WINDOW_H),
                          Qt::AlignCenter, overlayMessage);
    }
}

void GameView::drawHUD(QPainter* p, GameModel* model) {
    PlayerModel* player = model->getPlayer();
    if (!player) return;

    int mx = Cfg::HUD_MARGIN;
    int my = Cfg::HUD_MARGIN;

    drawHpBar(p, player->getHp(), player->getMaxHp(), mx, my);

    QFont font("Arial", 9, QFont::Bold);
    p->setFont(font);
    p->setPen(QColor(255, 210, 60));
    p->drawText(mx, my + Cfg::HP_BAR_H + 18,
                QString("GOLD : %1").arg(player->getGold()));

    if (player->isKatanaActive()) {
        p->setPen(QColor(255, 100, 60));
        p->drawText(mx, my + Cfg::HP_BAR_H + 36, "⚔ TRANCHE !");
    }
}

void GameView::drawHpBar(QPainter* p, int hp, int maxHp, int x, int y) {
    int bw = Cfg::HP_BAR_W;
    int bh = Cfg::HP_BAR_H;

    p->fillRect(x, y, bw, bh, QColor(50, 0, 0));

    if (maxHp > 0) {
        double ratio  = static_cast<double>(hp) / maxHp;
        int    filled = static_cast<int>(bw * ratio);
        // Commentaire développeur : Transition de couleur pour signifier visuellement l'état critique de la vie.
        QColor col = (ratio > 0.6) ? QColor(60, 200, 60)
                   : (ratio > 0.3) ? QColor(220, 140, 0)
                                   : QColor(200, 30, 30);
        p->fillRect(x, y, filled, bh, col);
    }

    p->setPen(QPen(Qt::white, 1));
    p->drawRect(x, y, bw, bh);
    QFont f("Arial", 8, QFont::Bold);
    p->setFont(f);
    p->setPen(Qt::white);
    p->drawText(x + 4, y + bh - 3,
                QString("HP : %1 / %2").arg(hp).arg(maxHp));
}