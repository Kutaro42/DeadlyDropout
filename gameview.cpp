/**
 * @file gameview.cpp
 * @brief Implémentation du moteur de rendu 2D et de la captation des inputs.
 */

#include "gameview.h"
#include "zombielauncherview.h"
#include "standarddoorview.h"
#include "bossdoorview.h"
#include "guiddef.h"
#include <QFont>
#include <QFontMetrics>

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      playerView(nullptr),
      currentModel(nullptr),
      showingOverlay(false),
      itsMouseScenePos(0.0, 0.0)
{
    setScene(scene);
    setFixedSize(Cfg::WINDOW_W, Cfg::WINDOW_H);

    // Désactive les scrollbars natives, car la caméra (followPlayer) est gérée par code.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configuration volontaire d'un rendu "pixel-perfect" sans flou pour l'esthétique rétro.
    setRenderHint(QPainter::Antialiasing,          false);
    setRenderHint(QPainter::SmoothPixmapTransform, false);

    setBackgroundBrush(QBrush(QColor(15, 12, 8)));
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Deadly Dropout");

    // Active le tracking de la souris sans qu'un bouton soit pressé,
    // afin de toujours connaître la position courante du curseur.
    setMouseTracking(true);
}

GameView::~GameView() {}

void GameView::initWithModel(GameModel* model) {
    currentModel = model;
    if (!model || !model->getPlayer()) return;

    playerView = new PlayerView(model->getPlayer());
    scene->addItem(playerView);

    gunView = new GunView(&model->getPlayer()->getGun(), model->getPlayer());
    scene->addItem(gunView);
}

void GameView::clearScene() {
    scene->clear();
    playerView = nullptr;
    gunView    = nullptr;
}

// ─── Souris ──────────────────────────────────────────────────────────────────

void GameView::mouseMoveEvent(QMouseEvent* event) {
    // Convertit les coordonnées widget → coordonnées scène et met à jour le cache.
    itsMouseScenePos = mapToScene(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}

void GameView::mousePressEvent(QMouseEvent* event) {
    // Met à jour la position même au moment du clic (la souris peut ne pas avoir bougé
    // depuis le dernier mouseMoveEvent si le joueur a déplacé la caméra).
    itsMouseScenePos = mapToScene(event->pos());

    if (event->button() == Qt::RightButton) {
        emit sig_rightClicked(itsMouseScenePos);
    } else if (event->button() == Qt::LeftButton) {
        emit sig_leftClicked(itsMouseScenePos);
    }

    QGraphicsView::mousePressEvent(event);
}

// ─────────────────────────────────────────────────────────────────────────────

void GameView::updateAllViews(float dt, GameModel* model) {
    if (!model) return;
    currentModel = model;

    if (playerView) playerView->updateView(dt);
    if (gunView)    gunView->updateView(dt, itsMouseScenePos, scene);

    for (QGraphicsItem* item : scene->items()) {
        if (auto* zv = dynamic_cast<ZombieStdView*>(item))
            zv->updateView(dt);
        else if (auto* lv = dynamic_cast<ZombieLauncherView*>(item))
            lv->updateView(dt);
        else if (auto* ev = dynamic_cast<ExitDoorView*>(item))
            ev->updateView();
        else if (auto*Bossv = dynamic_cast<BossView*>(item))
            Bossv->updateView(dt);
        else if (auto* dv = dynamic_cast<StandardDoorView*>(item))
            dv->updateView();
        else if (auto* bdv = dynamic_cast<BossDoorView*>(item))
            bdv->updateView();
    }

    viewport()->update();
}

void GameView::followPlayer(GameModel* model) {
    if (!model || !model->getPlayer()) return;
    Vector2f pos = model->getPlayer()->getPosition();
    centerOn(QPointF(pos.x + Cfg::ENTITY_SIZE / 2.0,
                     pos.y + Cfg::ENTITY_SIZE / 2.0));
}

void GameView::showOverlay(const QString& message) {
    overlayMessage = message;
    showingOverlay = true;

    if (!menuButton) {
        menuButton = new QPushButton("Menu", this);
        menuButton->setFixedSize(140, 45);
        menuButton->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1a1a1a, stop:1 #0d0d0d);"
            "  color: #00ffcc;"
            "  font-size: 16px;"
            "  font-weight: bold;"
            "  border: 2px solid #00ffcc;"
            "  border-radius: 8px;"
            "}"
            "QPushButton:hover { background: #00ffcc; color: #000000; border: 2px solid #ffffff; }"
            "QPushButton:pressed { background: #00cc99; color: #ffffff; }");
    }

    if (!replayButton) {
        replayButton = new QPushButton("Rejouer", this);
        replayButton->setFixedSize(140, 45);
        replayButton->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1a1a1a, stop:1 #0d0d0d);"
            "  color: #ff3366;"
            "  font-size: 16px;"
            "  font-weight: bold;"
            "  border: 2px solid #ff3366;"
            "  border-radius: 8px;"
            "}"
            "QPushButton:hover { background: #ff3366; color: #000000; border: 2px solid #ffffff; }"
            "QPushButton:pressed { background: #cc0044; color: #ffffff; }");
    }

    connect(replayButton, &QPushButton::clicked, this, &GameView::sigRestartRq);
    connect(menuButton,   &QPushButton::clicked, this, &GameView::sigQuitRq);

    int viewW   = this->width();
    int viewH   = this->height();
    int centerY = viewH / 2 + 50;
    menuButton->move(viewW / 2 - 155, centerY);
    replayButton->move(viewW / 2 + 15, centerY);
    menuButton->show();
    replayButton->show();

    viewport()->update();
}

void GameView::keyPressEvent(QKeyEvent* e) {
    pressedKeys.insert(e->key());
    // 1. On intercepte Échap en premier, sans conditions
    if (e->key() == Qt::Key_Escape && !e->isAutoRepeat()) {
        emit sig_escapePressed();
    }

    if (e->key() == Qt::Key_Up   || e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) {

        e->accept();
        return;
    }
    QGraphicsView::keyPressEvent(e);
}

void GameView::keyReleaseEvent(QKeyEvent* e) {
    pressedKeys.remove(e->key());
    if (e->key() == Qt::Key_Up   || e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) {
        e->accept();
        return;
    }
    QGraphicsView::keyReleaseEvent(e);
}

void GameView::drawForeground(QPainter* painter, const QRectF&) {
    painter->resetTransform();

    if (currentModel) drawHUD(painter, currentModel);

    if (showingOverlay) {
        painter->fillRect(0, 0, Cfg::WINDOW_W, Cfg::WINDOW_H, QColor(0, 0, 0, 160));
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

    if (player->getHasKey()) {
        p->setPen(QColor(192, 192, 192));
        p->drawText(mx + 72, my + Cfg::HP_BAR_H + 18, "KEY: FOUND");
    } else {
        p->setPen(QColor(192, 192, 192));
        p->drawText(mx + 72, my + Cfg::HP_BAR_H + 18, "KEY: NOT FOUND");
    }

    // Munitions pistolet
    const GunModel& gun = player->getGun();
    QColor ammoColor = (gun.getMag() == 0) ? QColor(220, 60, 60) : QColor(100, 200, 255);
    p->setPen(ammoColor);
    p->drawText(mx, my + Cfg::HP_BAR_H + 36,
                QString("GUN : %1 / %2  [R:%3]")
                    .arg(gun.getMag())
                    .arg(gun.getMagSize())
                    .arg(gun.getReserve()));

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
