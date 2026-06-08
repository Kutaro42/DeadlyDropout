/**
 * @file rubbleview.h
 * @brief Déclaration de la vue graphique du gravat (projectile du zombie lanceur).
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef RUBBLEVIEW_H
#define RUBBLEVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "rubblemodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  RubbleView — représentation graphique d'un gravat.
//
//  Sprite attendu (ressources Qt) :
//    :/images/Textures/Enemy/Launcher/rubble.png  (16×16 ou 32×32)
//  Si le fichier est absent, un carré gris-brun est dessiné en fallback.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class RubbleView
 * @brief Élément graphique Qt synchronisé sur un RubbleModel.
 */
class RubbleView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructeur.
     * @param model  Pointeur vers le modèle du gravat.
     * @param parent Élément parent Qt (nullptr par défaut).
     */
    explicit RubbleView(RubbleModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Met à jour la position graphique et masque l'item si inactif.
     */
    void updateView();

private:
    RubbleModel* model; ///< Modèle source.

    /// Charge le sprite depuis les ressources ; construit un fallback si absent.
    static QPixmap loadSprite();
};

#endif // RUBBLEVIEW_H
