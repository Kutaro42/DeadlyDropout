#ifndef MARKETCONTROLLER_H
#define MARKETCONTROLLER_H

#include <QObject>
#include "marketview.h"
#include "playermodel.h"

class MarketController : public QObject {
    Q_OBJECT
public:
    explicit MarketController(QObject* parent = nullptr);
    ~MarketController();

    void open(PlayerModel* player);
    MarketView* getView() const { return itsView; }

signals:
    void sigClosed();

private slots:
    void sltBuy(int itemId);

private:
    MarketView*  itsView;
    PlayerModel* itsPlayer = nullptr;
};

#endif // MARKETCONTROLLER_H