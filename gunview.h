/**
 * @file gunview.h
 * @brief Vue du pistolet : collé au joueur, orienté vers le curseur souris.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */
#ifndef GUNVIEW_H
#define GUNVIEW_H

#include "bulletview.h"
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "gunmodel.h"
#include "playermodel.h"
#include "bulletmodel.h"
#include "config.h"



/**
 * @class GunView
 * @brief Item graphique du pistolet, orienté vers le curseur et ancré sur le joueur.
 *
 * Positionnement :
 *  - Le pivot de rotation est le bord GAUCHE du sprite (point d'ancrage sur le joueur).
 *  - setRotation(deg) fait tourner le sprite autour de ce pivot → pas de dérive.
 *  - Quand la souris est à gauche du joueur, le sprite est pre-flippé verticalement
 *    via un QPixmap miroir (pas de setTransform pour éviter tout conflit avec setRotation).
 *
 * Chargement du sprite (chemin principal, puis chemin alternatif, puis fallback) :
 *  1. :/images/Textures/Player/Gun/gun.png
 *  2. :/images/Textures/Player/gun.png
 *  3. Fallback procédural (corps + canon gris anthracite, GUN_W × GUN_H).
 *
 * Convention du PNG attendu : canon pointant vers la DROITE (angle 0°),
 * fond transparent, dimensions quelconques (sera mis à l'échelle GUN_W × GUN_H).
 *
 * Balles : une QGraphicsEllipseItem est créée pour chaque BulletModel actif
 * et retirée de la scène dès qu'il devient inactif.
 */
class GunView : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Constructeur.
     * @param gunModel    Modèle logique du pistolet (possède les BulletModel).
     * @param playerModel Modèle du joueur (position d'ancrage).
     * @param parent      Item parent Qt (nullptr = directement dans la scène).
     */
    explicit GunView(GunModel*      gunModel,
                     PlayerModel*   playerModel,
                     QGraphicsItem* parent = nullptr);

    /**
     * @brief Met à jour position + rotation du pistolet et les vues de balles.
     * @param dt         Delta-time en secondes.
     * @param mouseScene Position courante du curseur en coordonnées scène.
     * @param scene      Scène Qt pour y ajouter / supprimer les BulletView.
     */
    void updateView(float dt, QPointF mouseScene, QGraphicsScene* scene);

private:
    GunModel*    gun;
    PlayerModel* player;
    bool         flipped;  ///< true quand le sprite est affiché en miroir vertical.

    /// Vues des balles en vol.
    QVector<BulletView*> bulletViews;
    /// Modèles correspondants (même index que bulletViews).
    QVector<BulletModel*>   bulletModels;

    /// Synchronise bulletViews;  getBullets() du modèle.
    void syncBulletViews(QGraphicsScene* scene);

    /**
     * @brief Charge le sprite depuis les ressources ou génère un fallback.
     *        Résultat mis en cache statique dans le .cpp.
     */
    static QPixmap loadGunSprite();

    // ─── Dimensions d'affichage du sprite pistolet ───────────────
    static constexpr int GUN_W = 32; ///< Largeur affichée du pistolet (px).
    static constexpr int GUN_H = 32; ///< Hauteur affichée du pistolet (px).
};

#endif // GUNVIEW_H
