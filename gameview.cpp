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

    // Désactive les scrollbars (caméra gérée manuellement)
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Rendu pixel-perfect (pas d'antialiasing pour le style rétro)
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

    // Crée le PlayerView et l'insère dans la scène
    playerView = new PlayerView(model->getPlayer());
    scene->addItem(playerView);
}

void GameView::clearScene() {
    scene->clear();
    playerView = nullptr;   // scene->clear() a détruit l'item
}

void GameView::updateAllViews(float dt, GameModel* model) {
    if (!model) return;
    currentModel = model;

    // ─── Vue joueur ───────────────────────────────────────────
    if (playerView) playerView->updateView(dt);

    // ─── Vues ennemis et sortie (parcours de la scène) ────────
    for (QGraphicsItem* item : scene->items()) {
        if (auto* zv = dynamic_cast<ZombieStdView*>(item))
            zv->updateView(dt);
        else if (auto* ev = dynamic_cast<ExitDoorView*>(item))
            ev->updateView();
    }

    viewport()->update();   // déclenche paintEvent → drawForeground
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

// ── Entrées clavier ───────────────────────────────────────────────
void GameView::keyPressEvent(QKeyEvent* e) {
    pressedKeys.insert(e->key());
    QGraphicsView::keyPressEvent(e);
}

void GameView::keyReleaseEvent(QKeyEvent* e) {
    pressedKeys.remove(e->key());
    QGraphicsView::keyReleaseEvent(e);
}

// ── HUD (dessiné EN COORDONNÉES ÉCRAN — indépendant de la caméra) ─
void GameView::drawForeground(QPainter* painter, const QRectF& /*rect*/) {
    painter->resetTransform();  // repasse en coords écran

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

    // ─── Barre de vie ─────────────────────────────────────────
    drawHpBar(p, player->getHp(), player->getMaxHp(), mx, my);

    // ─── Gold ─────────────────────────────────────────────────
    QFont font("Arial", 9, QFont::Bold);
    p->setFont(font);
    p->setPen(QColor(255, 210, 60));
    p->drawText(mx, my + Cfg::HP_BAR_H + 18,
                QString("GOLD : %1").arg(player->getGold()));

    // ─── Indicateur katana (cooldown) ─────────────────────────
    if (player->isKatanaActive()) {
        p->setPen(QColor(255, 100, 60));
        p->drawText(mx, my + Cfg::HP_BAR_H + 36, "⚔ TRANCHE !");
    }
}

void GameView::drawHpBar(QPainter* p, int hp, int maxHp, int x, int y) {
    int bw = Cfg::HP_BAR_W;
    int bh = Cfg::HP_BAR_H;

    // Fond
    p->fillRect(x, y, bw, bh, QColor(50, 0, 0));

    // Remplissage proportionnel
    if (maxHp > 0) {
        double ratio  = static_cast<double>(hp) / maxHp;
        int    filled = static_cast<int>(bw * ratio);
        // Couleur : vert→orange→rouge selon le niveau de vie
        QColor col = (ratio > 0.6) ? QColor(60, 200, 60)
                   : (ratio > 0.3) ? QColor(220, 140, 0)
                                   : QColor(200, 30, 30);
        p->fillRect(x, y, filled, bh, col);
    }

    // Contour + texte
    p->setPen(QPen(Qt::white, 1));
    p->drawRect(x, y, bw, bh);
    QFont f("Arial", 8, QFont::Bold);
    p->setFont(f);
    p->setPen(Qt::white);
    p->drawText(x + 4, y + bh - 3,
                QString("HP : %1 / %2").arg(hp).arg(maxHp));
}
