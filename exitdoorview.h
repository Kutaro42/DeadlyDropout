/**
 * @file exitdoorview.h
 * @brief Déclaration de la vue graphique pour la porte de sortie.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef EXITDOORVIEW_H
#define EXITDOORVIEW_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include "exitdoormodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ExitDoorView — représentation visuelle de la porte de sortie.
//  Couleur rouge foncé = fermée, vert vif = ouverte.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ExitDoorView
 * @brief Rendu Qt de la porte gérant son code couleur en fonction de son verrouillage.
 */
class ExitDoorView : public QGraphicsRectItem {
private:
    ExitDoorModel* model;       ///< Référence vers le modèle logique de la porte.
    bool           cachedOpen;  ///< Mémorisation de l'état pour économiser des calculs de pinceau (Brush).

public:
    /**
     * @brief Constructeur de la vue graphique de la porte de sortie.
     * @param model Modèle physique source.
     * @param parent Élément parent dans la scène graphique Qt.
     */
    explicit ExitDoorView(ExitDoorModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Actualise les attributs graphiques (couleur, taille) selon l'état du modèle.
     */
    void updateView();
};

#endif // EXITDOORVIEW_H