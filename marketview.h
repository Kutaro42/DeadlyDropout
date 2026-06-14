#ifndef MARKETVIEW_H
#define MARKETVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include "playermodel.h"

class MarketView : public QWidget {
    Q_OBJECT
public:
    explicit MarketView(QWidget* parent = nullptr);
    void refresh(PlayerModel* player);

signals:
    void sigContinue();
    void sigBuy(int itemId);

private:
    QLabel*      goldLabel;
    QPushButton* continueBtn;
    struct ItemBtn { QPushButton* btn; int id; };
    QVector<ItemBtn> itemBtns;

    struct ItemDef {
        int    id;
        QString name;
        int    cost;
        QString desc;
    };
    static const QVector<ItemDef> ITEMS;
};

#endif // MARKETVIEW_H