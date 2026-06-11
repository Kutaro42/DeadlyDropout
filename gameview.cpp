/**
 * @file gameview.cpp
 * @brief Implémentation du moteur de rendu 2D et de la captation des inputs.
 */

#include "gameview.h"
#include "zombielauncherview.h"
#include <QGraphicsTextItem>
#include <QGraphicsOpacityEffect>
#include <QFont>
#include "standarddoorview.h"
#include "bossdoorview.h"
#include "guiddef.h"
#include <QFont>
#include <QFontMetrics>

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      playerView(nullptr),
      gunView(nullptr),
      currentModel(nullptr),
      showingOverlay(false),
      itsMouseViewPos(0.0, 0.0),
      lastGold(-1),
      goldFlashTimer(0.0f),
      goldFlashVisible(false),
      goldFlashCycle(0.0f),
      lastHasKey(false),
      keyFlashTimer(0.0f),
      keyFlashVisible(false),
      keyFlashCycle(0.0f)
{
    setScene(scene);
    setFixedSize(Cfg::WINDOW_W, Cfg::WINDOW_H);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing,          false);
    setRenderHint(QPainter::SmoothPixmapTransform, false);
    setBackgroundBrush(QBrush(QColor(15, 12, 8)));
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Deadly Dropout");
    setMouseTracking(true);

    // ── Chargement spritesheets cooldown HUD ──────────────────────
    // gun : 15 frames 26×26 | katana : 21 frames 26×26
    // frame 0 = arme prête, dernière frame = vient d'être utilisée
    auto loadCdSheet = [](const QString& path, int nFrames,
                          QVector<QPixmap>& out) {
        QPixmap sheet(path);
        if (sheet.isNull()) return;
        for (int i = 0; i < nFrames; ++i)
            out.append(sheet.copy(i * 26, 0, 26, 26));
    };
    loadCdSheet(":/images/Textures/Cooldown/cooldowngun.png",    15, gunCdFrames);
    loadCdSheet(":/images/Textures/Cooldown/cooldownkatana.png", 21, katanaCdFrames);
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
    activePopups.clear(); // items détruits par scene->clear(), on vide juste les structs
    scene->clear();
    playerView = nullptr;
    gunView    = nullptr;
}

// ─── Souris ───────────────────────────────────────────────────────
void GameView::wheelEvent(QWheelEvent *event)
{
    // On accepte l'événement pour indiquer qu'il est géré,
    // mais on ne fait rien, ce qui désactive le scroll.
    event->ignore();
}

void GameView::mouseMoveEvent(QMouseEvent* event) {
    itsMouseViewPos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
    itsMouseViewPos = event->pos();
}

void GameView::mousePressEvent(QMouseEvent* event) {

    itsMouseViewPos = event->pos();
    QPointF scenePos = mapToScene(event->pos());
    if (event->button() == Qt::RightButton)
        emit sig_rightClicked(scenePos);
    else if (event->button() == Qt::LeftButton)
        emit sig_leftClicked(scenePos);
}

// ─── Mise à jour ──────────────────────────────────────────────────

