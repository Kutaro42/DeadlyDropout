/**
 * @file gunview.cpp
 * @brief Implémentation de la vue du pistolet et des balles en vol.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#include "gunview.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <cmath>

// ─── Chargement du sprite (cache statique) ───────────────────────
//
//  On tente les chemins dans l'ordre suivant :
//    1. :/images/Textures/Player/Gun/gun.png   (sprite horizontal, pointe à droite)
//    2. :/images/Textures/Player/gun.png       (chemin alternatif)
//  Si aucun n'est trouvé, un fallback procédural est généré.
//
//  Convention attendue pour le PNG source :
//    - Largeur quelconque, hauteur quelconque.
//    - Le canon pointe vers la DROITE (angle 0 rad / 0°).
//    - Le sprite sera mis à l'échelle GUN_W × GUN_H à l'affichage.
// ─────────────────────────────────────────────────────────────────
QPixmap GunView::loadGunSprite()
{
    // Chemin principal
    QPixmap pm(":/images/Textures/Player/Gun/gun.png");
    if (pm.isNull())
        pm = QPixmap(":/images/Textures/Player/gun.png");   // chemin alternatif

    if (!pm.isNull())
        return pm.scaled(GUN_W, GUN_H,
                         Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);

    // ── Fallback procédural ──────────────────────────────────────
    // Rectangle gris anthracite (corps) + canon à droite.
    // Dimensions : GUN_W × GUN_H, fond transparent.
    pm = QPixmap(GUN_W, GUN_H);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);

    // Corps du pistolet (gauche)
    p.setBrush(QColor(60, 62, 68));
    p.setPen(QPen(QColor(25, 25, 28), 1));
    p.drawRoundedRect(2, GUN_H / 4, GUN_W * 2 / 3, GUN_H / 2, 3, 3);

    // Canon (droite, centré verticalement)
    p.setBrush(QColor(85, 87, 92));
    p.setPen(Qt::NoPen);
    int canonY = (GUN_H - 4) / 2;
    p.drawRect(GUN_W * 2 / 3, canonY, GUN_W / 3, 4);

    // Poignée (bas-gauche)
    p.setBrush(QColor(45, 47, 52));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(4, GUN_H / 2, GUN_W / 4, GUN_H / 2 - 2, 2, 2);

    return pm;
}

// ─── Constructeur ────────────────────────────────────────────────
GunView::GunView(GunModel* gm, PlayerModel* pm, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , gun(gm)
    , player(pm)
    , flipped(false)
{
    static const QPixmap sprite = loadGunSprite();
    setPixmap(sprite);

    // Le pivot doit être au milieu gauche du sprite (point d'ancrage sur le joueur).
    // On utilise (0, GUN_H/2) : le pistolet « tourne » autour de ce point
    // quand on applique la rotation via setRotation().
    setTransformOriginPoint(0.0, GUN_H / 2.0);

    setZValue(Cfg::Z_PLAYER + 0.5);
}

// ─── Mise à jour principale ──────────────────────────────────────
void GunView::updateView(float dt, QPointF mouseScene, QGraphicsScene* scene)
{
    Q_UNUSED(dt);
    if (!player || !gun) return;

    if (player->isKatanaActive()) {
        setVisible(false);
        syncBulletViews(scene);
        return;
    }
    setVisible(true);

    // ── 1. Centre du joueur en coordonnées scène ─────────────────
    Vector2f ppos = player->getPosition();
    double cx = ppos.x + Cfg::ENTITY_SIZE / 2.0;
    double cy = ppos.y + Cfg::ENTITY_SIZE / 2.0;

    double dx    = mouseScene.x() - cx;
    double dy    = mouseScene.y() - cy;
    double angle = std::atan2(dy, dx);   // radians, 0 = droite
    double deg   = angle * 180.0 / M_PI;

    // ── 2. Détermination du flip ──────────────────────────────────
    // Quand la souris est à gauche du joueur (cos < 0) on retourne
    // le sprite verticalement pour qu'il reste lisible.
    // Le flip est géré via setPixmap d'une version miroir, sans toucher
    // à la transformation de rotation — les deux ne se mélangent pas.
    bool needFlip = (dx < 0.0);
    if (needFlip != flipped) {
        flipped = needFlip;
        static const QPixmap base    = loadGunSprite();
        static const QPixmap flippedPm = base.transformed(
            QTransform().scale(1.0, -1.0));
        setPixmap(flipped ? flippedPm : base);
    }

    // ── 3. Position ───────────────────────────────────────────────
    // On ancre le coin gauche du sprite au centre du joueur,
    // puis on déplace d'un petit décalage dans la direction du canon.
    // Le pivot étant en (0, GUN_H/2), setRotation tourne autour
    // du coin gauche-milieu = point d'ancrage sur le joueur.
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);

    // Ancre : centre joueur + léger décalage radial (évite chevauchement)
    double anchorX = cx + cosA * Cfg::GUN_OFFSET_X;
    double anchorY = cy + sinA * Cfg::GUN_OFFSET_X - GUN_H / 2.0;  // centré verticalement

    setPos(anchorX, anchorY);

    // ── 4. Rotation pure ──────────────────────────────────────────
    // setTransformOriginPoint(0, GUN_H/2) + setRotation(deg) fait
    // tourner le sprite autour du point d'ancrage = pas de dérive.
    setTransformOriginPoint(0.0, GUN_H / 2.0);
    setRotation(deg);

    // ── 5. Synchronisation des vues de balles ────────────────────
    syncBulletViews(scene);
}

// ─── Synchronisation BulletView ↔ BulletModel ───────────────────
void GunView::syncBulletViews(QGraphicsScene* scene)
{
    QVector<BulletModel*>& bullets = gun->getBullets();

    // 1) Créer les vues pour les nouvelles balles
    while (bulletViews.size() < bullets.size()) {
        int idx = bulletViews.size();
        auto* bv = new BulletView(bullets[idx]);
        scene->addItem(bv);
        bulletViews.append(bv);
        bulletModels.append(bullets[idx]);
    }

    // 2) Supprimer les vues des balles inactives
    for (int i = bulletViews.size() - 1; i >= 0; --i) {
        if (!bulletModels[i]->isActive()) {
            scene->removeItem(bulletViews[i]);
            delete bulletViews[i];
            bulletViews.removeAt(i);
            bulletModels.removeAt(i);
        }
    }

    // 3) Mettre à jour la position des vues restantes
    for (int i = 0; i < bulletViews.size(); ++i)
        bulletViews[i]->updateView();

    // 4) Libérer la mémoire des modèles inactifs
    gun->pruneInactiveBullets();
}
