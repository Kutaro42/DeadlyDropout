/**
 * @file gunview.cpp
 * @brief Implementation of the pistol view and in-flight bullet views.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */
#include "gunview.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

// ─── Sprite loading (static cache) ───────────────────────────────────
//
//  Attempt paths in order:
//    1. :/images/Textures/Player/Gun/gun.png   (horizontal sprite, barrel pointing right)
//    2. :/images/Textures/Player/gun.png       (alternative path)
//  If neither exists, generate a procedural fallback.
//
//  Expected PNG convention:
//    - Any dimensions (will be scaled to GUN_W × GUN_H).
//    - Barrel points RIGHT (angle 0 rad / 0°).
// ─────────────────────────────────────────────────────────────────────
QPixmap GunView::loadGunSprite()
{
    // Primary path
    QPixmap pm(":/images/Textures/Player/Gun/gun.png");
    if (pm.isNull())
        pm = QPixmap(":/images/Textures/Player/gun.png");   // alternative path

    if (!pm.isNull())
        return pm.scaled(GUN_W, GUN_H,
                         Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);

    // ── Procedural fallback ──────────────────────────────────────────
    // Dark anthracite rectangle (body) + right-side barrel.
    // Dimensions: GUN_W × GUN_H, transparent background.
    pm = QPixmap(GUN_W, GUN_H);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);

    // Gun body (left side)
    p.setBrush(QColor(60, 62, 68));
    p.setPen(QPen(QColor(25, 25, 28), 1));
    p.drawRoundedRect(2, GUN_H / 4, GUN_W * 2 / 3, GUN_H / 2, 3, 3);

    // Barrel (right side, vertically centred)
    p.setBrush(QColor(85, 87, 92));
    p.setPen(Qt::NoPen);
    int canonY = (GUN_H - 4) / 2;
    p.drawRect(GUN_W * 2 / 3, canonY, GUN_W / 3, 4);

    // Grip (bottom-left)
    p.setBrush(QColor(45, 47, 52));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(4, GUN_H / 2, GUN_W / 4, GUN_H / 2 - 2, 2, 2);

    return pm;
}

// ─── Constructor ─────────────────────────────────────────────────────
GunView::GunView(GunModel* gm, PlayerModel* pm, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , gun(gm)
    , player(pm)
    , flipped(false)
{
    static const QPixmap sprite = loadGunSprite();
    setPixmap(sprite);

    // The pivot must be at the middle-left of the sprite (anchor on the player).
    // Using (0, GUN_H/2): the gun "rotates" around this point when setRotation() is applied.
    setTransformOriginPoint(0.0, GUN_H / 2.0);

    setZValue(Cfg::Z_PLAYER + 0.5);
}

// ─── Main update ─────────────────────────────────────────────────────
void GunView::updateView(float dt, QPointF mouseScene, QGraphicsScene* scene)
{
    Q_UNUSED(dt);
    if (!player || !gun) return;

    // Hide the gun during a katana attack to avoid visual overlap.
    if (player->isKatanaActive()) {
        setVisible(false);
        syncBulletViews(scene);
        return;
    }
    setVisible(true);

    // ── 1. Player centre in scene coordinates ────────────────────────
    Vector2f ppos = player->getPosition();
    double cx = ppos.x + Cfg::ENTITY_SIZE / 2.0;
    double cy = ppos.y + Cfg::ENTITY_SIZE / 2.0;

    double dx    = mouseScene.x() - cx;
    double dy    = mouseScene.y() - cy;
    double angle = std::atan2(dy, dx);   // radians, 0 = right
    double deg   = angle * 180.0 / M_PI;

    // ── 2. Flip detection ────────────────────────────────────────────
    // When the cursor is left of the player (cos < 0), the sprite is flipped
    // vertically so the barrel still points toward the cursor and the grip stays
    // on the correct side.  Flip is managed via a pre-mirrored QPixmap rather
    // than via setTransform, because mixing setTransform and setRotation causes
    // compound transformation order issues in Qt.
    bool needFlip = (dx < 0.0);
    if (needFlip != flipped) {
        flipped = needFlip;
        static const QPixmap base      = loadGunSprite();
        static const QPixmap flippedPm = base.transformed(QTransform().scale(1.0, -1.0));
        setPixmap(flipped ? flippedPm : base);
    }

    // ── 3. Position ──────────────────────────────────────────────────
    // Anchor the left edge of the sprite at the player's centre, offset by a
    // small radial distance (GUN_OFFSET_X) so the gun does not overlap the
    // player sprite.  The pivot at (0, GUN_H/2) means setRotation() rotates
    // the entire sprite around this anchor without positional drift.
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);

    double anchorX = cx + cosA * Cfg::GUN_OFFSET_X;
    double anchorY = cy + sinA * Cfg::GUN_OFFSET_X - GUN_H / 2.0;  // vertically centred

    setPos(anchorX, anchorY);

    // ── 4. Pure rotation ─────────────────────────────────────────────
    // setTransformOriginPoint(0, GUN_H/2) + setRotation(deg) rotates the sprite
    // around the anchor point → no positional drift.
    setTransformOriginPoint(0.0, GUN_H / 2.0);
    setRotation(deg);

    // ── 5. Bullet view synchronisation ───────────────────────────────
    syncBulletViews(scene);
}

// ─── BulletView ↔ BulletModel synchronisation ───────────────────────
void GunView::syncBulletViews(QGraphicsScene* scene)
{
    QVector<BulletModel*>& bullets = gun->getBullets();

    // 1) Create views for new bullets
    while (bulletViews.size() < bullets.size()) {
        int idx = bulletViews.size();
        auto* bv = new BulletView(bullets[idx]);
        scene->addItem(bv);
        bulletViews.append(bv);
        bulletModels.append(bullets[idx]);
    }

    // 2) Remove views for inactive bullets
    for (int i = bulletViews.size() - 1; i >= 0; --i) {
        if (!bulletModels[i]->isActive()) {
            scene->removeItem(bulletViews[i]);
            delete bulletViews[i];
            bulletViews.removeAt(i);
            bulletModels.removeAt(i);
        }
    }

    // 3) Update positions of remaining views
    for (int i = 0; i < bulletViews.size(); ++i)
        bulletViews[i]->updateView();

    // 4) Free memory of inactive bullet models
    gun->pruneInactiveBullets();
}