void GameView::updateAllViews(float dt, GameModel* model, LevelController* levelCtrl) {
    if (!model) return;
    currentModel = model;

    // ── Clignotement gold ─────────────────────────────────────────
    if (model->getPlayer()) {
        int currentGold = model->getPlayer()->getGold();
        if (lastGold >= 0 && currentGold > lastGold) {
            goldFlashTimer   = 1.2f;
            goldFlashCycle   = 0.0f;
            goldFlashVisible = true;
        }
        lastGold = currentGold;
    }
    if (goldFlashTimer > 0.0f) {
        goldFlashTimer -= dt;
        goldFlashCycle += dt;
        if (goldFlashCycle >= 0.1f) { goldFlashCycle = 0.0f; goldFlashVisible = !goldFlashVisible; }
        if (goldFlashTimer <= 0.0f) { goldFlashTimer = 0.0f; goldFlashVisible = false; }
    }

    // ── Clignotement clé ──────────────────────────────────────────
    if (model->getPlayer()) {
        bool currentKey = model->getPlayer()->getHasKey();
        if (currentKey && !lastHasKey) {
            keyFlashTimer   = 1.8f;
            keyFlashCycle   = 0.0f;
            keyFlashVisible = true;
        }
        lastHasKey = currentKey;
    }
    if (keyFlashTimer > 0.0f) {
        keyFlashTimer -= dt;
        keyFlashCycle += dt;
        if (keyFlashCycle >= 0.1f) { keyFlashCycle = 0.0f; keyFlashVisible = !keyFlashVisible; }
        if (keyFlashTimer <= 0.0f) { keyFlashTimer = 0.0f; keyFlashVisible = false; }
    }

    // ── Création des popups (données reçues du Controller) ───────
    if (levelCtrl) {
        for (const PopupRequest& req : levelCtrl->getPendingPopups()) {
            auto* item = new QGraphicsTextItem();
            bool isDmg = (req.type == PopupRequest::Type::Damage);
            item->setPlainText(isDmg
                ? QString("-%1").arg(req.value)
                : QString("+%1 G").arg(req.value));
            QFont f("Arial", isDmg ? 11 : 10, QFont::Bold);
            item->setFont(f);
            item->setDefaultTextColor(isDmg ? QColor(255,60,60) : QColor(255,200,0));
            item->setZValue(Cfg::Z_POPUP);
            // Centrer horizontalement sur la position
            item->setPos(req.pos.x() - item->boundingRect().width() / 2.0,
                         req.pos.y());
            auto* fx = new QGraphicsOpacityEffect();
            fx->setOpacity(1.0);
            item->setGraphicsEffect(fx);
            scene->addItem(item);
            activePopups.append({item, fx, 0.0f,
                isDmg ? Cfg::POPUP_DMG_DUR : Cfg::POPUP_GOLD_DUR,
                static_cast<float>(req.pos.y())});
        }
        levelCtrl->clearPendingPopups();
    }

    // ── Tick et nettoyage des popups actifs ───────────────────────
    for (int i = activePopups.size() - 1; i >= 0; --i) {
        PopupAnim& pa = activePopups[i];
        pa.elapsed += dt;
        if (pa.elapsed >= pa.duration) {
            if (pa.item->scene()) scene->removeItem(pa.item);
            delete pa.item; // fx est enfant de item, détruit automatiquement
            activePopups.removeAt(i);
            continue;
        }
        float progress = pa.elapsed / pa.duration;
        // Montée verticale
        pa.item->setY(static_cast<double>(pa.originY - Cfg::POPUP_RISE_PX * progress));
        // Fondu sur la 2e moitié
        double opacity = (progress > 0.5f)
                         ? 1.0 - (progress - 0.5) * 2.0
                         : 1.0;
        pa.fx->setOpacity(opacity);
    }

    if (playerView) playerView->updateView(dt);
    if (gunView)    gunView->updateView(dt, mapToScene(itsMouseViewPos.toPoint()), scene);

    for (QGraphicsItem* item : scene->items()) {
        if (auto* zv = dynamic_cast<ZombieStdView*>(item))
            zv->updateView(dt);
        else if (auto* lv = dynamic_cast<ZombieLauncherView*>(item))
            lv->updateView(dt);
        else if (auto* ev = dynamic_cast<ExitDoorView*>(item))
            ev->updateView();
        else if (auto* bv = dynamic_cast<BossView*>(item))
            bv->updateView(dt);
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
            "  color: #00ffcc; font-size: 16px; font-weight: bold;"
            "  border: 2px solid #00ffcc; border-radius: 8px; }"
            "QPushButton:hover { background: #00ffcc; color: #000000; border: 2px solid #ffffff; }"
            "QPushButton:pressed { background: #00cc99; color: #ffffff; }");
    }
    if (!replayButton) {
        replayButton = new QPushButton("Rejouer", this);
        replayButton->setFixedSize(140, 45);
        replayButton->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1a1a1a, stop:1 #0d0d0d);"
            "  color: #ff3366; font-size: 16px; font-weight: bold;"
            "  border: 2px solid #ff3366; border-radius: 8px; }"
            "QPushButton:hover { background: #ff3366; color: #000000; border: 2px solid #ffffff; }"
            "QPushButton:pressed { background: #cc0044; color: #ffffff; }");
    }

    connect(replayButton, &QPushButton::clicked, this, &GameView::sigRestartRq);
    connect(menuButton,   &QPushButton::clicked, this, &GameView::sigQuitRq);

    int centerY = this->height() / 2 + 50;
    menuButton->move(this->width() / 2 - 155, centerY);
    replayButton->move(this->width() / 2 + 15, centerY);
    menuButton->show();
    replayButton->show();
    viewport()->update();
}

// ─── Clavier ──────────────────────────────────────────────────────

void GameView::keyPressEvent(QKeyEvent* e) {
    pressedKeys.insert(e->key());
    if (e->key() == Qt::Key_Escape && !e->isAutoRepeat())
        emit sig_escapePressed();
    if (e->key() == Qt::Key_Up   || e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) {
        e->accept(); return;
    }
    QGraphicsView::keyPressEvent(e);
}

void GameView::keyReleaseEvent(QKeyEvent* e) {
    pressedKeys.remove(e->key());
    if (e->key() == Qt::Key_Up   || e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) {
        e->accept(); return;
    }
    QGraphicsView::keyReleaseEvent(e);
}

