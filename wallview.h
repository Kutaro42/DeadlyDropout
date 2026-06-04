/**
 * @file wallView.h
 * @brief Déclaration de la classe WallView définissant les visuels des murs.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef WALLVIEW_H
#define WALLVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "wallmodel.h"
#include "config.h"

class WallView : public QGraphicsPixmapItem {
public:
    explicit WallView(WallModel* model, QGraphicsItem* parent = nullptr);
};

#endif // WALLVIEW_H
