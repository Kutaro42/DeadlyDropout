/**
 * @file bossdoorview.h
 * @brief Déclaration de la vue de la porte de boss.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef BOSSDOORVIEW_H
#define BOSSDOORVIEW_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include "bossdoormodel.h"
#include "config.h"

/**
 * @class BossDoorView
 * @brief Vue de la porte de boss.
 *        Affiche un sprite « ouverte » tant que non verrouillée,
 *        puis bascule sur « verrouillée » après la traversée.
 */
class BossDoorView : public QGraphicsPixmapItem
{
public:
    explicit BossDoorView(BossDoorModel* model, QGraphicsItem* parent = nullptr);
    void updateView();

private:
    BossDoorModel* itsModel;
    bool           wasLocked;
    QPixmap        unlockedPixmap;
    QPixmap        lockedPixmap;

    void        buildPixmaps();
    static QPixmap makeFallback(int w, int h, bool isLocked);
};

#endif // BOSSDOORVIEW_H
