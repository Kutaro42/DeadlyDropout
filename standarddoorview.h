/**
 * @file standarddoorview.h
 * @brief Déclaration de la vue graphique pour la porte standard.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef STANDARDDOORVIEW_H
#define STANDARDDOORVIEW_H

#include <QPainter>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include "standarddoormodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  StandardDoorView — représentation visuelle de la porte standard.
//  Couleur marron sombre = fermée, beige/doré = ouverte.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class StandardDoorView
 * @brief Rendu Qt de la porte standard avec code couleur ouvert/fermé.
 */
class StandardDoorView : public QGraphicsRectItem {
private:
    StandardDoorModel* itsModel;      ///< Référence vers le modèle logique.
    bool               cachedOpen; ///< Cache de l'état pour éviter un redraw inutile.

    bool           wasLocked;
    QPixmap        unlockedPixmap;
    QPixmap        lockedPixmap;


public:
    /**
     * @brief Constructeur.
     * @param model  Modèle source.
     * @param parent Élément parent Qt (optionnel).
     */
    explicit StandardDoorView(StandardDoorModel* model,
                              QGraphicsItem* parent = nullptr);

    /**
     * @brief Actualise la couleur selon l'état ouvert/fermé du modèle.
     */
    void updateView();

    /**
     * @brief Fallback si les textures sont introuvables.
     */
    QPixmap makeFallback(int w, int h, bool isLocked);

    /**
     * @brief Cherche, construit et charge les textures
     */
    void buildPixmaps();
};

#endif // STANDARDDOORVIEW_H
