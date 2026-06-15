#ifndef MARKETCONTROLLER_H
#define MARKETCONTROLLER_H

#include <QObject>
#include <algorithm>
#include "marketview.h"
#include "playermodel.h"
#include "config.h"

class MarketController : public QObject {
    Q_OBJECT
public:
    explicit MarketController(QWidget* parentWidget, QObject* parent = nullptr);
    ~MarketController();
    void open(PlayerModel* player); // bloquant (QDialog::exec)
    MarketView* getView() const{return itsView;};

private slots:
    void sltBuy(int itemId);

private:
    MarketView*  itsView;
    PlayerModel* itsPlayer = nullptr;

signals:
    void sigClosed();
};

#endif
