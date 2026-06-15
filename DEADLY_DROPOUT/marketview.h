#ifndef MARKETVIEW_H
#define MARKETVIEW_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include "playermodel.h"

class MarketView : public QDialog {
    Q_OBJECT
public:
    explicit MarketView(QWidget* parent = nullptr);
    void refresh(PlayerModel* player);

signals:
    void sigBuy(int itemId);

private:
    QLabel* goldLabel;
    struct ItemBtn { QPushButton* btn; int id; };
    QVector<ItemBtn> itemBtns;

public:
    struct ItemDef { int id; QString name; int cost; QString desc; };
    static const QVector<ItemDef> ITEMS;
};

#endif