// ─── Rendu HUD ────────────────────────────────────────────────────

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

    const int mx = Cfg::HUD_MARGIN;
    const int my = Cfg::HUD_MARGIN;

    drawHpBar(p, player->getHp(), player->getMaxHp(), mx, my);

    QFont font("Arial", 9, QFont::Bold);
    p->setFont(font);

    // Ligne GOLD + CLÉ : même rowRectY / rowTextY pour alignement parfait
    const int rowH     = 16;
    const int rowRectY = my + Cfg::HP_BAR_H + 4;
    const int rowTextY = rowRectY + 12;

    // ── Gold ──────────────────────────────────────────────────────
    {
        const int goldRectW = 68; // s'arrête avant KEY (mx+72)
        if (goldFlashTimer > 0.0f && goldFlashVisible) {
            p->fillRect(mx, rowRectY, goldRectW, rowH, QColor(255, 210, 0, 200));
            p->setPen(QColor(0, 0, 0));
        } else {
            p->setPen(QColor(255, 210, 60));
        }
        p->drawText(mx, rowTextY, QString("GOLD : %1").arg(player->getGold()));
    }

    // ── Clé ───────────────────────────────────────────────────────
    {
        const int keyRectX = mx + 72;
        if (player->getHasKey()) {
            if (keyFlashTimer > 0.0f && keyFlashVisible) {
                QFontMetrics fm(p->font());
                int tw = fm.horizontalAdvance("KEY: FOUND");
                p->fillRect(keyRectX, rowRectY, tw + 4, rowH, QColor(220, 220, 100, 220));
                p->setPen(QColor(0, 0, 0));
            } else {
                p->setPen(QColor(192, 192, 192));
            }
            p->drawText(keyRectX, rowTextY, "KEY: FOUND");
        } else {
            p->setPen(QColor(192, 192, 192));
            p->drawText(keyRectX, rowTextY, "KEY: NOT FOUND");
        }
    }

    // ── Niveau (haut à droite) ────────────────────────────────────
    {
        QString lvlText = QString("NIVEAU  %1").arg(model->getLevelIndex() + 1);
        QFontMetrics fm(p->font());
        int tw = fm.horizontalAdvance(lvlText);
        int lx = Cfg::WINDOW_W - Cfg::HUD_MARGIN - tw - 4;
        p->fillRect(lx - 2, my, tw + 8, rowH, QColor(0, 0, 0, 140));
        p->setPen(QColor(200, 200, 200));
        p->drawText(lx + 2, my + 12, lvlText);
    }

    // ── Icônes cooldown (au-dessus des munitions, bas à gauche) ───
    {
        const int ICON_SIZE = 65; // 26px source × 250%
        const int iconY = Cfg::WINDOW_H - Cfg::HUD_MARGIN - 4 - 14 - ICON_SIZE - 4;
        const int iconX = Cfg::HUD_MARGIN;

        if (!gunCdFrames.isEmpty()) {
            float ratio = player->getGun().getCooldownRatio();
            int   frame = qBound(0, static_cast<int>(std::round(
                              ratio * (gunCdFrames.size() - 1))), gunCdFrames.size() - 1);
            p->drawPixmap(iconX, iconY, ICON_SIZE, ICON_SIZE, gunCdFrames[frame]);
        }
        if (!katanaCdFrames.isEmpty()) {
            float ratio = player->getKatana().getCooldownRatio();
            int   frame = qBound(0, static_cast<int>(std::round(
                              ratio * (katanaCdFrames.size() - 1))), katanaCdFrames.size() - 1);
            p->drawPixmap(iconX + ICON_SIZE + 6, iconY, ICON_SIZE, ICON_SIZE, katanaCdFrames[frame]);
        }
    }

    // ── Munitions (bas à gauche, police 14pt) ─────────────────────
    {
        const GunModel& gun = player->getGun();
        QColor ammoColor = (gun.getMag() == 0) ? QColor(220, 60, 60) : QColor(100, 200, 255);
        QString ammoText = QString("GUN : %1 / %2  [R:%3]")
                               .arg(gun.getMag()).arg(gun.getMagSize()).arg(gun.getReserve());
        QFont ammoFont("Arial", 14, QFont::Bold);
        p->setFont(ammoFont);
        QFontMetrics fm(ammoFont);
        int ax = Cfg::HUD_MARGIN;
        int ay = Cfg::WINDOW_H - Cfg::HUD_MARGIN - 4;
        p->fillRect(ax - 4, ay - fm.height() + 2, fm.horizontalAdvance(ammoText) + 10,
                    fm.height() + 4, QColor(0, 0, 0, 140));
        p->setPen(ammoColor);
        p->drawText(ax, ay, ammoText);
        p->setFont(font); // restaurer police HUD
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
    p->drawText(x + 4, y + bh - 3, QString("HP : %1 / %2").arg(hp).arg(maxHp));
}
